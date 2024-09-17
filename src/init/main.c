#include <stdint.h>
#include <stdio.h>
#include <rtthread.h>
#include <syslog.h>
#include <stm32_types.h>

int main(void)
{
    while(1) {
        rt_thread_mdelay(500);
        LOGD("hello stm32 rtthread!\n");
    }    
}