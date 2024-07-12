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

#define test_stdio_sscanf() \
        do  { \
            uint64_t u64_val; \
            int64_t s64_val; \
            long double f80_val; \
            char str_val[16]; \
            bool bool_val; \
            sscanf( \
                "66, -88, 99.085, k8sio, 1", \
                "%llu, %lld, %Lf, %s, %hhu", \
                &u64_val, \
                &s64_val, \
                &f80_val, \
                str_val, \
                &bool_val \
            ); \
            INFOF( \
                "%llu, %lld, %Lf, %s %s", \
                u64_val, \
                s64_val, \
                f80_val, \
                str_val, \
                (bool_val ? "true" : "false") \
            ); \
        } while(0)

#define test_sys_env() \
        do { \
            char * path_var = getenv("PATH"); \
            if (path_var) \
                INFO(path_var); \
            path_var = getenv("GOPATH"); \
            if(path_var) \
                INFO(path_var); \
            system("ps -aux | grep \"docker\""); \
        } while(0)

void test_std_funcs(void )
{
    // test_stdio_sscanf();
    // test_sys_env();
    INFOF("%lld, %lld", INT_MAX, INT_MIN);  
    INFOF("%llu, %llu", UINT_MAX, 0);
    
}

void test_sys_memory_out(void )
{
    #define GB_ALLOC 1<<30
    uint64_t time_of_alloc_1GB = 0;
    int * i32_ptr = NULL;
    do {
        MALLOC_ARR(i32_ptr, int, GB_ALLOC);
        ++ time_of_alloc_1GB;
        INFOF("%p, %llu", i32_ptr, time_of_alloc_1GB);
        if(i32_ptr == NULL) {
            INFOF("alloc 1GB: %llu times", time_of_alloc_1GB);
            ERRORL(strerror(errno));
        }
        PAUSE(1000);
    }while(true);
}