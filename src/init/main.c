#include <stdint.h>
#include <stdio.h>
#include <rtthread.h>
#include <syslog.h>
#include <stm32_types.h>

#include "device/led.h"

int main(void)
{
    led_init();
    while(1) {
        led_toggle();
        rt_thread_mdelay(500);
    }    
}