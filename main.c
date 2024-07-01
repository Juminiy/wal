#include "wal.h"
#include "cowork.h"
#include "utils.h"

#include "cowork_test.h"

int main(int argc, char ** argv, char ** envp)
{

    // printf(
    //     "WAL State: %2d", 
    //     write_record_log("my_wal.log", "info, only test msg"));
    
    struct tasks_desc * tds = make_test_tasks_desc();
    coworker(
        tds, 
        get_tasks_option(0b111)
    );

    tasks_desc_free(tds); // callback do

    return 0;
}
