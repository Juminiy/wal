#pragma once

#ifndef COWORK_TEST_H
#define COWORK_TEST_H

#include "utils.h"
#include "cowork.h"
#include "stdlib.h"

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

#define decl_test_fnx(xid, xms) \
        any_ptr test_fn##xid(any_ptr _ptr) \
        { \
            INFOF("start tid: %02d", xid); \
            call_fnx_stack(xms); \
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
            return NULL; \
        }

struct tasks_desc* make_test_tasks_desc(size_t );

void cowork_example(void );

#endif// #define COWORK_TEST_H
