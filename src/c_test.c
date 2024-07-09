#include "../include/c_test.h"
#include "../include/log.h"

#include <stdlib.h>
#include <stddef.h>

void diff_prototype_ptr_cast(void )
{                                                                           // ieee754 underlying_repsentation
    struct test_type_a a_val = {5, 1.25};                                   // 40a00000 3fa00000
    struct test_type_b b_val = {1, 0};

    INFOF("%lu %lu", sizeof(struct test_type_b), sizeof(struct test_type_a));
    INFOF("before a_val.a:%X, a_val.b:%f", a_val.a, a_val.b); 
    INFOF("before b_val.a:%f, b_val.b:%X", b_val.a, b_val.b);

    b_val = *(struct test_type_b*)(&a_val);
    struct test_type_a_dup a_dup = *(struct test_type_a_dup*)(&a_val);

    INFOF("after  a_val.a:%X, a_val.b:%f", a_val.a, a_val.b);
    INFOF("after  a_dup.a:%X, a_dup.b:%f", a_dup.fa, a_dup.fb);
    INFOF("after  b_val.a:%X, b_val.b:%X", b_val.a, b_val.b);
   
}

void ptr_alloc_deref(void )
{
    const size_t arr2d_sz = 8, arr1d_sz = 8;
    int ** arr2d_ptr = NULL;
    MALLOC_ARR(arr2d_ptr, int*, arr2d_sz);

    for(int i = 0; i < arr2d_sz; i++)
        INFOF("%014p %014p %014p", arr2d_ptr+i, arr2d_ptr[i], arr2d_ptr);

    for(int i = 0; i < arr2d_sz; i++)
        MALLOC_ARR(arr2d_ptr[i], int, arr1d_sz);
    
    for(int i = 0; i < arr2d_sz; i++)
    {
        INFOF("%014p %014p %014p", arr2d_ptr+i, arr2d_ptr[i], arr2d_ptr);
        free(arr2d_ptr[i]);
    }

    free(arr2d_ptr);

}
