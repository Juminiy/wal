#pragma once

#ifndef COWORK_TEST_H
#define COWORK_TEST_H

#include "utils.h"
#include "cowork.h"

any_ptr test_fn0(any_ptr _ptr);
any_ptr test_fn1(any_ptr _ptr);
any_ptr test_fn2(any_ptr _ptr);
any_ptr test_fn3(any_ptr _ptr);
any_ptr test_fn4(any_ptr _ptr);
any_ptr test_fn5(any_ptr _ptr);

#define assign_fnx(tdesc, id) \
        do { \
            tdesc.tid = id; \
            tdesc.task = test_fn ##id; \
        }while(0)

// fncall may vary from 
// 1. discard return value: return NULL
// 2. accept return value: return any_ptr
#define decl_test_fnx(xid, xms, fncall) \
        any_ptr test_fn##xid(any_ptr _ptr) \
        { \
            INFOF("thread id: %lu", pthread_self()); \
            INFOF("start tid: %02d", xid); \
            any_ptr val = fncall(xms); \
            struct tasks_sync* state = (struct tasks_sync*)(_ptr); \
            if (state) { \
                pthread_mutex_lock(&state->mu_lock); \
                state->comp_count++; \
                if(state->comp_count == state->batch_size){ \
                    pthread_cond_signal(&state->cond_sema); \
                } \
                pthread_mutex_unlock(&state->mu_lock); \
            } \
            INFOF("end tid: %02d", xid); \
            pthread_exit(val); \
            return NULL; \
        }

void cowork_example(void );
struct tasks_desc* make_test_tasks_desc(size_t );


struct test_retval {
    int i32_val;
    char* str_val;
    double f64_arr_val[8];
};

struct test_retval* make_faked_retval(void );
void display_faked_res(any_ptr );

// multiple threads safe merge sort
void merge_sort_mts(void );
// multiple threads safe longest common sequence
void lcs_mts(void );

#endif// #define COWORK_TEST_H
