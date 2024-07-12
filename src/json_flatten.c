// #include "../include/log.h"
#include "../include/utils.h"

#include "../include/json_flatten.h"

char * json_flatten_of(const char * old)
{
    yyjson_doc *doc = yyjson_read(old, strlen(old), YYJSON_READ_NOFLAG);
    if (!doc) {
        return NULL;
    }

    yyjson_val *root = yyjson_doc_get_root(doc);

    
    struct json_flatten * jf = init_json_flatten();
    iter_yyjson_doc_root(root, "", jf);
    
    char * jstr = json_flatten_to_buffer(jf, YYJSON_WRITE_NOFLAG);

    free_json_flatten(jf);
    yyjson_doc_free(doc);

    return jstr;
}

void iter_json_string(const char * json_str)
{

}

void iter_json_file(const char * file_name)
{
    yyjson_read_flag flg = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS;
    yyjson_read_err err;
    yyjson_doc *doc = yyjson_read_file(file_name, flg, NULL, &err);

    if(!doc){
        // ERRORF("read error (%u): %s at position: %lu", err.code, err.msg, err.pos);
        return;
    }
    
    yyjson_val *root = yyjson_doc_get_root(doc);
    struct json_flatten * jf = init_json_flatten();
    iter_yyjson_doc_root(root, "", jf);
    
    // iter_json_flatten(jf);
    char * jstr = json_flatten_to_buffer(jf, YYJSON_WRITE_NOFLAG);
    if(jstr) {
        // INFO_NL(jstr);
        free(jstr);
    }

    free_json_flatten(jf);
    yyjson_doc_free(doc);

}  

void iter_yyjson_doc_root(
    yyjson_val *root, 
    const char *key_prefix, 
    struct json_flatten * jf
)
{
    if(root == NULL)
        return;
    
    yyjson_type typ = yyjson_get_type(root);
    yyjson_subtype subtyp = yyjson_get_subtype(root);
    // const char * typ_desc = yyjson_get_type_desc(root);

    const char *raw_val = NULL;         struct sc_array_str *raw_arr    = NULL;
    bool bool_val = false;              struct sc_array_bool *bool_arr  = NULL;
    uint64_t u64_val = 0;               struct sc_array_64 *u64_arr     = NULL;
    int64_t s64_val = 0;                struct sc_array_s64 *s64_arr    = NULL;
    double f64_val = 0.0;               struct sc_array_double *f64_arr = NULL;
    const char *str_val = NULL;         struct sc_array_str *str_arr    = NULL;

    if (typ >= 1 && typ <= 5)
    {
        // must be array of {bool, uint64_t, int64_t, double, char *}
        // must not be NULL
        // any_ptr value_of = NULL;
        // char * type_desc_gnuc99 = NULL;
        // int get_error_val = MAP_OK;
        // int put_error_val = MAP_OK;
        
        switch (typ)
    {
        // value type: RAW
        case YYJSON_TYPE_RAW:
            raw_val = yyjson_get_raw(root);
            ASSIGN_HASHMAP_VALUE(raw_arr, struct sc_array_str, raw_val, GNUC99_TYPE_DESC_STR);
            break;

        // value type: NULL, ignore value of: NULL
        case YYJSON_TYPE_NULL:
            break;

        // value type: BOOL
        case YYJSON_TYPE_BOOL:
            bool_val = yyjson_get_bool(root);
            ASSIGN_HASHMAP_VALUE(bool_arr, struct sc_array_bool, bool_val, GNUC99_TYPE_DESC_BOOL);
            break;

        // value type: NUM
        case YYJSON_TYPE_NUM:
            switch (subtyp)
            {
            // value subtype: UINT
            case YYJSON_SUBTYPE_UINT:
                u64_val = yyjson_get_uint(root);
                ASSIGN_HASHMAP_VALUE(u64_arr, struct sc_array_64, u64_val, GNUC99_TYPE_DESC_U64);
                break;

            // value subtype: SINT
            case YYJSON_SUBTYPE_SINT:
                s64_val = yyjson_get_sint(root);
                ASSIGN_HASHMAP_VALUE(s64_arr, struct sc_array_s64, s64_val, GNUC99_TYPE_DESC_S64);
                break;

            // value subtype: REAL
            case YYJSON_SUBTYPE_REAL:
                f64_val = yyjson_get_real(root);
                ASSIGN_HASHMAP_VALUE(f64_arr, struct sc_array_double, f64_val, GUNC99_TYPE_DESC_F64);
                break;

            default:
                // ERRORL(UNKNOWN_JSON_SUB_TYPE);
            }
            break;

        // value_type: STR
        case YYJSON_TYPE_STR:
            str_val = yyjson_get_str(root);
            ASSIGN_HASHMAP_VALUE(str_arr, struct sc_array_str, str_val, GNUC99_TYPE_DESC_STR);
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
            // ERRORL(UNKNOWN_JSON_TYPE);
    }
    }
    
}

