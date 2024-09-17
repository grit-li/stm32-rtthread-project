#ifndef __SETM32_PLATFORM_SYSTICK_H__
#define __SETM32_PLATFORM_SYSTICK_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define systick_time_ms(ms) (ms)

enum systick_time_e
{
    systick_time_1ms    = systick_time_ms(1),
    systick_time_10ms   = systick_time_ms(10),
    systick_time_100ms  = systick_time_ms(100),
};

void systick_init(enum systick_time_e);
void systick_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
