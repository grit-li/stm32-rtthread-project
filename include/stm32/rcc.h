#ifndef __SETM32_PLATFORM_RCC_H__
#define __SETM32_PLATFORM_RCC_H__

#include <stdint.h>
#include <stm32_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HZ(x)  (x)
#define KHZ(x) (x * 1000)
#define MHZ(x) (x * 1000000)

enum rcc_clock_tree_e
{
    rcc_clock_lse,
    rcc_clock_lsi,
    rcc_clock_msi,
    rcc_clock_hsi,
    rcc_clock_hse,

    rcc_clock_extern_sai1,
    rcc_clock_extern_sai2,

    rcc_clock_pll,
    rcc_clock_pllq,
    rcc_clock_pllp,

    rcc_clock_pllsai1r,
    rcc_clock_pllsai1q,
    rcc_clock_pllsai1p,

    rcc_clock_pllsai2r,
    rcc_clock_pllsai2p,

    rcc_clock_sysclk,
    rcc_clock_hclk,

    rcc_clock_powerclk,
    rcc_clock_ahbclk,
    rcc_clock_coreclk,
    rcc_clock_memoryclk,
    rcc_clock_dmaclk,
    rcc_clock_systickclk,
    rcc_clock_fclk,

    rcc_clock_apb1clk,
    rcc_clock_apb2clk,

    rcc_clock_apb1timerclk,
    rcc_clock_apb2timerclk,

    rcc_clock_rtcclk,
    rcc_clock_usart1clk,
    rcc_clock_usart2clk,
    rcc_clock_usart3clk,
    rcc_clock_usart4clk,
    rcc_clock_usart5clk,
    rcc_clock_lpusart1clk,

    rcc_clock_lptim1clk,
    rcc_clock_lptim2clk,

    rcc_clock_clk48,
    rcc_clock_adc1,
    rcc_clock_adc2,
    rcc_clock_adc3,

    rcc_clock_sai1,
    rcc_clock_sai2,

    rcc_clock_i2c1,
    rcc_clock_i2c2,
    rcc_clock_i2c3,

    rcc_clock_dfsdm1,

    rcc_clock_swpmi1,

    rcc_clock_mco,
    rcc_clock_lsco,
    rcc_clock_tree_max
};

enum rcc_clock_mul_e
{
    rcc_clock_mul_1 = 1,
    rcc_clock_mul_2,
    rcc_clock_mul_3,
    rcc_clock_mul_4,
    rcc_clock_mul_5,
    rcc_clock_mul_6,
    rcc_clock_mul_7,
    rcc_clock_mul_8,
    rcc_clock_mul_9,
    rcc_clock_mul_10,
    rcc_clock_mul_11,
    rcc_clock_mul_12,
    rcc_clock_mul_13,
    rcc_clock_mul_14,
    rcc_clock_mul_15,
    rcc_clock_mul_16,
    rcc_clock_mul_17,
    rcc_clock_mul_18,
    rcc_clock_mul_19,
    rcc_clock_mul_20,
    rcc_clock_mul_21,
    rcc_clock_mul_22,
    rcc_clock_mul_23,
    rcc_clock_mul_24,
    rcc_clock_mul_25,
    rcc_clock_mul_26,
    rcc_clock_mul_27,
    rcc_clock_mul_28,
    rcc_clock_mul_29,
    rcc_clock_mul_30,
    rcc_clock_mul_31,
    rcc_clock_mul_32,
    rcc_clock_mul_33,
    rcc_clock_mul_34,
    rcc_clock_mul_35,
    rcc_clock_mul_36,
    rcc_clock_mul_37,
    rcc_clock_mul_38,
    rcc_clock_mul_39,
    rcc_clock_mul_40,
    rcc_clock_mul_41,
    rcc_clock_mul_42,
    rcc_clock_mul_43,
    rcc_clock_mul_44,
    rcc_clock_mul_45,
    rcc_clock_mul_46,
    rcc_clock_mul_47,
    rcc_clock_mul_48,
    rcc_clock_mul_49,
    rcc_clock_mul_50,
    rcc_clock_mul_51,
    rcc_clock_mul_52,
    rcc_clock_mul_53,
    rcc_clock_mul_54,
    rcc_clock_mul_55,
    rcc_clock_mul_56,
    rcc_clock_mul_57,
    rcc_clock_mul_58,
    rcc_clock_mul_59,
    rcc_clock_mul_60,
    rcc_clock_mul_61,
    rcc_clock_mul_62,
    rcc_clock_mul_63,
    rcc_clock_mul_64,
    rcc_clock_mul_65,
    rcc_clock_mul_66,
    rcc_clock_mul_67,
    rcc_clock_mul_68,
    rcc_clock_mul_69,
    rcc_clock_mul_70,
    rcc_clock_mul_71,
    rcc_clock_mul_72,
    rcc_clock_mul_73,
    rcc_clock_mul_74,
    rcc_clock_mul_75,
    rcc_clock_mul_76,
    rcc_clock_mul_77,
    rcc_clock_mul_78,
    rcc_clock_mul_79,
    rcc_clock_mul_80,
    rcc_clock_mul_81,
    rcc_clock_mul_82,
    rcc_clock_mul_83,
    rcc_clock_mul_84,
    rcc_clock_mul_85,
    rcc_clock_mul_86,
};

