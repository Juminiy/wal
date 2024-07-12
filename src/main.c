#include "../include/log.h"
#include "../include/cowork.h"
#include "../include/utils.h"

#include "../include/linux_port.h"
#include "../include/json_flatten.h"

// __attribute__((constructor))
// void _init_callback(void )
// {
//     pthread_mutex_init(&log_mutex, NULL);
// }

int main(int argc, char** argv, char** envp)
{   
    // test_linux_port();
    char *jstr = json_flatten_of("{\"key\":1}");
    if(jstr) {
        puts(jstr);
        free(jstr);
    }
    
    return 0;
}

// __attribute__((destructor))
// void _callback_free(void )
// {
//     pthread_mutex_destroy(&log_mutex);
// }
