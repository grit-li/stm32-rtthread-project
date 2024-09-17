#ifndef __PLATFORM_SYSLOG_H__
#define __PLATFORM_SYSLOG_H__

#ifdef __cplusplus
extern "C" {
#endif

enum LOG_LEVEL_E
{
    LOG_LEVEL_ALL,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_SILENT,
};
void syslog_print(enum LOG_LEVEL_E, const char*, const char*, const char*, ...);

#define LOGD(fmt, ...) syslog_print(LOG_LEVEL_DEBUG, "DEBUG", __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) syslog_print(LOG_LEVEL_INFO, "INFO", __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) syslog_print(LOG_LEVEL_WARNING, "WARNING", __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) syslog_print(LOG_LEVEL_ERROR, "ERROR", __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOGF(fmt, ...) syslog_print(LOG_LEVEL_FATAL, "FATAL", __FUNCTION__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif