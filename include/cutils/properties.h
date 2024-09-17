#ifndef __PLATFORM_PROPERTIES_H__
#define __PLATFORM_PROPERTIES_H__

#define MAX_PROPERTIE_NUM      20
#define MAX_PROPERTIE_KEY_LEN  20
#define MAX_PROPERTIE_VAL_LEN  20

#ifdef __cplusplus
extern "C" {
#endif

void property_get(const char* key, char* value, const char* def);
void property_set(const char* key, const char* value);
void property_print(void);

#ifdef __cplusplus
}
#endif

#endif