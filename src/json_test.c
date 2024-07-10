#include "../include/log.h"
#include "../include/utils.h"

#include "../include/json_test.h"


void test_read_json_string_sample_code(void )
{
    const char *json = "{\"name\":\"Mash\",\"star\":4,\"hits\":[2,2,1,3]}";

    // Read JSON and get root
    yyjson_doc *doc = yyjson_read(json, strlen(json), 0);
    yyjson_val *root = yyjson_doc_get_root(doc);

    // Get root["name"]
    yyjson_val *name = yyjson_obj_get(root, "name");
    INFOF("name: %s", yyjson_get_str(name));
    INFOF("name length:%lu", yyjson_get_len(name));

    // Get root["star"]
    yyjson_val *star = yyjson_obj_get(root, "star");
    INFOF("star: %d", yyjson_get_int(star));

    // Get root["hits"], iterate over the array
    yyjson_val *hits = yyjson_obj_get(root, "hits");
    size_t idx, max;
    yyjson_val *hit;
    yyjson_arr_foreach(hits, idx, max, hit) {
        INFOF("hit%lu: %d", idx, yyjson_get_int(hit));
    }

    // Free the doc
    yyjson_doc_free(doc);
}

void test_read_json_string(void )
{   
    char *jstr = NULL;
    READ_FILE_BUF("data/text/github_api_users.txt", "r", jstr);
    INFO("get file buffer"); 
    // INFO(jstr);

    yyjson_doc *jdoc = yyjson_read(jstr, strlen(jstr), 0);
    yyjson_val *jroot = yyjson_doc_get_root(jdoc);
    INFO("get json doc");

    // yyjson_val *id_val = yyjson_obj_get(jroot, "id");
    // yyjson_val *name_val = yyjson_obj_get(jroot, "login");
    // INFOF(
    //     "id [%d] name [%s]", 
    //     (int)yyjson_get_int(id_val),
    //     (const char *)yyjson_get_str(name_val) 
    // );

    size_t idx_i, max_m;
    yyjson_val *obj;
    yyjson_arr_foreach(jroot, idx_i, max_m, obj) {
        yyjson_val *id_val = yyjson_obj_get(obj, "id");
        yyjson_val *name_val = yyjson_obj_get(obj, "login");
        INFOF(
            "id [%d], name: [%s]",
            (int)yyjson_get_int(id_val),
            (const char *)yyjson_get_str(name_val)
        );
    }

    yyjson_doc_free(jdoc);
    free(jstr);

}

void test_write_json_string_sample_code(void )
{
    // Create a mutable doc
    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    // Set root["name"] and root["star"]
    yyjson_mut_obj_add_str(doc, root, "name", "Mash");
    yyjson_mut_obj_add_int(doc, root, "star", 4);

    // Set root["hits"] with an array
    int hits_arr[] = {2, 2, 1, 3};
    yyjson_mut_val *hits = yyjson_mut_arr_with_sint32(doc, hits_arr, 4);
    yyjson_mut_obj_add_val(doc, root, "hits", hits);

    // To string, minified
    const char *json = yyjson_mut_write(doc, 0, NULL);
    if (json) {
        INFOF("json: %s", json); // {"name":"Mash","star":4,"hits":[2,2,1,3]}
        free((void *)json);
    }

    // Free the doc
    yyjson_mut_doc_free(doc);
}

void test_write_json_string(void )
{

}

void test_read_json_file_sample_code(void )
{
    // Read JSON file, allowing comments and trailing commas
    yyjson_read_flag flg = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS;
    yyjson_read_err err;
    yyjson_doc *doc = yyjson_read_file("data/json/github_api_users_{user}.json", flg, NULL, &err);

    // Iterate over the root object
    if (doc) {
        yyjson_val *obj = yyjson_doc_get_root(doc);
        yyjson_obj_iter iter;
        yyjson_obj_iter_init(obj, &iter);
        yyjson_val *key, *val;
        while ((key = yyjson_obj_iter_next(&iter))) {
            val = yyjson_obj_iter_get_val(key);
            INFOF("%s: %s", yyjson_get_str(key), yyjson_get_type_desc(val));
        }
    } else {
        ERRORF("read error (%u): %s at position: %ld", err.code, err.msg, err.pos);
    }

    // Free the doc
    yyjson_doc_free(doc);
}

void test_read_json_file(void )
{
    yyjson_read_flag flg = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS;
    yyjson_read_err err;
    yyjson_doc *doc = yyjson_read_file("data/json/github_api_users.json", flg, NULL, &err);

    if(!doc){
        ERRORF("read error (%u): %s at position: %ld", err.code, err.msg, err.pos);
        return;
    }

    yyjson_val *root = yyjson_doc_get_root(doc);
    // iter_yyjson_doc_root(root);

    free(doc);
}   

void test_write_json_file_sample_code(void )
{
    // Read the JSON file as a mutable doc
    yyjson_doc *idoc = yyjson_read_file("data/json/github_api_users_dup.json", 0, NULL, NULL);
    yyjson_mut_doc *doc = yyjson_doc_mut_copy(idoc, NULL);
    yyjson_mut_val *obj = yyjson_mut_doc_get_root(doc);

    // Remove null values in root object
    yyjson_mut_obj_iter iter;
    yyjson_mut_obj_iter_init(obj, &iter);
    yyjson_mut_val *key, *val;
    while ((key = yyjson_mut_obj_iter_next(&iter))) {
        val = yyjson_mut_obj_iter_get_val(key);
        if (yyjson_mut_is_null(val)) {
            yyjson_mut_obj_iter_remove(&iter);
        }
    }

    // Write the json pretty, escape unicode
    yyjson_write_flag flg = YYJSON_WRITE_PRETTY | YYJSON_WRITE_ESCAPE_UNICODE;
    yyjson_write_err err;
    yyjson_mut_write_file("data/json/github_api_users_dup.json", doc, flg, NULL, &err);
    if (err.code) {
        ERRORF("write error (%u): %s", err.code, err.msg);
    }

    // Free the doc
    yyjson_doc_free(idoc);
    yyjson_mut_doc_free(doc);
}

void test_write_json_file(void )
{

}
