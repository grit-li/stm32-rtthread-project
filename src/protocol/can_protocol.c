#include <stdio.h>
#include <rtthread.h>
#include <stm32_gcc_attribute.h>
#include <syslog.h>
#include <version.h>
#include <common.h>
#include "stm32/system.h"
#include "device/can.h"
#include "can_protocol.h"
#include "can_ack.h"

#define CAN_DEVICE_ID (0x26)
#define CAN_TIMER_FREQ (100)
#define CAN_CHECK_TIMEOUT(timeout, tick, freq) ((tick % (timeout / freq)) == 0)

#define GET_CAN_DEVICE_STATE()  (canDeviceState)
#define SET_CAN_DEVICE_STATE(s) (GET_CAN_DEVICE_STATE() = (s))

enum CAN_DEVICE_STATE_E
{
    CAN_DEVICE_STATE_DISCOVER,
    CAN_DEVICE_STATE_SESSION_CONNECTION,
    CAN_DEVICE_STATE_SESSION_CONNECTIONED,
};

struct can_timer_t
{
    uint32_t tick;
    uint32_t time;
};

static __stm32_platform_aligned__(8) rt_uint8_t canStackBuffer[5 << 10];
static struct rt_thread canThreadHandle;
static rt_mutex_t canSendMutex = NULL;
static rt_timer_t canTimer = NULL;
static enum CAN_DEVICE_STATE_E canDeviceState = CAN_DEVICE_STATE_DISCOVER;
static uint8_t canDeviceSessionId = 0;
static struct rt_messagequeue canMq;
static struct can_timer_t canTimerBuffer[24];
static uint8_t canDeviceHeadTick = 0;

static void can_device_state_print(enum CAN_DEVICE_STATE_E old, enum CAN_DEVICE_STATE_E new)
{
    static const char* CAN_DEVICE_STATE_MAP[] = {
        "CAN_DEVICE_STATE_DISCOVER",
        "CAN_DEVICE_STATE_SESSION_CONNECTION",
        "CAN_DEVICE_STATE_SESSION_CONNECTIONED",
    };
    if(old != new) {
        LOGD("can device status update: %s -> %s\n", CAN_DEVICE_STATE_MAP[old], CAN_DEVICE_STATE_MAP[new]);
    }
}
static void can_device_set_state(enum CAN_DEVICE_STATE_E state)
{
    can_device_state_print(GET_CAN_DEVICE_STATE(), state);
    SET_CAN_DEVICE_STATE(state);
}
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
    LOGD("can device wait discover.\n");
}
static void can_send_packet(uint32_t id, uint8_t ack, const uint8_t* data, uint32_t size, uint16_t crc)
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
    if(ack != 0) {
        can_ack_add(ack, id, buffer, len);
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
    can_send_packet(CAN_HOST_ACK_ID(canDeviceSessionId), 0, (const uint8_t*)&msg, sizeof(struct CAN_NOTIFY_ACK_T), crc);
}
static void can_send_device_info(void)
{
    struct CAN_REPORT_INFO_PACKET_T msg;
    char buffer[16];
    uint32_t UID[3];
    rt_memset(UID, 0, sizeof(UID));
    rt_memset(buffer, 0, sizeof(buffer));
    rt_memset(&msg, 0, sizeof(struct CAN_REPORT_INFO_PACKET_T));
    msg.head.cmd = CAN_REPORT_INFO_CMD;
    msg.head.len = sizeof(struct CAN_REPORT_INFO_PACKET_T);
    stm32_env_get_project(buffer, sizeof(buffer));
    stm32_platform_device_id(UID, sizeof(UID)/sizeof(UID[0]));
    rt_snprintf((char*)msg.uuid, sizeof(msg.uuid), "%08X%08X%08X", UID[0], UID[1], UID[2]);
    rt_memcpy(msg.version, STM32_PROJECT_VERSION, sizeof(STM32_PROJECT_VERSION));
    rt_memcpy(msg.project, buffer, rt_strlen(buffer));
    rt_memcpy(msg.build, STM32_BUILD_TIME, sizeof(STM32_BUILD_TIME));
    uint16_t crc = can_cipher_crc16((const uint8_t *)&msg, sizeof(struct CAN_REPORT_INFO_PACKET_T)); 
    can_send_packet(CAN_HOST_AFFAIR_ID(canDeviceSessionId), CAN_REPORT_INFO_CMD_ACK, (const uint8_t*)&msg, sizeof(struct CAN_REPORT_INFO_PACKET_T), crc);
}
static void can_send_heart_packet(void)
{
    struct CAN_HEART_PACKET_T  msg;
    rt_memset(&msg, 0, sizeof(struct CAN_HEART_PACKET_T));
    msg.head.cmd = CAN_HEART_CMD;
    msg.head.len = sizeof(struct CAN_HEART_PACKET_T);
    msg.deviceID = CAN_DEVICE_ID;
    msg.sessionID = canDeviceSessionId;
    msg.status = 0;
    uint16_t crc = can_cipher_crc16((const uint8_t *)&msg, sizeof(struct CAN_HEART_PACKET_T)); 
    can_send_packet(CAN_HOST_AFFAIR_ID(canDeviceSessionId), 0, (const uint8_t*)&msg, sizeof(struct CAN_HEART_PACKET_T), crc);
}
static void can_parser_session_cmd(const struct CAN_SESSION_PACKET_T* packet)
{
    if(GET_CAN_DEVICE_STATE() != CAN_DEVICE_STATE_DISCOVER) {
        return;
    }
    LOGD("recv device id: %08x, session id: %d\n", packet->deviceID, packet->sessionID);
    if(packet->deviceID != CAN_DEVICE_ID) {
        LOGD("device error:%08x != %08x", CAN_DEVICE_ID, packet->deviceID);
        return;
    }
    uint8_t TOKENID     = CAN_DEVICE_TOKEN_ID(packet->sessionID);
    uint8_t REQUESTID   = CAN_DEVICE_REQUEST_ID(packet->sessionID);
    uint8_t AFFAIRID    = CAN_DEVICE_AFFAIR_ID(packet->sessionID);
    uint8_t DATAID      = CAN_DEVICE_DATA_ID(packet->sessionID);
    uint8_t ACKID       = CAN_DEVICE_ACK_ID(packet->sessionID);
    uint8_t NOTIFYID    = CAN_DEVICE_NOTIFY_ID(packet->sessionID);

    canDeviceSessionId  = packet->sessionID;
    can_device_set_state(CAN_DEVICE_STATE_SESSION_CONNECTION);
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
    can_send_device_info();
}
static void can_parser_reconn_cmd(const struct CAN_RECONNECT_DEVICE_PACKET_T* packet)
{
    if(packet->sessionID == canDeviceSessionId) {
        LOGD("reset can session.\n");
        can_ack_init();
        can_device_set_state(CAN_DEVICE_STATE_DISCOVER);
    }
}
static void can_parser_ack(uint8_t ack)
{
    switch(ack) {
        case CAN_REPORT_INFO_CMD_ACK:
            can_device_set_state(CAN_DEVICE_STATE_SESSION_CONNECTIONED);
            break;
        default: break;
    }
}
static void can_recv_data(const uint8_t* data, int32_t len)
{
    struct CAN_PROTOCOL_HEAD_T* header = (struct CAN_PROTOCOL_HEAD_T *)data;
    switch (header->cmd) {
        case CAN_SESSION_CMD:
            can_parser_session_cmd((const struct CAN_SESSION_PACKET_T *)data);
            break;
        case CAN_REPORT_INFO_CMD_ACK:
            can_ack_remove(header->cmd);
            can_parser_ack(header->cmd);
            break;
        case CAN_HEART_CMD:
            canDeviceHeadTick++;
            break;
        case CAN_RECONNECT_CMD:
            can_parser_reconn_cmd((const struct CAN_RECONNECT_DEVICE_PACKET_T *)data);
            break;
        default:
            LOGD("unkown cmd: 0x%02x\n", header->cmd);
            break;
    }
}
static struct can_notify_t canNotify = {
    can_recv_data,
};

