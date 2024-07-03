#include "../thirdparty/yyjson/yyjson.h" 
#include "../include/log.h"
#include "../include/utils.h"

#include "../include/json_test.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

void test_read_json_string(void )
{   
    char *jstr;
    READ_FILE_BUF("data/text/github_api_users_{user}.txt", "r", jstr);
    INFO("get file buffer");

    yyjson_doc *jdoc = yyjson_read(jstr, strlen(jstr), 0X0);
    yyjson_val *jroot = yyjson_doc_get_root(jdoc);
    INFO("get json doc");

    yyjson_val *id_val =yyjson_obj_get(jroot, "id");
    yyjson_val *name_val = yyjson_obj_get(jroot, "login");
    INFOF(
        "id [%d] name [%s]", 
        (int)yyjson_get_int(id_val),
        (const char *)yyjson_get_str(name_val) 
    );

    // yyjson_val *arr = yyjson_obj_get(jroot, "arr");
    // size_t idx_i, max_m;
    // yyjson_val *obj;
    // yyjson_arr_foreach(arr, idx_i, max_m, obj) {
    //     yyjson_val *id_val = yyjson_obj_get(obj, "id");
    //     yyjson_val *name_val = yyjson_obj_get(obj, "login");
    //     INFOF(
    //         "id [%d], name: [%s]",
    //         (int)yyjson_get_int(id_val),
    //         (const char *)yyjson_get_str(name_val)
    //     );
    // }

    yyjson_doc_free(jdoc);
    free(jstr);

}

void test_write_json_string(void )
{

}

void test_read_json_file(void )
{

}

void test_write_json_file(void )
{

}