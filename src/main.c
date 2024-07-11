#include "../include/log.h"
#include "../include/cowork.h"
#include "../include/utils.h"

#include "../include/json_flatten.h"
#include "../include/json_test.h"
#include "../include/c_test.h"

__attribute__((constructor))
void _init_callback(void )
{
    pthread_mutex_init(&log_mutex, NULL);
}

int main(int argc, char** argv, char** envp)
{   
    CLOG(LOG_TYPE__INFO, "start");
    iter_json_file("data/json/100MB.json");
    CLOG(LOG_TYPE__INFO, "end");

    // test_write_json_string_sample_code();
    // test_write_json_string();

    return 0;
}

__attribute__((destructor))
void _callback_free(void )
{
    pthread_mutex_destroy(&log_mutex);
}
