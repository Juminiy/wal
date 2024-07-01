#pragma once

#ifndef COWORK_H
#define COWORK_H

#include "utils.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define err_task_id -1

typedef any_ptr (*task_func)(any_ptr);


struct task_desc {
    size_t tid;
    task_func task;
    any_ptr param_list;
};

struct tasks_desc {
    struct task_desc* tasks;
    size_t task_cnt;
    size_t max_thds;
};

struct tasks_option {
    bool cancel_all_when_exception; // true: cancel, false: dryrun
    bool exit_main_when_exception;  // true: exit(ERR_CODE), false: cancel_upper
    bool record_progress;           // true: task_counts, task_progress, task_successes, task_failures
};

struct tasks_sync {
    pthread_mutex_t mu_lock;
    pthread_cond_t cond_sema;

    size_t comp_counts;
    size_t success;
    size_t failure;
};

#define COWORK_OK 0
#define COWORK_ERR 1

#define CPU_MAX_THDS 12 // get from cpu xabi

struct tasks_option get_tasks_option(int );
struct tasks_sync* tasks_sync_init(void );
void tasks_sync_destroy(struct tasks_sync* );
int coworker(struct tasks_desc* , struct tasks_option );
void tasks_desc_free(struct tasks_desc* );

#endif// #define COWORK_H