static void can_check_device_state(uint32_t tick, uint32_t time)
{
    switch(GET_CAN_DEVICE_STATE()) {
        case CAN_DEVICE_STATE_DISCOVER:
                can_send_discover_packet();
                break;
            case CAN_DEVICE_STATE_SESSION_CONNECTION:
                if(CAN_CHECK_TIMEOUT(20000, tick, time)) {
                    LOGD("can device connection timeout.\n");
                    can_ack_init();
                    can_device_set_state(CAN_DEVICE_STATE_DISCOVER);
                }
                break;
            case CAN_DEVICE_STATE_SESSION_CONNECTIONED:
                if(CAN_CHECK_TIMEOUT(30000, tick, time)) {
                    if(canDeviceHeadTick == 0) {
                        LOGD("can device host lost.\n");
                        can_ack_init();
                        can_device_set_state(CAN_DEVICE_STATE_DISCOVER);
                    } else {
                        canDeviceHeadTick = 0;
                    }
                }
                can_send_heart_packet();
                break;
            default: break;
    }
}

static void can_timeout(void *parameter)
{
    static uint32_t mCanTickCount = 0;
    if(mCanTickCount++ >= 20000) {
        mCanTickCount = 0;
    }
    struct can_timer_t timer;
    timer.tick = mCanTickCount;
    timer.time = CAN_TIMER_FREQ;
    rt_mq_send(&canMq, &timer, sizeof(struct can_timer_t));
}
static void can_thread_entry(void *parameter)
{
    struct can_timer_t timer = { 0 };
    while(1) {
        if(rt_mq_recv(&canMq, &timer, sizeof(struct can_timer_t), RT_WAITING_FOREVER) == RT_EOK) {
            if(CAN_CHECK_TIMEOUT(1000, timer.tick, timer.time)) {
                can_check_device_state(timer.tick, timer.time);
            }
            if(CAN_CHECK_TIMEOUT(500, timer.tick, timer.time)) {
                can_ack_check(can_send);
            }
       }
    }
}

void can_srv_init(void)
{
    can_ack_init();
    canSendMutex = rt_mutex_create("can mutex", RT_IPC_FLAG_FIFO);
    canTimer = rt_timer_create("can time", can_timeout, RT_NULL, CAN_TIMER_FREQ, RT_TIMER_FLAG_SOFT_TIMER | RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(canTimer);
    can_device_init(&canNotify);
    rt_mq_init(&canMq, "can tmq", canTimerBuffer, sizeof(struct can_timer_t), sizeof(canTimerBuffer), RT_IPC_FLAG_FIFO);
    rt_thread_init(&canThreadHandle, "can protocol", can_thread_entry, RT_NULL, canStackBuffer, sizeof(canStackBuffer), 5, 20);
    rt_thread_startup(&canThreadHandle);
}