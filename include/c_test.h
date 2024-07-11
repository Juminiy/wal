#pragma once

#ifndef C_TEST_H
#define C_TEST_H

struct test_type_a {
    int a;      // 4B
    float b;    // 4B
};

struct test_type_b {
    float a;    // 4B
    int b;      // 4B
};

struct test_type_a_dup {
    int fa;
    float fb;
};

void diff_prototype_ptr_cast(void );
void ptr_alloc_deref(void );
int test_c_hashmap(void );
void test_std_funcs(void );
void test_map_example(void );
void test_map_full_of_all(void );
void test_sys_memory_out(void );

#endif//#define C_TEST_H
