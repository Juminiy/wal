#include "../include/log.h"
#include "../include/utils.h"

#include <sys/time.h>

// definition log_mutex
pthread_mutex_t log_mutex;

// must be free after used
char* get_time_now_str(void )
{
    struct timeval timev;
    gettimeofday(&timev, NULL);
    time_t timep = timev.tv_sec;
    // struct tm *timeg = gmtime(&timep); // UTC
    struct tm *timel = localtime(&timep); // CST

    char *tm_buf = NULL;
    MALLOC_STR(tm_buf, 32);
    snprintf(
        tm_buf, 
        24, 
        "%04d-%02d-%02d %02d:%02d:%02d-%03ld", 
        timel->tm_year + 1900, // 2024
        timel->tm_mon + 1,     // 07
        timel->tm_mday,        // 01
        timel->tm_hour,        // 10
        timel->tm_min,         // 25
        timel->tm_sec,         // 09
        timev.tv_usec / 1000   // 315
    );

    return tm_buf;
}

/// @brief appendonly write time and log
/// @param file_name "fname" 
/// @param buf       "debug"| " info"| " warn"| "error"| "fatal" + $msg
/// @return 
int write_record_log
(const char *file_name, const char *buf) 
{
    int file_desc = open(file_name, O_WRONLY | O_CREAT | O_APPEND, WAL_PERM);

    // write buf
    // type
    write(file_desc, "type [", 6);
    write(file_desc, buf, 5);
    write(file_desc, "], ", 3);

    // time
    write(file_desc, "time [", 6);
    char *time_buf = get_time_now_str();
    write(file_desc, time_buf, strlen(time_buf));
    free(time_buf);
    write(file_desc, "], ", 3);

    // msg
    write(file_desc, "msg [", 5);
    write(file_desc, buf + 5, strlen(buf) - 5);
    write(file_desc, "]\n", 2);

    // flush log
    if (fsync(file_desc) == FSYNC_ERR_CODE) {
        perror(FSYNC_ERR_MSG);
        return WAL_ERR;
    }

    // close file
    close(file_desc);
    return WAL_OK;
}

// TODO: use yyjson to flat, fold
int write_record_log_json
(const char *file_name, const char *buf)
{
    return WAL_OK;
}

void write_log_example(void )
{
    INFOF(
        "WAL State: %2d\n", 
        write_record_log("my_wal.log", "info, only test msg")
    );
}

void log_log_example(void ) 
{
    INFO("i got smoke");
    INFOF("i got ruike %01d's smokes", 5);

    WARN("jumpling formula");
    WARNF("i got %02dth prize", 12);

    ERRORL("accept not equal to agree");
    ERRORF("i got %s formula", "糖妞");

    DEBUGL("38.8W");
    DEBUGF("i got %.1fW retainer", 38.8);

    FATAL("26");
    FATALF("i got %02d abortions", 26);
}
