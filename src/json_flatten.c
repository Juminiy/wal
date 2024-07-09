#include "../include/log.h"
#include "../include/utils.h"

#include "../include/json_flatten.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <stdint.h>


void iter_json_string(const char * json_str)
{

}

void iter_json_file(const char * file_name)
{
    yyjson_read_flag flg = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS;
    yyjson_read_err err;
    yyjson_doc *doc = yyjson_read_file(file_name, flg, NULL, &err);

    if(!doc){
        ERRORF("read error (%u): %s at position: %lu", err.code, err.msg, err.pos);
        return;
    }

    yyjson_val *root = yyjson_doc_get_root(doc);
    struct json_flatten * jf = init_json_flatten();
    iter_yyjson_doc_root(root, "", jf);

    free(doc);
    // free_json_flatten(jf);
}  

void iter_yyjson_doc_root(
    const yyjson_val *root, 
    const char *key_prefix, 
    struct json_flatten * jf
)
{
    if(root == NULL)
        return;
    yyjson_type typ = yyjson_get_type(root);
    yyjson_subtype subtyp = yyjson_get_subtype(root);
    // const char * typ_desc = yyjson_get_type_desc(root);
    char * type_desc_gnuc99;

    const char *raw_val;
    bool bool_val;
    uint64_t u64_val;
    int64_t s64_val;
    double f64_val;
    const char *str_val;

    if (typ >= 1 && typ <= 5)
    {
        // must be array of {NULL, bool, uint64_t, int64_t, double, char *}
        // must not be NULL
        // any_ptr value_of = NULL; 
        
        switch (typ)
    {
        // value type: RAW
        case YYJSON_TYPE_RAW:
            raw_val = yyjson_get_raw(root);
            INFOF("key = %s, value = %s", key_prefix, raw_val);
            // value_of = 
            type_desc_gnuc99 = "char *";
            break;

        // value type: NULL, ignore value of: NULL
        case YYJSON_TYPE_NULL:
            INFOF("key = %s, value = %s", key_prefix, "null");
            type_desc_gnuc99 = "NULL";
            break;

        // value type: BOOL
        case YYJSON_TYPE_BOOL:
            bool_val = yyjson_get_bool(root);
            INFOF("key = %s, value = %s", key_prefix, yyjson_get_type_desc(root));
            type_desc_gnuc99 = "bool";
            break;

        // value type: NUM
        case YYJSON_TYPE_NUM:
            switch (subtyp)
            {
            case YYJSON_SUBTYPE_UINT:
                u64_val = yyjson_get_uint(root);
                INFOF("key = %s, value = %lu", key_prefix, u64_val);
                type_desc_gnuc99 = "uint64_t";
                break;

            case YYJSON_SUBTYPE_SINT:
                s64_val = yyjson_get_sint(root);
                INFOF("key = %s, value = %ld", key_prefix, s64_val);
                type_desc_gnuc99 = "int64_t";
                break;

            case YYJSON_SUBTYPE_REAL:
                f64_val = yyjson_get_real(root);
                INFOF("key = %s, value = %f", key_prefix, f64_val);
                type_desc_gnuc99 = "double";
                break;

            default:
                ERRORL(UNKNOWN_JSON_SUB_TYPE);
            }
            break;

        // value_type: STR
        case YYJSON_TYPE_STR:
            str_val = yyjson_get_str(root);
            INFOF("key = %s, value = %s", key_prefix, str_val);
            type_desc_gnuc99 = "char *";
            break;
    }
        // int error_val = hashmap_get(jf->data, key_prefix, (void**)(&value_of));
        // if (error_val == MAP_MISSING) {
        //     // alloc array of value by type(typ)
        //     hashmap_put(jf->data, key_prefix, value_of);
        //     sc_array_add(&(jf->key), );
        // } else {
            
        // }
    }
    else
    {
        switch(typ)
    {
        // iterable type: ARR
        case YYJSON_TYPE_ARR:
            iter_yyjson_doc_arr(root, key_prefix, jf);
            break;
        
        // iterable type: OBJ
        case YYJSON_TYPE_OBJ:
            iter_yyjson_doc_obj(root, key_prefix, jf);
            break;

        default:
            ERRORL(UNKNOWN_JSON_TYPE);
    }
    }
    
}

void iter_yyjson_doc_arr(
    const yyjson_val *arr, 
    const char *key_prefix,  
    struct json_flatten * jf
)
{
    yyjson_val *obj;
    yyjson_arr_iter iter;
    yyjson_arr_iter_init(arr, &iter);
    while((obj = yyjson_arr_iter_next(&iter))) {
        iter_yyjson_doc_root(obj, key_prefix, jf);
    }
}

void iter_yyjson_doc_obj(
    const yyjson_val *obj, 
    const char *key_prefix,  
    struct json_flatten * jf
)
{
    yyjson_obj_iter iter;
    yyjson_obj_iter_init(obj, &iter);
    yyjson_val *key, *val;
    while ((key = yyjson_obj_iter_next(&iter))) {
        const char * key_of = yyjson_get_raw(key);
        if (key_of == NULL) {
            key_of = yyjson_get_str(key);
        }
        val = yyjson_obj_iter_get_val(key);
        
        if (strlen(key_prefix) == 0) {
            iter_yyjson_doc_root(val, key_of, jf);
            continue;
        }
        char *cat_key_of = NULL;
        STR3CAT(cat_key_of, key_prefix, ".", key_of);
        iter_yyjson_doc_root(val, cat_key_of, jf);
        free(cat_key_of);
    }
}

struct json_flatten * init_json_flatten()
{
    struct json_flatten *jf = NULL;
    MALLOC_OBJ(jf, struct json_flatten);
    sc_array_init(&(jf->key));
    jf->data = hashmap_new();
}

void free_json_flatten(struct json_flatten * jf)
{
     // free data
        // free each value of val_ptr(void*)
    hashmap_free(jf->data);

    // free key 
        // free key 4 field of buffer(char*)
    sc_array_term(&(jf->key));

    // free self
    free(jf);   
    jf = NULL;
}
