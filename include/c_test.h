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

void diff_prototype_ptr_cast();

#endif// #define C_TEST_H