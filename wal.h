#pragma once

#ifndef WAL_H
#define WAL_H

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

char* get_time_now_str(void );
int write_record_log(const char *, const char *);
int write_record_log_json(const char *, const char *);

#endif// #define WAL_H

