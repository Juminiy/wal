#include "../include/cowork.h"
#include "../include/log.h"
#include "../include/math.h"

#include "../include/cowork_test.h"

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

// pause of faked work
// return NULL
void call_fnx_stack(int ms)
{
    PAUSE(ms); 
}

// pause of faked work
// return any_ptr
any_ptr call_fnx_ret_val(int ms)
{
    PAUSE(ms);
    return make_faked_retval();
}

// any_ptr test_fn0(any_ptr _ptr) 
// {
//     int tid = err_task_id;
//     if (_ptr) {
//         tid = *(int*)(_ptr);
//         printf("running tid: %02d\n", tid);
//     } 
//     call_fnx_stack(1000); // 1s
//     return NULL;
// }

// any_ptr test_fn1(any_ptr _ptr) 
// {
//     int tid = err_task_id;
//     if (_ptr) {
//         tid = *(int*)(_ptr);
//         printf("running tid: %02d\n", tid);
//     } 
//     call_fnx_stack(1450); // 1.45s
//     return NULL;
// }

// any_ptr test_fn2(any_ptr _ptr) 
// {
//     int tid = err_task_id;
//     if (_ptr) {
//         tid = *(int*)(_ptr);
//         printf("running tid: %02d\n", tid);
//     } 
//     call_fnx_stack(2370); // 2.37s
//     return NULL;
// }

// any_ptr test_fn3(any_ptr _ptr) 
// {
//     int tid = err_task_id;
//     if (_ptr) {
//         tid = *(int*)(_ptr);
//         printf("running tid: %02d\n", tid);
//     } 
//     call_fnx_stack(3330); // 3.33s
//     return NULL;
// }

// any_ptr test_fn4(any_ptr _ptr) 
// {
//     int tid = err_task_id;
//     if (_ptr) {
//         tid = *(int*)(_ptr);
//         printf("running tid: %02d\n", tid);
//     } 
//     call_fnx_stack(900); // 0.9s
//     return NULL;
// }

// any_ptr test_fn5(any_ptr _ptr) 
// {
//     int tid = err_task_id;
//     if (_ptr) {
//         tid = *(int*)(_ptr);
//         printf("running tid: %02d\n", tid);
//     } 
//     call_fnx_stack(2200); // 2.2s
//     return NULL;
// }

decl_test_fnx(0, 1000, call_fnx_ret_val) // 1s
decl_test_fnx(1, 8500, call_fnx_ret_val) // 8.5s
decl_test_fnx(2, 5700, call_fnx_ret_val) // 5.7s
decl_test_fnx(3, 6300, call_fnx_ret_val) // 6.3s
decl_test_fnx(4, 4000, call_fnx_ret_val) // 4s
decl_test_fnx(5, 900, call_fnx_ret_val) // 0.9s

struct tasks_desc* make_test_tasks_desc(size_t sz)
{
    struct tasks_desc* tdesc = 
        (struct tasks_desc*)malloc(sizeof(struct tasks_desc));

    tdesc->max_thds = CPU_MAX_THDS;
    tdesc->task_cnt = sz;
    tdesc->tasks = 
        (struct task_desc*)malloc(sizeof(struct task_desc) * sz);
    // call_fnx_stack for each jobs
    assign_fnx(tdesc->tasks[0], 0); 
    assign_fnx(tdesc->tasks[1], 1); 
    assign_fnx(tdesc->tasks[2], 2); 
    assign_fnx(tdesc->tasks[3], 3); 
    assign_fnx(tdesc->tasks[4], 4); 
    assign_fnx(tdesc->tasks[5], 5);   

    return tdesc;
}

void cowork_example(void )
{
    size_t tasks_size = 6;
    struct tasks_desc* tdesc = make_test_tasks_desc(tasks_size);
    struct tasks_sync* tstate = tasks_sync_init();
    tasks_res tres = tasks_res_init(tasks_size);

    INFO("cowork start");

    int co_state = coworker(
        tdesc, 
        tstate, 
        tres, 
        get_tasks_option(0b111) 
    );

    INFO("cowork finished");
    INFOF("cowork signal: %d", co_state);

    // callback do
    tasks_desc_free(tdesc); 
    tasks_sync_free(tstate);
    tasks_res_display(tres, tasks_size, display_faked_res);
    tasks_res_free(tres, tasks_size);
}

struct test_retval* make_faked_retval()
{
    srand((unsigned int)time(NULL));
    struct test_retval* val = (struct test_retval*)malloc(sizeof(struct test_retval));

    val->i32_val = RAND_Int(0, 1024);

    int char_sz = RAND_Int(0, 16);
    val->str_val = (char*)malloc(sizeof(char) * char_sz); 
    RAND_Str(val->str_val, char_sz);

    RAND_F64Arr(val->f64_arr_val, 8, 0.0, 100.0);

    return val;
}

void display_faked_res(any_ptr r0)
{
    struct test_retval* val = (struct test_retval*)(r0);
    CLOG_FMT(
        LOG_TYPE__INFO, 
        "i32_val [%d], str_val [%s], f64_arr_val_avg [%.5f]", 
        val->i32_val, 
        val->str_val, 
        f64_arr_avg(val->f64_arr_val, 8)
    );
}
