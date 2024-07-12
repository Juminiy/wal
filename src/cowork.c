#include "../include/log.h"
#include "../include/cowork.h"

#include <stdlib.h>

/// @brief waitgroup
/// @param tdesc for tasks description
/// @param state for tasks syncronized
/// @param res for tasks return values
/// @param opts for tasks apply option
/// @return 
int coworker(struct tasks_desc* tdesc, 
            struct tasks_sync* state, 
            tasks_res res, 
            struct tasks_option opts)
{   
    int aval_thds = MINInteger(tdesc->max_thds, CPU_MAX_THDS);
    int batch_thds = MINInteger(tdesc->task_cnt, aval_thds);
    
    // state init
    state->batch_size = batch_thds;
    for(int i = 0; i < tdesc->task_cnt; ++i)
        tdesc->tasks[i].param_list = state;

    // start work batch in threads
    for (int thds_launched = 0; 
        thds_launched < tdesc->task_cnt; 
        thds_launched += batch_thds) 
    {   

        pthread_t thd[CPU_MAX_THDS];
        for(int i = 0; i < batch_thds; i++)
        {
            if (pthread_create(
                    thd + i, 
                    NULL, 
                    tdesc->tasks[i].task, 
                    tdesc->tasks[i].param_list
                )) 
            {
                ERRORL("cowork create error");
                return COWORK_ERR;
            }
        }

        #ifdef DEBUG
            DEBUGL("threads launched, wait for threads sync");
        #endif 

        pthread_mutex_lock(&state->mu_lock);
        while(state->comp_count - thds_launched < batch_thds)
        {
            pthread_cond_wait(&state->cond_sema, &state->mu_lock);
        }
        pthread_mutex_unlock(&state->mu_lock);

        for(int i = 0; i < batch_thds; i++)
        {
            if(pthread_join(thd[i], &res[i])) 
            {
                ERRORL("cowork join error");
                return COWORK_ERR;
            }
        }

        #ifdef DEBUG
            DEBUGL("batch over");
        #endif 

    }
    
    return COWORK_OK;
}

void tasks_desc_free(struct tasks_desc* tds)
{
    // for(int i = 0; i < tds->task_cnt; ++i)
    //     free(tds->tasks->param_list);
    free(tds->tasks);
    free(tds);
}

struct tasks_option get_tasks_option(int _bin_rep)
{
    struct tasks_option opts;
    if(_bin_rep & 0b001)
        opts.cancel_all_when_exception = true;
    else 
        opts.cancel_all_when_exception = false;

    if(_bin_rep & 0b010)
        opts.exit_main_when_exception = true;
    else 
        opts.exit_main_when_exception = false;

    if(_bin_rep & 0b100)
        opts.record_progress = true;
    else 
        opts.record_progress = false;
    return opts;
}

struct tasks_sync* tasks_sync_init(void )
{
    struct tasks_sync* state = NULL;
    MALLOC_OBJ(state, struct tasks_sync);

    pthread_mutex_init(&state->mu_lock, NULL);
    pthread_cond_init(&state->cond_sema, NULL);
    state->comp_count = 0;
    state->success = 0;
    state->failure = 0;

    return state;
}

void tasks_sync_free(struct tasks_sync* state)
{   
    pthread_cond_destroy(&state->cond_sema);
    pthread_mutex_destroy(&state->mu_lock);
    free(state);
}

tasks_res tasks_res_init(size_t sz)
{
    tasks_res tres = NULL;
    MALLOC_ARR(tres, any_ptr, sz);
    return tres;
}

void tasks_res_display(tasks_res res, size_t sz, display_task_res fn)
{
    if (res == NULL)
        return;
    for(size_t i = 0; i < sz; i++)
        if (res[i])
            fn(res[i]);
}

void tasks_res_free(tasks_res res, size_t sz)
{
    if (res == NULL)
        return;
    for(size_t i = 0; i < sz; i++)
    {
        free(res[i]);
    }
    free(res);
}
