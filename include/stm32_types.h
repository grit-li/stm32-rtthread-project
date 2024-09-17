#ifndef __STM32_TYPES_H__
#define __STM32_TYPES_H__

enum stm32_platform_bool_e
{
    STM32_PLATFORM_FALSE,
    STM32_PLATFORM_TRUE = !STM32_PLATFORM_FALSE,
};

enum stm32_platform_enable_e
{
    STM32_PLATFORM_DISABLE = STM32_PLATFORM_FALSE,
    STM32_PLATFORM_ENABLE  = STM32_PLATFORM_TRUE,
};

enum stm32_platform_errcode_e
{
    STM32_PLATFORM_ERRCODE_SUCCESS         =  0,
    STM32_PLATFORM_ERRCODE_FAILED          = -1,
    STM32_PLATFORM_ERRCODE_PARAM_INVAILD   = -2,
    STM32_PLATFORM_ERRCODE_UNKWON_PLATFORM = -3,
    STM32_PLATFORM_ERRCODE_NOT_SUPPORT     = -4,
};

const char* stm32_get_cmdlind(void);

#endif