enum rcc_clock_flash_wait_cycle_e
{
    rcc_clock_flash_wait_0ws,
    rcc_clock_flash_wait_1ws,
    rcc_clock_flash_wait_2ws,
    rcc_clock_flash_wait_3ws,
    rcc_clock_flash_wait_4ws,
};

enum rcc_pll_clock_source_e
{
    rcc_pll_clock_source_msi,
    rcc_pll_clock_source_hsi,
    rcc_pll_clock_source_hse,
};

enum rcc_rtc_clock_source_e
{
    rcc_rtc_clock_source_hse,
    rcc_rtc_clock_source_lse,
    rcc_rtc_clock_source_lsi,
};

enum rcc_system_clock_source_e
{
    rcc_system_clock_source_msi,
    rcc_system_clock_source_hsi,
    rcc_system_clock_source_hse,
    rcc_system_clock_source_pllclk,
};

enum rcc_clk48_source_e
{
    rcc_clk48_source_pllsai1q,
    rcc_clk48_source_pllq,
    rcc_clk48_source_msi,
};

enum rcc_adc_clock_source_e
{
    rcc_adc_clock_source_pllsai1r,
    rcc_adc_clock_source_pllsai2r,
    rcc_adc_clock_source_sysclk,
};

enum rcc_sai_clock_source_e
{
    rcc_sai_clock_source_pllsai1p,
    rcc_sai_clock_source_pllsai2p,
    rcc_sai_clock_source_pllp,
    rcc_sai_clock_source_ext_sai1,
    rcc_sai_clock_source_ext_sai2,
};

enum rcc_i2c_clock_source_e
{
    rcc_i2c_clock_source_apb1,
    rcc_i2c_clock_source_sysclk,
    rcc_i2c_clock_source_hsi,
};

enum rcc_dfsdm_clock_source_e
{
    rcc_dfsdm_clock_source_apb2,
    rcc_dfsdm_clock_source_sysclk,
};

enum rcc_usart_clock_source_e
{
    rcc_usart_clock_source_apb2,
    rcc_usart_clock_source_sysclk,
    rcc_usart_clock_source_hsi,
    rcc_usart_clock_source_lse,
};

enum rcc_lptim_clock_source_e
{
    rcc_lptim_clock_source_apb1,
    rcc_lptim_clock_source_lsi,
    rcc_lptim_clock_source_hsi,
    rcc_lptim_clock_source_lse,
};

