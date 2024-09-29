#include <string.h>
#include <stm32_types.h>
#include "common.h"

void stm32_env_get_project(char* buff, unsigned char size)
{
    const char* cmd = stm32_get_cmdlind();
    const char* ret = strstr(cmd, "project");
    if(ret) {
        memcpy(buff, ret + 8, strlen(ret));
    }
}