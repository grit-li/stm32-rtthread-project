#include <stdio.h>
#include <rtthread.h>
#include <stm32_gcc_attribute.h>
#include <syslog.h>
#include "device/can.h"
#include "can_protocol.h"

#define CAN_DEVICE_ID (0x26)
enum CAN_DEVICE_STATE_E
{
    CAN_DEVICE_STATE_DISCOVER,
    CAN_DEVICE_STATE_SESSION_CONNECTION,
    CAN_DEVICE_STATE_SESSION_CONNECTIONED,
};

static __stm32_platform_aligned__(8) rt_uint8_t canStackBuffer[5 << 10];
static struct rt_thread canThreadHandle;
static rt_mutex_t canSendMutex = NULL;
static enum CAN_DEVICE_STATE_E canDeviceState = CAN_DEVICE_STATE_DISCOVER;
static uint8_t canDeviceSessionId = 0;

static void can_send(uint32_t id, const uint8_t* data, int32_t size)
{
    struct can_device_msg_t msg;
    uint32_t sendIndex = 0;
    uint32_t packetNum = (size > CAN_DATA_MAX_LENGTH) ? ((size % CAN_DATA_MAX_LENGTH) == 0 ? (size / CAN_DATA_MAX_LENGTH) : (size / CAN_DATA_MAX_LENGTH) + 1) : 1;
    uint8_t  sendFlag  = 0;
    uint32_t offset = 0;
    uint32_t send   = 0;
    msg.id = id;
    msg.ide = 0;
    msg.rtr = 0;
    msg.size = size;
    rt_mutex_take(canSendMutex, RT_WAITING_FOREVER);
    while(offset < size) {
        send = (size - offset) > CAN_DATA_MAX_LENGTH ? CAN_DATA_MAX_LENGTH : (size - offset);
        msg.size = send + 1;
        sendIndex++;
        if(packetNum == 1) {
            sendFlag = CAN_PACKET_SINGLE;
        } else {
            if(sendIndex == 1) {
                sendFlag = (CAN_PACKET_START | (sendIndex & CAN_PACKET_NUM_MASK));
            } else if(sendIndex == packetNum) {
                sendFlag = (CAN_PACKET_END | (sendIndex & CAN_PACKET_NUM_MASK));
            } else {
                sendFlag = (CAN_PACKET_CONTINUE | (sendIndex & CAN_PACKET_NUM_MASK));
            }
        }
        msg.data[0] = sendFlag;
        rt_memcpy(&msg.data[1], (const uint8_t *)((char *)data + offset), send);
        can_device_send(&msg);
        offset += send;
        rt_thread_mdelay(5);
    }

    rt_mutex_release(canSendMutex);
}
static void can_send_discover_packet(void)
{
    struct can_device_msg_t msg;
    msg.id = CAN_DEVICE_ID;
    msg.ide = 1;
    msg.rtr = 1;
    msg.size = 0;
    rt_mutex_take(canSendMutex, RT_WAITING_FOREVER);
    can_device_send(&msg);
    rt_mutex_release(canSendMutex);
}
static void can_send_packet(uint32_t id, const uint8_t* data, uint32_t size, uint16_t crc)
{
    #define CAN_BUFFER_MAX 256
    int32_t len = (size > CAN_DATA_MAX_LENGTH) ? size + 2 : size;
    uint8_t buffer[CAN_BUFFER_MAX];
    if(len >= CAN_BUFFER_MAX) {
        LOGD("send can packet size > %d\n", CAN_BUFFER_MAX);
        return;
    }
    rt_memcpy(buffer, data, len);
    if(len > CAN_DATA_MAX_LENGTH) {
        rt_memcpy(&buffer[len], &crc, sizeof(uint16_t));
    }
    can_send(id, buffer, len);
}
static void can_send_ack(uint8_t ack, uint8_t state)
{
    struct CAN_NOTIFY_ACK_T msg;
    msg.head.cmd = ack;
    msg.head.len = sizeof(struct CAN_NOTIFY_ACK_T);
    msg.status = state;
    uint16_t crc = can_cipher_crc16((const uint8_t *)&msg, sizeof(struct CAN_NOTIFY_ACK_T)); 
    can_send_packet(CAN_ACK_ID(canDeviceSessionId), (const uint8_t*)&msg, sizeof(struct CAN_NOTIFY_ACK_T), crc);
}
static void can_send_device_info(void)
{
    struct CAN_REPORT_INFO_PACKET_T msg;
    rt_memset(&msg, 0, sizeof(struct CAN_REPORT_INFO_PACKET_T));
    msg.head.cmd = CAN_REPORT_INFO_CMD;
    msg.head.len = sizeof(struct CAN_REPORT_INFO_PACKET_T);
    rt_memcpy(msg.uuid, "123", 3);
    rt_memcpy(msg.version, "V1.0.0001", 10);
    rt_memcpy(msg.project, "CRCP11", 7);
    rt_memcpy(msg.build, "2024/09/22 21:42:36", 20);
    uint16_t crc = can_cipher_crc16((const uint8_t *)&msg, sizeof(struct CAN_REPORT_INFO_PACKET_T)); 
    can_send_packet(CAN_ACK_ID(canDeviceSessionId), (const uint8_t*)&msg, sizeof(struct CAN_REPORT_INFO_PACKET_T), crc);   
}
static void can_parser_session_cmd(const struct CAN_SESSION_PACKET_T* packet)
{
    if(canDeviceState != CAN_DEVICE_STATE_DISCOVER) {
        return;
    }
    LOGD("recv device id: %08x, session id: %d\n", packet->deviceID, packet->sessionID);
    if(packet->deviceID != CAN_DEVICE_ID) {
        LOGD("device error:%08x != %08x", CAN_DEVICE_ID, packet->deviceID);
        return;
    }
    uint8_t TOKENID     = CAN_TOKEN_ID(packet->sessionID);
    uint8_t REQUESTID   = CAN_REQUEST_ID(packet->sessionID);
    uint8_t AFFAIRID    = CAN_AFFAIR_ID(packet->sessionID);
    uint8_t DATAID      = CAN_DATA_ID(packet->sessionID);
    uint8_t ACKID       = CAN_ACK_ID(packet->sessionID);
    uint8_t NOTIFYID    = CAN_NOTIFY_ID(packet->sessionID);

    canDeviceSessionId  = packet->sessionID;
    canDeviceState = CAN_DEVICE_STATE_SESSION_CONNECTION;

    struct can_device_filter_t canSessionFiltter[] = {
        {3, 1, 16, 0, 0xA0, 0x00}, // 0x05 std/data
        {4, 1, 16, 0, 0x00, 0x00}, // 0x05 std/data
        // {5, 1, 16, 0, 0x00, 0x00}, // 0x05 std/data
        // {6, 1, 16, 0, 0x00, 0x00}, // 0x05 std/data
    };
    #define CAN_DEVICE_FILTTER(id1, id2) (id1 << 21 | id2 << 5)
    canSessionFiltter[0].FR1 = CAN_DEVICE_FILTTER(TOKENID, REQUESTID);
    canSessionFiltter[0].FR2 = CAN_DEVICE_FILTTER(AFFAIRID, DATAID);
    canSessionFiltter[1].FR1 = CAN_DEVICE_FILTTER(ACKID, NOTIFYID);
    canSessionFiltter[2].FR2 = CAN_DEVICE_FILTTER(0, 0);
    can_device_set_filter(&canSessionFiltter[0]);
    can_device_set_filter(&canSessionFiltter[1]);
    can_send_ack(CAN_SESSION_CMD_ACK, 0);
}
static void can_recv_data(const uint8_t* data, int32_t len)
{
    struct CAN_PROTOCOL_HEAD_T* header = (struct CAN_PROTOCOL_HEAD_T *)data;
    LOGD("cmd: 0x%02x\n", header->cmd);
    switch (header->cmd) {
        case CAN_SESSION_CMD: can_parser_session_cmd((const struct CAN_SESSION_PACKET_T *)data); break;
        case CAN_REPORT_INFO_CMD_ACK: canDeviceState = CAN_DEVICE_STATE_SESSION_CONNECTIONED; break;
        default:
            break;
    }
}
static struct can_notify_t canNotify = {
    can_recv_data,
};

static void can_thread_entry(void *parameter)
{
    while(1) {
        switch(canDeviceState) {
            case CAN_DEVICE_STATE_DISCOVER:
                can_send_discover_packet();
                break;
            case CAN_DEVICE_STATE_SESSION_CONNECTION:
                can_send_device_info();
                break;
            case CAN_DEVICE_STATE_SESSION_CONNECTIONED:
            default: break;
        }
        rt_thread_mdelay(1000);
    }
}

void can_srv_init(void)
{
    canSendMutex = rt_mutex_create("can mutex", RT_IPC_FLAG_FIFO);
    can_device_init(&canNotify);
    rt_thread_init(&canThreadHandle, "can protocol", can_thread_entry, RT_NULL, canStackBuffer, sizeof(canStackBuffer), 5, 20);
    rt_thread_startup(&canThreadHandle);
}