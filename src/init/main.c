#include <stdint.h>
#include <stdio.h>
#include <rtthread.h>
#include <syslog.h>
#include <stm32_types.h>

#include "device/led.h"
#include "protocol/can_protocol.h"

int main(void)
{
    led_init();
    can_srv_init();
    while(1) {
        led_toggle();
        rt_thread_mdelay(500);
    }    
}