#include "../include/log.h"
#include "../include/cowork.h"
#include "../include/utils.h"

#include "../include/linux_port.h"
#include "../include/json_flatten.h"
#include "../include/c_test.h"

// comment when dylib
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
        C_FREE(jstr);
    }

    return 0;
}

// comment when dylib
// __attribute__((destructor))
// void _callback_free(void )
// {
//     pthread_mutex_destroy(&log_mutex);
// }
