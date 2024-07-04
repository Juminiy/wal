#include "../include/c_test.h"
#include "../include/log.h"

void diff_prototype_ptr_cast()
{
    struct test_type_a a_val = {0xf0f0f0f0, 1.25}; // f0f0f0f0 3fa00000
    struct test_type_b b_val = {0, 0};

    INFOF("%d %d", sizeof(struct test_type_b), sizeof(struct test_type_a));
    INFOF("before a_val.a:%X, a_val.b:%X", a_val.a, a_val.b);
    INFOF("before b_val.a:%X, b_val.b:%X", b_val.a, b_val.b);

    b_val = *(struct test_type_b*)(&a_val);

    INFOF("after  b_val.a:%X, b_val.b:%X", b_val.a, b_val.b);
    
}