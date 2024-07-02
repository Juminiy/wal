#pragma once

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define FSYNC_ERR_CODE -1
#define FSYNC_ERR_MSG "fsync error"

#define WAL_OK 0
#define WAL_ERR 1

#define WAL_PERM 0644

int write_record_log(const char *, const char *);
int write_record_log_json(const char *, const char *);

void write_log_example();

#define DEFAULT_LOG_PATH "app.log"
#define DEFAULT_LOG_BUF_LEN 64

#define LOG_TYPE_DEBUG "debug"
#define LOG_TYPE__INFO " info"
#define LOG_TYPE__WARN " warn"
#define LOG_TYPE_ERROR "error"
#define LOG_TYPE_FATAL "fatal"


#define ILOG(file_name, type, msg) \
        do { \
            char *app_buf = (char*)(malloc(5 + strlen(msg))); \
            strncpy(app_buf, type, 5); \
            strncpy(app_buf + 5, msg, strlen(msg)); \
            write_record_log(file_name, app_buf); \
            free(app_buf); \
        } while(0)

#define ILOG_FMT(file_name, type, fmt, ...) \
        do { \
            char log_buf[DEFAULT_LOG_BUF_LEN]; \
            int sz = snprintf(log_buf, DEFAULT_LOG_BUF_LEN, fmt, __VA_ARGS__); \
            log_buf[sz] = 0; \
            ILOG(file_name, type, log_buf); \
        } while(0)

#define INFO(msg)        ILOG(DEFAULT_LOG_PATH, LOG_TYPE__INFO, msg)
#define INFOF(fmt, ...)  ILOG_FMT(DEFAULT_LOG_PATH, LOG_TYPE__INFO, fmt, __VA_ARGS__)

#define WARN(msg)        ILOG(DEFAULT_LOG_PATH, LOG_TYPE__WARN, msg)
#define WARNF(fmt, ...)  ILOG_FMT(DEFAULT_LOG_PATH, LOG_TYPE__WARN, fmt, __VA_ARGS__)

#define ERRORL(msg)       ILOG(DEFAULT_LOG_PATH, LOG_TYPE_ERROR, msg)
#define ERRORF(fmt, ...) ILOG_FMT(DEFAULT_LOG_PATH, LOG_TYPE_ERROR, fmt, __VA_ARGS__)

#define DEBUGL(msg)       ILOG(DEFAULT_LOG_PATH, LOG_TYPE_DEBUG, msg)
#define DEBUGF(fmt, ...) ILOG_FMT(DEFAULT_LOG_PATH, LOG_TYPE_DEBUG, fmt, __VA_ARGS__)

#define FATAL(msg)       ILOG(DEFAULT_LOG_PATH, LOG_TYPE_FATAL, msg)
#define FATALF(fmt, ...) ILOG_FMT(DEFAULT_LOG_PATH, LOG_TYPE_FATAL, fmt, __VA_ARGS__)


#define CLOG(type, msg) \
        printf("type [%s], time [%s], msg [%s]\n", type, get_time_now_str(), log_info)
#define CLOG_FMT(type, fmt, ...) \
        do { \
            char log_buf[DEFAULT_LOG_BUF_LEN]; \
            int sz = snprintf(log_buf, DEFAULT_LOG_BUF_LEN, fmt, __VA_ARGS__); \
            log_buf[sz] = 0; \
            printf("type [%s], time [%s], msg [%s]\n", type, get_time_now_str(), log_buf); \
        } while(0)

void log_log_example(void );

#endif// #define LOG_H
