#ifndef __STM32_DEVICE_CAN_PROTOCOL_H__
#define __STM32_DEVICE_CAN_PROTOCOL_H__

#include <stdint.h>
#include <stm32_gcc_attribute.h>

#define CAN_DATA_MAX_LENGTH (7)
#define CAN_SESSION_CMD     (0x01)
#define CAN_SESSION_CMD_ACK (0x81)

#define CAN_REPORT_INFO_CMD     (0x02)
#define CAN_REPORT_INFO_CMD_ACK (0x82)

#define CAN_HEART_CMD           (0x02)

#define CAN_PACKET_ID(session_id, offset) (((session_id & 0x7F) << 4) | (offset & 0x0F))

#define CAN_TOKEN_PACKET_OFFSET     (0x05)
#define CAN_REQUEST_PACKET_OFFSET   (0x06)
#define CAN_AFFAIR_PACKET_OFFSET    (0x07)
#define CAN_DATA_PACKET_OFFSET      (0x08)
#define CAN_ACK_PACKET_OFFSET       (0x09)
#define CAN_NOTIFY_PACKET_OFFSET    (0x0A)

#define CAN_TOKEN_ID(id)        (CAN_PACKET_ID( 0, CAN_TOKEN_PACKET_OFFSET))
#define CAN_REQUEST_ID(id)      (CAN_PACKET_ID(id, CAN_REQUEST_PACKET_OFFSET))
#define CAN_AFFAIR_ID(id)       (CAN_PACKET_ID(id, CAN_AFFAIR_PACKET_OFFSET))
#define CAN_DATA_ID(id)         (CAN_PACKET_ID(id, CAN_DATA_PACKET_OFFSET))
#define CAN_ACK_ID(id)          (CAN_PACKET_ID(id, CAN_ACK_PACKET_OFFSET))
#define CAN_NOTIFY_ID(id)       (CAN_PACKET_ID(id, CAN_NOTIFY_PACKET_OFFSET))

#define CAN_GET_ID(id)          (id & 0x0000000F)
#define CAN_GET_SESSION_ID(id)  ((id & 0x000007F0) >> 4)

#define CAN_PACKET_SINGLE        (0x00)
#define CAN_PACKET_START         (0x01 << 6)
#define CAN_PACKET_CONTINUE      (0x02 << 6)
#define CAN_PACKET_END           (0x03 << 6)

#define CAN_PACKET_TYPE_MASK     (0xC0)
#define CAN_PACKET_NUM_MASK      (0x3F)

struct CAN_PROTOCOL_HEAD_T
{
    uint8_t  len;
    uint8_t  cmd;
}__stm32_platform_packed__;

struct CAN_NOTIFY_ACK_T
{
    struct CAN_PROTOCOL_HEAD_T head;
    uint8_t  status;
}__stm32_platform_packed__;

struct CAN_SESSION_PACKET_T
{
    struct CAN_PROTOCOL_HEAD_T head;
    uint8_t  deviceID;
    uint8_t  sessionID;
}__stm32_platform_packed__;

struct CAN_HEART_PACKET_T
{
    struct CAN_PROTOCOL_HEAD_T head;
    uint8_t  data[16];
}__stm32_platform_packed__;

struct CAN_REPORT_INFO_PACKET_T
{
    struct CAN_PROTOCOL_HEAD_T head;
    uint8_t  uuid[12];
    uint8_t  version[24];
    uint8_t  project[24];
    uint8_t  build[24];
}__stm32_platform_packed__;

void can_srv_init(void);
#endif