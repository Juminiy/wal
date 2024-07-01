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
            int* iaddr = (int*)malloc(sizeof(int)); \
            *iaddr = id; \
            tdesc.param_list = iaddr; \
        }while(0)

struct tasks_desc* make_test_tasks_desc(void );

#endif// #define COWORK_TEST_H
