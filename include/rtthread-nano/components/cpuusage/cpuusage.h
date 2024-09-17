#ifndef __CPUUSAGE_H__
#define __CPUUSAGE_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor);

#ifdef __cplusplus
}
#endif

#endif