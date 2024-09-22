#include <stm32/gpio.h>
#include <syslog.h>
#include "led.h"

static const struct gpio_init_t led_config = {
    .group = gpio_group_f,
    .port = gpio_port_7,
    .mode = gpio_mode_output_push_pull,
    .alternate = gpio_alternate_invalid,
    .speed = gpio_speed_very_high,
    .status = gpio_status_high,
};

static HGPIO hled = HGPIO_NULL;

static inline void LED(HGPIO h, int n)
{
    n ? gpio_set_status(h, gpio_status_low) : gpio_set_status(h, gpio_status_high);
    //LOGD("LED: %s\n", n ? "On" : "Off");
}

void led_init(void)
{
    hled = gpio_init(&led_config);
}

void led_toggle(void)
{
    static unsigned char color = 0;
    if(color++ % 2 == 0) {
        LED(hled, 0);
    } else {
        LED(hled, 1);
    }
}