void iter_yyjson_doc_arr(
    yyjson_val *arr, 
    const char *key_prefix,  
    struct json_flatten * jf
)
{
    yyjson_val *obj = NULL;
    yyjson_arr_iter iter;
    yyjson_arr_iter_init(arr, &iter);
    while((obj = yyjson_arr_iter_next(&iter))) {
        iter_yyjson_doc_root(obj, key_prefix, jf);
    }
}

void iter_yyjson_doc_obj(
    yyjson_val *obj, 
    const char *key_prefix,  
    struct json_flatten * jf
)
{
    yyjson_obj_iter iter;
    yyjson_obj_iter_init(obj, &iter);
    yyjson_val *key = NULL, *val = NULL;
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
        STR3CAT(cat_key_of, key_prefix, JSON_FLATTEN_SPLIT_STR, key_of);
        iter_yyjson_doc_root(val, cat_key_of, jf);
        // free(cat_key_of); // sure to cause error but need to free not alc
    }
}

struct json_flatten * init_json_flatten(void )
{
    struct json_flatten *jf = NULL;
    MALLOC_OBJ(jf, struct json_flatten);
    MALLOC_OBJ(jf->key, struct sc_array_key_rep); sc_array_init(jf->key);
    MALLOC_OBJ(jf->map, struct sc_map_sv); sc_map_init_sv(jf->map, 0, 0);
    return jf;
}

void free_json_flatten(struct json_flatten * jf)
{
     // free map
        // do not free key
        // free each key map to val_rep(void*)
            // tag is uint64_t cannot free 
            // free val_rep.arr(void*)
    sc_map_term_sv(jf->map);

    // free key 
        // free key 4 field of buffer(char*)
    sc_array_term(jf->key);

    // free self
    free(jf);
}

// no copy
// get_short_path_by_full_path
// NULL      -> NULL
// a         -> a
// a.b       -> b
// a.b.c.d.e -> e
char* split_dot_get_last_val(const char * full_path)
{
    if(full_path == NULL)
        return NULL;
    char * last_dot = strrchr(full_path, JSON_FLATTEN_SPLIT_CHR);
    if (last_dot == NULL) {
        return full_path;
    }
    return last_dot + 1;
}

void iter_json_flatten(struct json_flatten * jf)
{
    struct key_rep key_rep_of;
    char * map_key_of;
    any_ptr map_value_of;

    uint64_t tot_key_cnt_of = 0;
    uint64_t tot_val_cnt_of = 0;
    sc_array_foreach(jf->key, key_rep_of) {
        // INFOF_NL("key : %s", key_rep_of.short_path);
        ++ tot_key_cnt_of;
        map_key_of = key_rep_of.full_path;
        map_value_of = sc_map_get_sv(jf->map, map_key_of);
        if(SC_MAP_LAST_GET_FOUND(jf->map))
            ITER_FLATTEN_JSON_PAIR(
                map_key_of, map_value_of, 
                {
                    ++ tot_val_cnt_of;
                }
            );
    }

    // INFOF_NL("key_cnt: %lu, val_cnt: %lu", tot_key_cnt_of, tot_val_cnt_of);
}

