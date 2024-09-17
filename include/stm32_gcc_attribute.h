#ifndef __STM32_GCC_ATTRIBUTE_H__
#define __STM32_GCC_ATTRIBUTE_H__

#define __stm32_platform_set_attribute__(param) __attribute__(param)

#define __stm32_platform_section__(param)       __stm32_platform_set_attribute__((section(param)))

#define __stm32_platform_aligned__(param)       __stm32_platform_set_attribute__((aligned (param)))

#define __stm32_platform_at__(param)            __stm32_platform_set_attribute__((at(param)))

#define __stm32_platform_format__(fmt, m, n)    __stm32_platform_set_attribute__((format(fmt, m, n)))

#define __stm32_platform_packed__               __stm32_platform_set_attribute__((__packed__))

#define __stm32_platform_noreturn__             __stm32_platform_set_attribute__((noreturn))

#define __stm32_platform_const__                __stm32_platform_set_attribute__((const))

#define __stm32_platform_constructor__          __stm32_platform_set_attribute__((constructor))

#define __stm32_platform_destructor__           __stm32_platform_set_attribute__((destructor))

#define __stm32_platform_always_inline__        __stm32_platform_set_attribute__((always_inline)) 

#define __stm32_platform_used__                 __stm32_platform_set_attribute__((used))

#define __stm32_platform_unused__               __stm32_platform_set_attribute__((unused))

#define __stm32_platform_weak__                 __stm32_platform_set_attribute__((weak))

#define __stm32_platform_deprecated(x)__        __stm32_platform_set_attribute__((deprecated(x)))

#define __stm32_platform_bitwise__              __stm32_platform_set_attribute__((bitwise))

#define __stm32_platform_naked__                __stm32_platform_set_attribute__((naked))

#endif