#ifndef __SETM32_DEVICE_CAN_H__
#define __SETM32_DEVICE_CAN_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct can_device_msg_t
{
    uint32_t id;
    uint8_t ide;
    uint8_t rtr;
    uint8_t size;
    uint8_t data[8];
};
struct can_device_filter_t
{
    uint8_t channel;
    uint8_t mode;
    uint8_t scale;
    uint8_t fifo;
    uint32_t FR1;
    uint32_t FR2;
};
struct can_notify_t
{
    void (*can_recv_data)(const uint8_t*, int32_t);
};
void can_device_init(const struct can_notify_t *);
void can_device_send(const struct can_device_msg_t *);
void can_device_set_filter(const struct can_device_filter_t*);
uint16_t can_cipher_crc16(const uint8_t*, int32_t);

#ifdef __cplusplus
}
#endif

#endif