char* json_flatten_to_buffer(struct json_flatten * jf, yyjson_write_flag flg)
{

    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);
    yyjson_mut_val *data = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_val(doc, root, "data", data);
    yyjson_mut_val *key = yyjson_mut_arr(doc);
    yyjson_mut_obj_add_val(doc, root, "key", key);


    // write json_flatten to yyjson_mut_doc
    struct key_rep key_rep_of;
    char * map_key_of               = NULL;
    any_ptr map_value_of            = NULL;
    struct val_rep *val_of          = NULL;
    struct sc_array_str *raw_arr    = NULL;
    struct sc_array_bool *bool_arr  = NULL;
    struct sc_array_64 *u64_arr     = NULL;
    struct sc_array_s64 *s64_arr    = NULL;
    struct sc_array_double *f64_arr = NULL;
    struct sc_array_str *str_arr    = NULL;

    sc_array_foreach(jf->key, key_rep_of) {

        map_key_of = key_rep_of.full_path;
        map_value_of = sc_map_get_sv(jf->map, map_key_of); // be sure to get ok

        val_of = (struct val_rep *)(map_value_of);
        yyjson_type typ = UNSAFE_YYJSON_GET_TYPE(val_of->tag);
        yyjson_subtype subtyp = UNSAFE_YYJSON_GET_SUBTYPE(val_of->tag);
        
        // write key
        yyjson_mut_val *key_rep_elem = yyjson_mut_obj(doc);
        yyjson_mut_arr_append(key, key_rep_elem);
        yyjson_mut_obj_add_str(doc, key_rep_elem, "reName", "");
        yyjson_mut_obj_add_str(doc, key_rep_elem, "fullPath", key_rep_of.full_path); 
        yyjson_mut_obj_add_str(doc, key_rep_elem, "name", key_rep_of.short_path);

        // write value
        yyjson_mut_val *val_rep_arr = NULL;
        switch (typ)
        {
        case YYJSON_TYPE_RAW:
            raw_arr = (struct sc_array_str *)(val_of->arr);
            val_rep_arr = yyjson_mut_arr_with_str(doc, raw_arr->elems, raw_arr->size);
            break;

        case YYJSON_TYPE_BOOL:
            bool_arr = (struct sc_array_bool *)(val_of->arr);
            val_rep_arr = yyjson_mut_arr_with_bool(doc, bool_arr->elems, bool_arr->size);
            break;

        case YYJSON_TYPE_NUM:
            switch (subtyp)
            {
            case YYJSON_SUBTYPE_UINT:
                u64_arr = (struct sc_array_64 *)(val_of->arr);
                val_rep_arr = yyjson_mut_arr_with_uint64(doc, u64_arr->elems, u64_arr->size);
                break;

            case YYJSON_SUBTYPE_SINT:
                s64_arr = (struct sc_array_s64 *)(val_of->arr);
                val_rep_arr = yyjson_mut_arr_with_sint64(doc, s64_arr->elems, s64_arr->size);
                break;

            case YYJSON_SUBTYPE_REAL:
                f64_arr = (struct sc_array_double *)(val_of->arr);
                val_rep_arr = yyjson_mut_arr_with_double(doc, f64_arr->elems, f64_arr->size);
                break;

            }
            break;

        case YYJSON_TYPE_STR:
            str_arr = (struct sc_array_str *)(val_of->arr);
            val_rep_arr = yyjson_mut_arr_with_str(doc, str_arr->elems, str_arr->size);
            break;

        }
        yyjson_mut_obj_add_val(doc, data, map_key_of, val_rep_arr);
    }


    // write yyjson_mut_doc to buffer(char*)
    char * jstr = yyjson_mut_write(doc, flg, NULL);

    // free yyjson_mut_doc
    yyjson_mut_doc_free(doc);

    // return buffer
    return jstr;
}

// void assign_hashmap_value_func_debug_s64
// (
//     struct sc_array_s64 *value_arr, 
//     int64_t value_val, 
//     const char * key_prefix,
//     struct json_flatten * jf
// )
// {
//     any_ptr get_value_arr = sc_map_get_sv(jf->map, key_prefix);
//     if(SC_MAP_LAST_GET_FOUND(jf->map)) {
//         value_arr = (struct sc_array_s64 *)(get_value_arr);
//     } else {
//         MALLOC_OBJ(value_arr, struct sc_array_s64); sc_array_init(value_arr);

//         struct key_rep key_of;
//         key_of.full_path = key_prefix;
//         key_of.short_path = split_dot_get_last_val(key_prefix);
//         key_of.modify_path = key_of.short_path;
//         key_of.type_desc_gnuc99 = GNUC99_TYPE_DESC_S64;
//         sc_array_add(jf->key, key_of);
//     }
//     sc_array_add(value_arr, value_val);
//     sc_map_put_sv(jf->map, key_prefix, value_arr);
// }