#ifndef __SETM32_PLATFORM_IVT_H__
#define __SETM32_PLATFORM_IVT_H__

#include <stdint.h>
#include <stm32_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*irq_vector_handler)(void);

enum irq_vector_type_e
{
    irq_vector_type_systick,
    irq_vector_type_wwdg,
    irq_vector_type_pvd,
    irq_vector_type_tamper,
    irq_vector_type_rcc,
    irq_vector_type_flash,
    irq_vector_type_rtc,
    irq_vector_type_rtc_alarm,
    irq_vector_type_fsmc,
    irq_vector_type_sdio,
    irq_vector_type_exti0,
    irq_vector_type_exti1,
    irq_vector_type_exti2,
    irq_vector_type_exti3,
    irq_vector_type_exti4,
    irq_vector_type_exti5,
    irq_vector_type_exti6,
    irq_vector_type_exti7,
    irq_vector_type_exti8,
    irq_vector_type_exti9,
    irq_vector_type_exti10,
    irq_vector_type_exti11,
    irq_vector_type_exti12,
    irq_vector_type_exti13,
    irq_vector_type_exti14,
    irq_vector_type_exti15,
    irq_vector_type_dma1_channel1,
    irq_vector_type_dma1_channel2,
    irq_vector_type_dma1_channel3,
    irq_vector_type_dma1_channel4,
    irq_vector_type_dma1_channel5,
    irq_vector_type_dma1_channel6,
    irq_vector_type_dma1_channel7,
    irq_vector_type_dma2_channel1,
    irq_vector_type_dma2_channel2,
    irq_vector_type_dma2_channel3,
    irq_vector_type_dma2_channel4,
    irq_vector_type_dma2_channel5,
    irq_vector_type_adc1,
    irq_vector_type_adc2,
    irq_vector_type_adc3,
    irq_vector_type_usb_hp,
    irq_vector_type_usb_lp,
    irq_vector_type_usb_wakeup,
    irq_vector_type_can1_tx,
    irq_vector_type_can1_rx0,
    irq_vector_type_can1_rx1,
    irq_vector_type_can1_sce,
    irq_vector_type_i2c1_ev,
    irq_vector_type_i2c1_er,
    irq_vector_type_i2c2_ev,
    irq_vector_type_i2c2_er,
    irq_vector_type_spi1,
    irq_vector_type_spi2,
    irq_vector_type_spi3,
    irq_vector_type_usart1,
    irq_vector_type_usart2,
    irq_vector_type_usart3,
    irq_vector_type_uart4,
    irq_vector_type_uart5,
    irq_vector_type_tim1_brk,
    irq_vector_type_tim1_up,
    irq_vector_type_tim1_trg_com,
    irq_vector_type_tim1_cc,
    irq_vector_type_tim2,
    irq_vector_type_tim3,
    irq_vector_type_tim4,
    irq_vector_type_tim5,
    irq_vector_type_tim6,
    irq_vector_type_tim7,
    irq_vector_type_tim8_brk,
    irq_vector_type_tim8_up,
    irq_vector_type_tim8_trg_com,
    irq_vector_type_tim8_cc,

    irq_vector_type_max,
};

typedef void (*vector_table_entry_t)(void);
struct vector_table_t {
    unsigned int* initial_sp_value;
    vector_table_entry_t reset;
    vector_table_entry_t nmi;
    vector_table_entry_t hard_fault;
    vector_table_entry_t memory_manage_fault;
    vector_table_entry_t bus_fault;
    vector_table_entry_t usage_fault;
    vector_table_entry_t reserved1[4];
    vector_table_entry_t svcall;
    vector_table_entry_t debug_monitor;
    vector_table_entry_t reserved2;
    vector_table_entry_t pend_sv;
    vector_table_entry_t systick;
    vector_table_entry_t irq[240];
};

enum irq_priority_group_e
{
    irq_priority_group_0,
    irq_priority_group_1,
    irq_priority_group_2,
    irq_priority_group_3,
    irq_priority_group_4,

    irq_priority_group_null = 0xFF,
};

enum irq_preempt_priority_e
{
    irq_preempt_priority_0,
    irq_preempt_priority_1,
    irq_preempt_priority_2,
    irq_preempt_priority_3,
};

enum irq_sub_priority_e
{
    irq_sub_priority_0,
    irq_sub_priority_1,
    irq_sub_priority_2,
    irq_sub_priority_3,
};

void irq_set_vector_table(uint32_t, uint32_t);
int32_t irq_register_handler(enum irq_vector_type_e, irq_vector_handler);
int32_t irq_unregister_handler(enum irq_vector_type_e);

int32_t irq_set_priority_group(enum irq_priority_group_e);
enum irq_priority_group_e irq_get_priority_group(void);

int32_t irq_set_priority(enum irq_vector_type_e, enum irq_preempt_priority_e, enum irq_sub_priority_e);
int32_t irq_get_priority(enum irq_vector_type_e, enum irq_preempt_priority_e *const, enum irq_sub_priority_e *const);

int32_t irq_enable(enum irq_vector_type_e);
int32_t irq_disable(enum irq_vector_type_e);

#ifdef __cplusplus
}
#endif

#endif
