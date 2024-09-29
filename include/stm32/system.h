#ifndef __SETM32_PLATFORM_SYSTEM_H__
#define __SETM32_PLATFORM_SYSTEM_H__

#include <stdint.h>
#include <stm32_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t stm32_platform_device_id(uint32_t*, int8_t);

#ifdef __cplusplus
}
#endif

#endif