enum rcc_swpmi_clock_source_e
{
    rcc_swpmi_clock_source_apb1,
    rcc_swpmi_clock_source_hsi,
};

enum rcc_mco_clock_source_e
{
    rcc_mco_clock_source_lse,
    rcc_mco_clock_source_lsi,
    rcc_mco_clock_source_hse,
    rcc_mco_clock_source_hsi,
    rcc_mco_clock_source_pllclk,
    rcc_mco_clock_source_sysclk,
    rcc_mco_clock_source_msi,
};

enum rcc_lsco_clock_source_e
{
    rcc_lsco_clock_source_lse,
    rcc_lsco_clock_source_lsi,
};

enum rcc_clock_prescaler_e
{
    rcc_clock_prescaler_1 = 1,
    rcc_clock_prescaler_2 = 2,
    rcc_clock_prescaler_3 = 3,
    rcc_clock_prescaler_4 = 4,
    rcc_clock_prescaler_5 = 5,
    rcc_clock_prescaler_6 = 6,
    rcc_clock_prescaler_7 = 7,
    rcc_clock_prescaler_8 = 8,
    rcc_clock_prescaler_16 = 16,
    rcc_clock_prescaler_32 = 32,
    rcc_clock_prescaler_64 = 64,
    rcc_clock_prescaler_128 = 128,
    rcc_clock_prescaler_256 = 256,
    rcc_clock_prescaler_512 = 512,
};

enum rcc_peripheral_clock_enable_e
{
    rcc_peripheral_clock_disable = 0,
    rcc_peripheral_clock_enable  = 1,
};

struct rcc_clock_enable_t
{
    enum rcc_peripheral_clock_enable_e IOPAEN;
    enum rcc_peripheral_clock_enable_e IOPBEN;
    enum rcc_peripheral_clock_enable_e IOPCEN;
    enum rcc_peripheral_clock_enable_e IOPDEN;
    enum rcc_peripheral_clock_enable_e IOPEEN;
    enum rcc_peripheral_clock_enable_e IOPFEN;
    enum rcc_peripheral_clock_enable_e IOPGEN;
    enum rcc_peripheral_clock_enable_e AFIOEN;
    enum rcc_peripheral_clock_enable_e RTCEN;
    enum rcc_peripheral_clock_enable_e USART1EN;
    enum rcc_peripheral_clock_enable_e USART2EN;
    enum rcc_peripheral_clock_enable_e USART3EN;
    enum rcc_peripheral_clock_enable_e USART4EN;
    enum rcc_peripheral_clock_enable_e USART5EN;
    enum rcc_peripheral_clock_enable_e LPUSART1EN;
    enum rcc_peripheral_clock_enable_e LPTIM1EN;
    enum rcc_peripheral_clock_enable_e LPTIM2EN;
    enum rcc_peripheral_clock_enable_e SWPMI1EN;
    enum rcc_peripheral_clock_enable_e MCOEN;
    enum rcc_peripheral_clock_enable_e LSCOEN;
    enum rcc_peripheral_clock_enable_e CLK48EN;
    enum rcc_peripheral_clock_enable_e I2C1EN;
    enum rcc_peripheral_clock_enable_e I2C2EN;
    enum rcc_peripheral_clock_enable_e I2C3EN;
    enum rcc_peripheral_clock_enable_e ADC1EN;
    enum rcc_peripheral_clock_enable_e ADC2EN;
    enum rcc_peripheral_clock_enable_e ADC3EN;
    enum rcc_peripheral_clock_enable_e SAI1EN;
    enum rcc_peripheral_clock_enable_e SAI2EN;
    enum rcc_peripheral_clock_enable_e DFSDM1EN;
    enum rcc_peripheral_clock_enable_e CAN1EN;
};

struct rcc_clock_extern_t
{
    uint32_t sai1;
    uint32_t sai2;
};
struct rcc_pll_clock_scale_t
{
    enum rcc_pll_clock_source_e pll_source;
    enum rcc_clock_prescaler_e pll_prescaler;
    enum rcc_clock_flash_wait_cycle_e wait_cycle;

