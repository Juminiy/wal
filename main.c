#include "log.h"
#include "cowork.h"
#include "utils.h"

#include "cowork_test.h"

__attribute__((constructor))
void _init_callback(void )
{
    pthread_mutex_init(&log_mutex, NULL);
}


int main(int argc, char ** argv, char ** envp)
{   
    cowork_example();
    return 0;
}

__attribute__((destructor))
void _callback_free(void )
{
    pthread_mutex_destroy(&log_mutex);
}
