#ifndef __SETM32_PLATFORM_CAN_H__
#define __SETM32_PLATFORM_CAN_H__

#include <stdint.h>
#include <stm32_types.h>

#ifdef __cplusplus
extern "C" {
#endif

enum can_frame_type_e
{
    can_frame_type_std,
    can_frame_type_ext,
};

enum can_transmission_type_e
{
    can_transmission_type_data,
    can_transmission_type_remote,
};

struct can_frame_t
{
    uint32_t ID;
    uint8_t IDE;
    uint8_t RTR;
    uint8_t DLC;
    uint8_t DATA[8];
};

struct can_filter_t
{
    uint8_t channel;
    uint8_t mode;
    uint8_t scale;
    uint8_t fifo;
    uint32_t FR1;
    uint32_t FR2;
};

int32_t can_init(void);
int32_t can_set_filter(const struct can_filter_t*);
int32_t can_send_data(const struct can_frame_t *);
int32_t can_recv_data(struct can_frame_t *);

#ifdef __cplusplus
}
#endif

#endif