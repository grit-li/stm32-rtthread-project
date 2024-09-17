#include "stm32_gcc_attribute.h"
#include "stm32/rcc.h"

static const struct rcc_clock_scale_t rcc_clock_config_72mhz = {
    .lse = HZ(0),
    .lsi = KHZ(40),
    .msi = MHZ(0),
    .hsi = MHZ(8),
    .hse = MHZ(8),

    .extern_clock = {
        .sai1 = HZ(0),
        .sai2 = HZ(0),
    },
    .pll_clock = {
        .pll_source = rcc_pll_clock_source_hse,
        .pll_prescaler = rcc_clock_prescaler_1,
        .wait_cycle = rcc_clock_flash_wait_2ws,
        .pll_mul = rcc_clock_mul_9,
        .pllr_prescaler = rcc_clock_prescaler_1,
        .pllq_prescaler = rcc_clock_prescaler_1,
        .pllp_prescaler = rcc_clock_prescaler_1,
        .pllsai1_mul = rcc_clock_mul_1,
        .pllsai1r_prescaler = rcc_clock_prescaler_1,
        .pllsai1q_prescaler = rcc_clock_prescaler_1,
        .pllsai1p_prescaler = rcc_clock_prescaler_1,
        .pllsai2_mul = rcc_clock_mul_1,
        .pllsai2r_prescaler = rcc_clock_prescaler_1,
        .pllsai2q_prescaler = rcc_clock_prescaler_1,
        .pllsai2p_prescaler = rcc_clock_prescaler_1,
    },
    .rtc_clock = {
        .rtc_source = rcc_rtc_clock_source_lsi,
        .hse_prescaler = rcc_clock_prescaler_128,
    },
    .system_clock = {
        .system_clock_source = rcc_system_clock_source_pllclk,
        .ahb_prescaler = rcc_clock_prescaler_1,
        .apb1_prescaler = rcc_clock_prescaler_2,
        .apb2_prescaler = rcc_clock_prescaler_1,
        .cortex_system_prescaler = rcc_clock_prescaler_1,
        .apb1_timer_mul = rcc_clock_mul_2,
        .apb2_timer_mul = rcc_clock_mul_1,
    },
    .mco_clock = {
        .mco_source = rcc_mco_clock_source_sysclk,
        .mco_prescaler = rcc_clock_prescaler_1,
    },

    .clk48_source = rcc_clk48_source_msi,
    .adc1_source = rcc_adc_clock_source_sysclk,
    .adc2_source = rcc_adc_clock_source_sysclk,
    .adc3_source = rcc_adc_clock_source_sysclk,
    .sai1_source = rcc_sai_clock_source_pllsai1p,
    .sai2_source = rcc_sai_clock_source_pllsai1p,
    .i2c1_source = rcc_i2c_clock_source_sysclk,
    .i2c2_source = rcc_i2c_clock_source_sysclk,
    .i2c3_source = rcc_i2c_clock_source_sysclk,
    .dfsdm1_source = rcc_dfsdm_clock_source_sysclk,
    .usart1_source = rcc_usart_clock_source_apb2,
    .usart2_source = rcc_usart_clock_source_apb2,
    .usart3_source = rcc_usart_clock_source_apb2,
    .usart4_source = rcc_usart_clock_source_apb2,
    .usart5_source = rcc_usart_clock_source_apb2,
    .lpusart1_source = rcc_usart_clock_source_apb2,
    .lptime1_source = rcc_lptim_clock_source_hsi,
    .lptime2_source = rcc_lptim_clock_source_hsi,
    .swpmi1_source = rcc_swpmi_clock_source_hsi,
    .lsco_source = rcc_lsco_clock_source_lse,
    .clock_enable = {
        .IOPAEN = rcc_peripheral_clock_enable,
        .IOPBEN = rcc_peripheral_clock_enable,
        .IOPCEN = rcc_peripheral_clock_enable,
        .IOPDEN = rcc_peripheral_clock_enable,
        .IOPEEN = rcc_peripheral_clock_enable,
        .IOPFEN = rcc_peripheral_clock_enable,
        .IOPGEN = rcc_peripheral_clock_enable,
        .AFIOEN = rcc_peripheral_clock_enable,
        .RTCEN = rcc_peripheral_clock_enable,
        .USART1EN = rcc_peripheral_clock_enable,
        .USART2EN = rcc_peripheral_clock_enable,
        .USART3EN = rcc_peripheral_clock_enable,
        .USART4EN = rcc_peripheral_clock_enable,
        .USART5EN = rcc_peripheral_clock_enable,
        .LPUSART1EN = rcc_peripheral_clock_enable,
        .LPTIM1EN = rcc_peripheral_clock_enable,
        .LPTIM2EN = rcc_peripheral_clock_enable,
        .SWPMI1EN = rcc_peripheral_clock_enable,
        .MCOEN = rcc_peripheral_clock_enable,
        .LSCOEN = rcc_peripheral_clock_enable,
        .CLK48EN = rcc_peripheral_clock_enable,
        .I2C1EN = rcc_peripheral_clock_enable,
        .I2C2EN = rcc_peripheral_clock_enable,
        .I2C3EN = rcc_peripheral_clock_enable,
        .ADC1EN = rcc_peripheral_clock_enable,
        .ADC2EN = rcc_peripheral_clock_enable,
        .ADC3EN = rcc_peripheral_clock_enable,
        .SAI1EN = rcc_peripheral_clock_enable,
        .SAI2EN = rcc_peripheral_clock_enable,
        .DFSDM1EN = rcc_peripheral_clock_enable,
        .CAN1EN = rcc_peripheral_clock_enable,
    },
};
static inline const struct rcc_clock_scale_t* system_clock_scale(void)
{
    return &rcc_clock_config_72mhz;
}

void boot_main(void)
{
    rcc_clock_setup_init(system_clock_scale());
}