    enum rcc_clock_mul_e pll_mul;
    enum rcc_clock_prescaler_e pllr_prescaler;
    enum rcc_clock_prescaler_e pllq_prescaler;
    enum rcc_clock_prescaler_e pllp_prescaler;

    enum rcc_clock_mul_e pllsai1_mul;
    enum rcc_clock_prescaler_e pllsai1r_prescaler;
    enum rcc_clock_prescaler_e pllsai1q_prescaler;
    enum rcc_clock_prescaler_e pllsai1p_prescaler;

    enum rcc_clock_mul_e pllsai2_mul;
    enum rcc_clock_prescaler_e pllsai2r_prescaler;
    enum rcc_clock_prescaler_e pllsai2q_prescaler;
    enum rcc_clock_prescaler_e pllsai2p_prescaler;
};

struct rcc_rtc_clock_scale_t
{
    enum rcc_rtc_clock_source_e rtc_source;
    enum rcc_clock_prescaler_e hse_prescaler;
};

struct rcc_system_clock_scale_t
{
    enum rcc_system_clock_source_e system_clock_source;
    enum rcc_clock_prescaler_e ahb_prescaler;
    enum rcc_clock_prescaler_e apb1_prescaler;
    enum rcc_clock_prescaler_e apb2_prescaler;
    enum rcc_clock_prescaler_e cortex_system_prescaler;
    enum rcc_clock_mul_e        apb1_timer_mul;
    enum rcc_clock_mul_e        apb2_timer_mul;
};

struct rcc_mco_clock_scale_t
{
    enum rcc_mco_clock_source_e mco_source;
    enum rcc_clock_prescaler_e mco_prescaler;
};

struct rcc_clock_scale_t
{
    uint32_t lse;
    uint32_t lsi;
    uint32_t msi;
    uint32_t hsi;
    uint32_t hse;
    struct rcc_clock_extern_t extern_clock;
    
    struct rcc_pll_clock_scale_t pll_clock;
    struct rcc_rtc_clock_scale_t rtc_clock;
    struct rcc_system_clock_scale_t system_clock;
    struct rcc_mco_clock_scale_t mco_clock;

    enum rcc_clk48_source_e clk48_source;
    enum rcc_adc_clock_source_e adc1_source;
    enum rcc_adc_clock_source_e adc2_source;
    enum rcc_adc_clock_source_e adc3_source;
    enum rcc_sai_clock_source_e sai1_source;
    enum rcc_sai_clock_source_e sai2_source;
    enum rcc_i2c_clock_source_e i2c1_source;
    enum rcc_i2c_clock_source_e i2c2_source;
    enum rcc_i2c_clock_source_e i2c3_source;
    enum rcc_dfsdm_clock_source_e dfsdm1_source;
    enum rcc_usart_clock_source_e usart1_source;
    enum rcc_usart_clock_source_e usart2_source;
    enum rcc_usart_clock_source_e usart3_source;
    enum rcc_usart_clock_source_e usart4_source;
    enum rcc_usart_clock_source_e usart5_source;
    enum rcc_usart_clock_source_e lpusart1_source;
    enum rcc_lptim_clock_source_e lptime1_source;
    enum rcc_lptim_clock_source_e lptime2_source;
    enum rcc_swpmi_clock_source_e swpmi1_source;
    enum rcc_lsco_clock_source_e lsco_source;

    struct rcc_clock_enable_t clock_enable;
};

void rcc_clock_setup_init(const struct rcc_clock_scale_t *);
void rcc_clock_scale_init(struct rcc_clock_scale_t *);
uint32_t rcc_get_clock_freq(enum rcc_clock_tree_e);
void rcc_print_clock_tree(void);

#ifdef __cplusplus
}
#endif

#endif
