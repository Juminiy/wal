#include "cowork.h"
#include "cowork_test.h"

#include <stdio.h>
#include <time.h>

void call_fnx_stack(int ms)
{
    puts("DAG start");
    PAUSE(ms); 
    puts("DAG end");
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

#define decl_test_fnx(xid, xms) \
        any_ptr test_fn##xid(any_ptr _ptr) \
        { \
            if (_ptr) { \
                printf("start tid: %02d\n", *(int*)(_ptr)); \
            } \
            call_fnx_stack(xms); \
            if (_ptr) { \
                printf("end tid: %02d\n", *(int*)(_ptr)); \
            } \
            return NULL; \
        }

decl_test_fnx(0, 1000) // 1s
decl_test_fnx(1, 8500) // 8.5s
decl_test_fnx(2, 5700) // 5.7s
decl_test_fnx(3, 6300) // 6.3s
decl_test_fnx(4, 4000) // 4s
decl_test_fnx(5, 900) // 0.9s

struct tasks_desc* make_test_tasks_desc(void )
{
    struct tasks_desc* tdesc = 
        (struct tasks_desc*)malloc(sizeof(struct tasks_desc));

    tdesc->max_thds = CPU_MAX_THDS;
    tdesc->task_cnt = 6;
    tdesc->tasks = 
        (struct task_desc*)malloc(sizeof(struct task_desc) * 6);
    assign_fnx(tdesc->tasks[0], 0);
    assign_fnx(tdesc->tasks[1], 1);
    assign_fnx(tdesc->tasks[2], 2);
    assign_fnx(tdesc->tasks[3], 3);
    assign_fnx(tdesc->tasks[4], 4);
    assign_fnx(tdesc->tasks[5], 5);   

    return tdesc;
}
