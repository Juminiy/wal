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

void diff_prototype_ptr_cast();
void ptr_alloc_deref();
int test_c_hashmap();

#endif// #define C_TEST_H