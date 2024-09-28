#include <stdio.h>
#include <string.h>
#include <stm32_gcc_attribute.h>
#include <syslog.h>
#include "can_ack.h"

#define CAN_ACK_BUFFER_MAX (128)
#define CAN_ACK_NUMBER     (24)

struct can_ack_t
{
    uint8_t ack;
    uint32_t id; 
    uint8_t buffer[CAN_ACK_BUFFER_MAX];
    int32_t size;
}__stm32_platform_packed__;

static struct can_ack_t canAckItem[CAN_ACK_NUMBER];

void can_ack_init(void)
{
    memset(canAckItem, 0, sizeof(canAckItem));
}
void can_ack_add(uint8_t ack, uint32_t id, const uint8_t* data, int32_t len)
{
    if(len >= CAN_ACK_BUFFER_MAX) {
        LOGW("ack(%02x) %08x data size %d > %d\n", ack, id, len, CAN_ACK_BUFFER_MAX);
        return;
    }
    uint8_t i = 0;
    for(i = 0; i < CAN_ACK_NUMBER; i++) {
        if(canAckItem[i].ack == 0) {
            canAckItem[i].ack = ack;
            canAckItem[i].id = id;
            memcpy(&canAckItem[i].buffer, data, len);
            canAckItem[i].size = len;
            LOGD("add ack %02x\n", ack);
            break;
        }
    }
    if(i >= CAN_ACK_BUFFER_MAX) {
        LOGE("can ack queue is full.\n");
    }
}
void can_ack_remove(uint8_t ack)
{
    for(uint8_t i = 0; i < CAN_ACK_NUMBER; i++) {
        if(canAckItem[i].ack == ack) {
            canAckItem[i].ack = 0;
        }
    }
}
void can_ack_check(void (*can_send)(uint32_t, const uint8_t*, int32_t))
{
    for(uint8_t i = 0; i < CAN_ACK_NUMBER; i++) {
        if(canAckItem[i].ack != 0) {
            LOGD("check %d ack %02x timeout.\n", i, canAckItem[i].ack);
            can_send(canAckItem[i].id, canAckItem[i].buffer, canAckItem[i].size);
        }
    }
}