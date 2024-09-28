#ifndef __STM32_DEVICE_CAN_PROTOCOL_ACK_H__
#define __STM32_DEVICE_CAN_PROTOCOL_ACK_H__

#include <stdint.h>

void can_ack_init(void);
void can_ack_add(uint8_t, uint32_t, const uint8_t*, int32_t);
void can_ack_remove(uint8_t);
void can_ack_check(void (*can_send)(uint32_t, const uint8_t*, int32_t));

#endif