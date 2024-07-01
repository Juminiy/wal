#include "wal.h"

// must be free after used
char* get_time_now_str(void )
{
    time_t tt;
    time(&tt);
    struct tm *loc_tm = localtime(&tt);

    char *tm_buf = (char*)(malloc(32 * sizeof(char)));
    snprintf(
        tm_buf, 
        20, 
        "%04d-%02d-%02d %02d:%02d:%02d", 
        loc_tm->tm_year + 1900, // 2024
        loc_tm->tm_mon + 1,     // 07
        loc_tm->tm_mday,        // 01
        loc_tm->tm_hour,        // 10
        loc_tm->tm_min,         // 25
        loc_tm->tm_sec          // 09
    );

    return tm_buf;
}

// appendonly write time and log
int write_record_log
(const char *file_name, const char *buf) 
{
    int file_desc = open(file_name, O_WRONLY | O_CREAT | O_APPEND, WAL_PERM);

    write(file_desc, "time [", 6);
    char *time_buf = get_time_now_str();
    write(file_desc, time_buf, strlen(time_buf));
    free(time_buf);
    write(file_desc, "], ", 3);

    write(file_desc, "msg [", 5);
    write(file_desc, buf, strlen(buf));
    write(file_desc, "]\n", 2);


    if (fsync(file_desc) == FSYNC_ERR_CODE) {
        perror(FSYNC_ERR_MSG);
        return WAL_ERR;
    }

    close(file_desc);
    return WAL_OK;
}

// TODO: use yyjson to flat, fold
int write_record_log_json
(const char *file_name, const char *buf)
{
    return WAL_OK;
}
