#include "../include/log.h"
#include "../include/cowork.h"
#include "../include/utils.h"

#include "../include/cowork_test.h"
#include "../include/json_test.h"

__attribute__((constructor))
void _init_callback(void )
{
    pthread_mutex_init(&log_mutex, NULL);
}

int main(int argc, char** argv, char** envp)
{   
    // test_iter_json_file("data/json/multiple_embedded_26s.json");
    
    // test_stu_map();
    INFO("stop use uthash.h");
    return 0;
}

__attribute__((destructor))
void _callback_free(void )
{
    pthread_mutex_destroy(&log_mutex);
}
