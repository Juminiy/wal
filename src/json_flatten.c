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

#define ASSIGN_HASHMAP_VALUE(value_arr, value_val, type_desc_gnuc99_desc) \
        do { \
            get_error_val = hashmap_get(jf->data, key_prefix, (any_t *)(&value_arr)); \
            sc_array_add(value_arr, value_val); \
            if (get_error_val == MAP_MISSING) { \
                put_error_val = hashmap_put(jf->data, key_prefix, value_arr); \
                struct key_rep key_of; \
                key_of.full_path = key_prefix; \
                key_of.short_path = split_dot_get_last_val(key_prefix); \
                key_of.modify_path = key_of.short_path; \
                key_of.type_desc_gnuc99 = type_desc_gnuc99_desc; \
                sc_array_add(jf->key, key_of); \
            } else { \
                hashmap_put(jf->data, key_prefix, value_arr); \
            } \
        } while(0)

#define INFOF_JSON_KV(value_fmt, value_val, value_typ) \
        INFOF( \
            "key = %s, value = %"#value_fmt", value_type = %s", \
                key_prefix, \
                value_val, \
                value_typ \
        )

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
    const char * typ_desc = yyjson_get_type_desc(root);

    const char *raw_val;    // str_arr as well
    bool bool_val;          struct sc_array_bool *bool_arr = NULL;
    uint64_t u64_val;       struct sc_array_64 *u64_arr = NULL; 
    int64_t s64_val;        struct sc_array_s64 *s64_arr = NULL;
    double f64_val;         struct sc_array_double *f64_arr = NULL;
    const char *str_val;    struct sc_array_str *str_arr = NULL;

    if (typ >= 1 && typ <= 5)
    {
        // must be array of {NULL, bool, uint64_t, int64_t, double, char *}
        // must not be NULL
        // any_ptr value_of = NULL;
        // char * type_desc_gnuc99 = NULL;
        int get_error_val = MAP_OK;
        int put_error_val = MAP_OK;
        
        switch (typ)
    {
        // value type: RAW
        case YYJSON_TYPE_RAW:
            raw_val = yyjson_get_raw(root);
            INFOF_JSON_KV(s, raw_val, typ_desc);
            MALLOC_OBJ(str_arr, struct sc_array_str);
            sc_array_init(str_arr);
            ASSIGN_HASHMAP_VALUE(str_arr, raw_val, "char *");
            break;

        // value type: NULL, ignore value of: NULL
        case YYJSON_TYPE_NULL:
            INFOF_JSON_KV(s, typ_desc, typ_desc);
            break;

        // value type: BOOL
        case YYJSON_TYPE_BOOL:
            bool_val = yyjson_get_bool(root);
            INFOF_JSON_KV(s, typ_desc, "bool");
            MALLOC_OBJ(bool_arr, struct sc_array_bool);
            sc_array_init(bool_arr); 
            ASSIGN_HASHMAP_VALUE(bool_arr, bool_val, "bool");
            break;

        // value type: NUM
        case YYJSON_TYPE_NUM:
            switch (subtyp)
            {
            case YYJSON_SUBTYPE_UINT:
                u64_val = yyjson_get_uint(root);
                INFOF_JSON_KV(lu, u64_val, "uint64_t");
                MALLOC_OBJ(u64_arr, struct sc_array_64);
                sc_array_init(u64_arr); 
                ASSIGN_HASHMAP_VALUE(u64_arr, u64_val, "uint64_t");
                break;

            case YYJSON_SUBTYPE_SINT:
                s64_val = yyjson_get_sint(root);
                INFOF_JSON_KV(lu, s64_val, "int64_t");
                MALLOC_OBJ(s64_arr, struct sc_array_s64);
                sc_array_init(s64_arr); 
                ASSIGN_HASHMAP_VALUE(s64_arr, s64_val, "int64_t");
                break;

            case YYJSON_SUBTYPE_REAL:
                f64_val = yyjson_get_real(root);
                INFOF_JSON_KV(f, s64_val, "double");
                MALLOC_OBJ(f64_arr, struct sc_array_double);
                sc_array_init(f64_arr); 
                ASSIGN_HASHMAP_VALUE(f64_arr, f64_val, "double");
                break;

            default:
                ERRORL(UNKNOWN_JSON_SUB_TYPE);
            }
            break;

        // value_type: STR
        case YYJSON_TYPE_STR:
            str_val = yyjson_get_str(root);
            INFOF_JSON_KV(s, raw_val, typ_desc);
            MALLOC_OBJ(str_arr, struct sc_array_str);
            sc_array_init(str_arr);
            ASSIGN_HASHMAP_VALUE(str_arr, raw_val, "char *");
            break;
    }
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

struct json_flatten * init_json_flatten(void )
{
    struct json_flatten *jf = NULL;
    MALLOC_OBJ(jf, struct json_flatten);
    MALLOC_OBJ(jf->key, struct sc_array_key_rep);
    sc_array_init(jf->key);
    jf->data = hashmap_new();
}

void free_json_flatten(struct json_flatten * jf)
{
     // free data
        // free each value of val_ptr(void*)
    hashmap_free(jf->data);

    // free key 
        // free key 4 field of buffer(char*)
    sc_array_term(jf->key);

    // free self
    free(jf);
}

// no copy
// get_short_path_by_full_path
// NULL
// a
// a.b
// a.b.c.d.e
char* split_dot_get_last_val(const char * full_path)
{
    return strrchr(full_path, '.') + 1;
}