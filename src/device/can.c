#include <rtthread.h>
#include <syslog.h>
#include <stdio.h>
#include <stm32_gcc_attribute.h>
#include "stm32/can.h"
#include "stm32/gpio.h"
#include "stm32/interrupt_vector_table.h"
#include "can.h"

#define CAN_PACKET_SINGLE       (0x00)
#define CAN_PACKET_START        (0x01 << 6)
#define CAN_PACKET_CONTINUE     (0x02 << 6)
#define CAN_PACKET_END          (0x03 << 6)

#define CAN_PACKET_TYPE_MASK    (0xC0)
#define CAN_PACKET_NUM_MASK     (0x3F)

static const struct gpio_init_t can_tx_config = {
    .group = gpio_group_b,
    .port = gpio_port_9,
    .mode = gpio_mode_alternate_push_pull,
    .alternate = gpio_alternate_invalid,
    .speed = gpio_speed_very_high,
    .status = gpio_status_low,
};
static const struct gpio_init_t can_rx_config = {
    .group = gpio_group_b,
    .port = gpio_port_8,
    .mode = gpio_mode_input_pull_up,
    .alternate = gpio_alternate_invalid,
    .speed = gpio_speed_very_high,
    .status = gpio_status_high,
};

static struct rt_messagequeue canMq;
static struct can_frame_t canFrameData[32];

static __stm32_platform_aligned__(8) rt_uint8_t canStackBuffer[5 << 10];
static struct rt_thread canThreadHandle;

#define CAN_DEVICE_PROTOCOL_BUFFER_MAX 256
static uint8_t canProtocolBuffer[CAN_DEVICE_PROTOCOL_BUFFER_MAX] = { 0 };
static uint8_t canProtocolBufferCount = 0;
static struct can_notify_t canNotify;
static const struct can_filter_t canDefaultFiltter[] = {
    {3, 1, 16, 0, 0xA0, 0x00}, // 0x05 std/data
};

static void printCanData(const struct can_frame_t* can)
{
    char buff[64] = { 0 };
    rt_snprintf(buff, sizeof(buff), "CAN ID: 0x%X, ide:%d, rtr:%d, len: %d -> ", can->ID, can->IDE, can->RTR, can->DLC);
    for (uint8_t i = 0; i < can->DLC; i++) {
        rt_snprintf(buff + rt_strlen(buff), sizeof(buff) - rt_strlen(buff), "%02X ", can->DATA[i]);
    }
    LOGD("%s\n", buff);
}

static const uint16_t can_crc16_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823, 0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

static void can_cipher_crc16_calc(uint16_t* crc, const uint8_t* data, int32_t len)
{
    uint16_t val = *crc;
    while(len > 0) {
        val = ((val & 0xff) << 8) ^ can_crc16_table[(((val & 0xFF00) >> 8) ^ *data) & 0xff];
        len--;
        data++;
    }
    *crc = val;
}

uint16_t can_cipher_crc16(const uint8_t* data, int32_t len)
{
    uint16_t crc = 0;
    can_cipher_crc16_calc(&crc, data, len);
    return crc;
}

