#ifndef __SETM32_PLATFORM_GPIO_H__
#define __SETM32_PLATFORM_GPIO_H__

#include <stdint.h>
#include <stm32_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t HGPIO;
#define HGPIO_NULL ((HGPIO)0xffffffff)

#define GPIO_MODE_INPUT     (0x00)
#define GPIO_MODE_OUTPUT    (0x01)
#define GPIO_MODE_ALTERNATE (0x02)
#define GPIO_MODE_ANALOG    (0x03)
#define GPIO_MODE_MASK      (0x03)

#define GPIO_PUPD_NOPULL    (0x00)
#define GPIO_PUPD_UP        (0x04)
#define GPIO_PUPD_DOWN      (0x08)
#define GPIO_PUPD_RESERVED  (0x0C)
#define GPIO_PUPD_MASK      (0x0C)

#define GPIO_OTYPE_PP       (0x00)
#define GPIO_OTYPE_OD       (0x10)
#define GPIO_OTYPE_MASK     (0x10)

enum gpio_group_type_e
{
    gpio_group_a,
    gpio_group_b,
    gpio_group_c,
    gpio_group_d,
    gpio_group_e,
    gpio_group_f,
    gpio_group_g,
    gpio_group_h,
    gpio_group_max,
};

enum gpio_port_type_e
{
    gpio_port_0,
    gpio_port_1,
    gpio_port_2,
    gpio_port_3,
    gpio_port_4,
    gpio_port_5,
    gpio_port_6,
    gpio_port_7,
    gpio_port_8,
    gpio_port_9,
    gpio_port_10,
    gpio_port_11,
    gpio_port_12,
    gpio_port_13,
    gpio_port_14,
    gpio_port_15,
    gpio_port_max,
};

enum gpio_alternate_e
{
    gpio_alternate_0,
    gpio_alternate_1,
    gpio_alternate_2,
    gpio_alternate_3,
    gpio_alternate_4,
    gpio_alternate_5,
    gpio_alternate_6,
    gpio_alternate_7,
    gpio_alternate_8,
    gpio_alternate_9,
    gpio_alternate_10,
    gpio_alternate_11,
    gpio_alternate_12,
    gpio_alternate_13,
    gpio_alternate_14,
    gpio_alternate_15,
    gpio_alternate_invalid,
};

enum gpio_mode_e
{
    gpio_mode_analog                     = (GPIO_MODE_ANALOG | GPIO_PUPD_NOPULL),

    gpio_mode_input_floating             = (GPIO_MODE_INPUT | GPIO_PUPD_NOPULL),
    gpio_mode_input_pull_up              = (GPIO_MODE_INPUT | GPIO_PUPD_UP),
    gpio_mode_input_pull_down            = (GPIO_MODE_INPUT | GPIO_PUPD_DOWN),

    gpio_mode_output_push_pull           = (GPIO_MODE_OUTPUT | GPIO_OTYPE_PP | GPIO_PUPD_NOPULL),
    gpio_mode_output_push_pull_up        = (GPIO_MODE_OUTPUT | GPIO_OTYPE_PP | GPIO_PUPD_UP),
    gpio_mode_output_push_pull_down      = (GPIO_MODE_OUTPUT | GPIO_OTYPE_PP | GPIO_PUPD_DOWN),

    gpio_mode_output_open_drain          = (GPIO_MODE_OUTPUT | GPIO_OTYPE_OD | GPIO_PUPD_NOPULL),
    gpio_mode_output_open_drain_up       = (GPIO_MODE_OUTPUT | GPIO_OTYPE_OD | GPIO_PUPD_UP),
    gpio_mode_output_open_drain_down     = (GPIO_MODE_OUTPUT | GPIO_OTYPE_OD | GPIO_PUPD_DOWN),

    gpio_mode_alternate_push_pull        = (GPIO_MODE_ALTERNATE | GPIO_OTYPE_PP | GPIO_PUPD_NOPULL),
    gpio_mode_alternate_push_pull_up     = (GPIO_MODE_ALTERNATE | GPIO_OTYPE_PP | GPIO_PUPD_UP),
    gpio_mode_alternate_push_pull_down   = (GPIO_MODE_ALTERNATE | GPIO_OTYPE_PP | GPIO_PUPD_DOWN),

    gpio_mode_alternate_open_drain       = (GPIO_MODE_ALTERNATE | GPIO_OTYPE_OD | GPIO_PUPD_NOPULL),
    gpio_mode_alternate_open_drain_up    = (GPIO_MODE_ALTERNATE | GPIO_OTYPE_OD | GPIO_PUPD_UP),
    gpio_mode_alternate_open_drain_down  = (GPIO_MODE_ALTERNATE | GPIO_OTYPE_OD | GPIO_PUPD_DOWN),
};

enum gpio_speed_e
{
    gpio_speed_low,
    gpio_speed_medium,
    gpio_speed_high,
    gpio_speed_very_high,
};

enum gpio_status_e
{
    gpio_status_low,
    gpio_status_high,
};

struct gpio_init_t
{
    enum gpio_group_type_e group;
    enum gpio_port_type_e  port;
    enum gpio_mode_e mode; 
    enum gpio_alternate_e alternate;
    enum gpio_speed_e speed;
    enum gpio_status_e status;
};

HGPIO gpio_init(const struct gpio_init_t *);
int32_t gpio_set_mode(HGPIO, enum gpio_mode_e);
int32_t gpio_set_speed(HGPIO, enum gpio_speed_e);
int32_t gpio_set_alternate(HGPIO hgpio, enum gpio_alternate_e);
int32_t gpio_set_status(HGPIO, enum gpio_status_e);
enum gpio_status_e gpio_get_status(HGPIO);
int32_t gpio_toggle_status(HGPIO);

#ifdef __cplusplus
}
#endif

#endif