static int32_t can_check(const struct can_frame_t* frame)
{
    if(frame->IDE == 1 || frame->RTR == 1) {
        return -1;
    }
    return 0;
}
static int32_t can_append_packet_data(const uint8_t* data, int32_t len)
{
    if(len <= 0) {
        LOGE("can single packet length error!\n");
        return -1;
    }
    if((canProtocolBufferCount + len) > CAN_DEVICE_PROTOCOL_BUFFER_MAX) {
        LOGE("can device buffer is full!\n");
        return -1;
    }
    rt_memcpy(&canProtocolBuffer[canProtocolBufferCount], data, len);
    canProtocolBufferCount += len;
    return canProtocolBufferCount;
}
static void can_dispose_single_packet(const uint8_t* data, int32_t size)
{
    if(can_append_packet_data(data, size) < 0) {
        canProtocolBufferCount = 0;
        return;
    }

    if(canNotify.can_recv_data) {
        canNotify.can_recv_data(canProtocolBuffer, canProtocolBufferCount);
    }
    //todo.
    canProtocolBufferCount = 0;
}
static void can_dispose_mult_packet(const uint8_t* data, int32_t size)
{
    if(can_append_packet_data(data, size) < 0) {
        canProtocolBufferCount = 0;
        return;
    }
    uint8_t len = canProtocolBuffer[0] + 2;

    if(len != canProtocolBufferCount) {
        LOGD("buffer error. %d != %d\n", len, canProtocolBufferCount);
        canProtocolBufferCount = 0;
        return;
    }
    uint16_t crc = canProtocolBuffer[canProtocolBufferCount - 1] << 8 | canProtocolBuffer[canProtocolBufferCount - 2];
    if(crc == can_cipher_crc16(canProtocolBuffer, canProtocolBufferCount -2)) {
        if(canNotify.can_recv_data) {
            canNotify.can_recv_data(canProtocolBuffer, canProtocolBufferCount - 2);
        }
    } else {
        LOGE("packet crc error! %04x != %04x\n", crc, can_cipher_crc16(canProtocolBuffer, canProtocolBufferCount - 2));
    }
    canProtocolBufferCount = 0;
}
static void can_parser_protocol(const struct can_frame_t* frame)
{
    if(can_check(frame) < 0) {
        LOGE("can frame format error! id: %08x, ide: %d, rtr: %d\n", frame->ID, frame->IDE, frame->RTR);
        return;
    }

    switch(frame->DATA[0] & CAN_PACKET_TYPE_MASK) {
        case CAN_PACKET_SINGLE:
            // LOGD("recv can single packet\n");
            can_dispose_single_packet(&frame->DATA[1], frame->DLC - 1);
            break;
        case CAN_PACKET_END:
            // LOGD("recv can mult packet\n");
            can_dispose_mult_packet(&frame->DATA[1], frame->DLC - 1);
            break;
        default:
            // LOGD("too few packets, waiting for the next packet\n");
            can_append_packet_data(&frame->DATA[1], frame->DLC - 1);
            break;
    }
}

static void irq_can0_handler(void)
{
    rt_interrupt_enter();
    struct can_frame_t buffer = { 0 };
    can_recv_data(&buffer);
    rt_mq_send(&canMq, &buffer, sizeof(struct can_frame_t));
    rt_interrupt_leave();
}

static void can_thread_entry(void *parameter)
{
    struct can_frame_t buffer = { 0 };
    while(1) {
       if(rt_mq_recv(&canMq, &buffer, sizeof(struct can_frame_t), RT_WAITING_FOREVER) == RT_EOK) {
        //    printCanData(&buffer);
           can_parser_protocol(&buffer);
       }
    }
}

void can_device_init(const struct can_notify_t* notify)
{
    rt_memcpy(&canNotify, notify, sizeof(struct can_notify_t));
    gpio_init(&can_tx_config);
    gpio_init(&can_rx_config);

    irq_set_priority(irq_vector_type_can1_rx0, irq_preempt_priority_0, irq_sub_priority_0);
    irq_register_handler(irq_vector_type_can1_rx0, irq_can0_handler);
    irq_enable(irq_vector_type_can1_rx0);

    can_init();
    for(uint32_t i = 0; i < sizeof(canDefaultFiltter)/sizeof(canDefaultFiltter); i++) {
        can_set_filter(&canDefaultFiltter[i]);
    }
    rt_mq_init(&canMq, "can mq", canFrameData, sizeof(struct can_frame_t), sizeof(canFrameData), RT_IPC_FLAG_FIFO);

    rt_thread_init(&canThreadHandle, "can recv", can_thread_entry, RT_NULL, canStackBuffer, sizeof(canStackBuffer), 4, 20);
    rt_thread_startup(&canThreadHandle);
}

void can_device_send(const struct can_device_msg_t* msg)
{
    struct can_frame_t can_data;
    can_data.ID = msg->id;
    can_data.IDE = msg->ide;
    can_data.RTR = msg->rtr;
    can_data.DLC = msg->size;
    for (int i = 0; i < 8; i++) {
        can_data.DATA[i] = msg->data[i];
    }
    can_send_data(&can_data);
}

void can_device_set_filter(const struct can_device_filter_t* filter)
{
    can_set_filter((const struct can_filter_t*)filter);
}

