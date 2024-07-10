#pragma JSON_FLATTEN_H

#ifndef JSON_FLATTEN_H
#define JSON_FLATTEN_H

#include "../thirdparty/yyjson/yyjson.h" 
#include "../thirdparty/sc/array/sc_array.h"
#include "../thirdparty/sc/map/sc_map.h"

#define UNKNOWN_JSON_TYPE "unknown of json type"
#define UNKNOWN_JSON_SUB_TYPE "unknown of json sub type"

#define GNUC99_TYPE_DESC_STR "char *"
#define GNUC99_TYPE_DESC_NULL "NULL"
#define GNUC99_TYPE_DESC_BOOL "bool"
#define GNUC99_TYPE_DESC_U64 "uint64_t"
#define GNUC99_TYPE_DESC_S64 "int64_t"
#define GUNC99_TYPE_DESC_F64 "double"

#define JSON_FLATTEN_SPLIT_STR "."
#define JSON_FLATTEN_SPLIT_CHR '.'

/*
layout: struct key_rep 32B
    -----------------------------------------------------------
    | full_path | short_path | modify_path | type_desc_gnuc99 |
    -----------------------------------------------------------
         |            |             |               |
        \ /          \ /            |               |
       | full_path_buffer '\0'|     |               |
                                    |               ------> | CONST_VALUE '\0'|
                                    ---> | NULL |
*/
struct key_rep {

    // full_path is a malloc buffer with '\0'
    // example: a.b.c.d.e.f
    char * full_path;

    // short_path shares same location with full_path
    // example: f
    char * short_path;

    // modify_path is NULL in C mem
    // example: f_of
    char * modify_path;

    // type_desc_gnuc99 is const value in static location
    // "raw", "string" : "char*", 
    // "null": "NULL", 
    // "true", "false": "bool", 
    // "uint": "uint64_t", 
    // "sint": "int64_t", 
    // "real": "double"
    char * type_desc_gnuc99;

};

sc_array_def(struct key_rep, key_rep);
sc_array_def(int64_t, s64);
sc_array_def(bool, bool);

/*
layout: struct val_rep 16B
*/
struct val_rep {
    uint64_t tag;
    any_ptr arr;
};

struct json_flatten {

    // function: to keep order of original key_rep 
    // layout: header with array {key_rep(32B char*), ...}
    struct sc_array_key_rep *key;


    /*
        layout: json_flatten.map
          8B                        8B
        -----                    -------
        |key| -----------------> |value|
        -----                    -------
          |                         |
          |                         |        -----------------        ---------------------
         \ /                        -------> |tag: 8B|arr: 8B| -----> | | | | | | | | | | | ...
        | full_path_buffer '\0'|             -----------------        ---------------------
                                            


    */

    // function: to record full_path of value_arr and value_typ
    // layout: map [key_rep.full_path] struct val_rep*
    struct sc_map_sv *map;

};


struct json_flatten * init_json_flatten(void );
void free_json_flatten(struct json_flatten * );

void iter_yyjson_doc_root(const yyjson_val * , const char * , struct json_flatten *);
void iter_yyjson_doc_obj(const yyjson_val * , const char * , struct json_flatten *);
void iter_yyjson_doc_arr(const yyjson_val * , const char * , struct json_flatten *);

void iter_json_string(const char * );
void iter_json_file(const char * );

char* split_dot_get_last_val(const char * );
void iter_json_flatten(struct json_flatten *);

#define UNSAFE_YYJSON_GET_TYPE(tag) \
        (yyjson_type)((uint8_t)(tag) & YYJSON_TYPE_MASK)

#define UNSAFE_YYJSON_GET_SUBTYPE(tag) \
        (yyjson_subtype)((uint8_t)(tag) & YYJSON_SUBTYPE_MASK)

#define SC_MAP_LAST_PUT_OVERRIDE sc_map_found
#define SC_MAP_LAST_GET_FOUND sc_map_found
#define SC_MAP_LAST_DEL_SUCCESS sc_map_found

#define ASSIGN_HASHMAP_VALUE(value_arr, value_arr_typ, value_val, value_val_typ_desc) \
        do { \
            any_ptr get_value_rep = sc_map_get_sv(jf->map, key_prefix); \
            struct val_rep* value_rep; \
            if(SC_MAP_LAST_GET_FOUND(jf->map)) { \
                value_rep = (struct val_rep *)(get_value_rep); \
                value_arr = (value_arr_typ *)(value_rep->arr); \
            } else { \
                MALLOC_OBJ(value_rep, struct val_rep); \
                value_rep->tag = yyjson_get_tag(root); \
                MALLOC_OBJ(value_arr, value_arr_typ); sc_array_init(value_arr); \
                value_rep->arr = value_arr; \
                struct key_rep key_of; \
                key_of.full_path = key_prefix; \
                key_of.short_path = split_dot_get_last_val(key_prefix); \
                key_of.modify_path = NULL; \
                key_of.type_desc_gnuc99 = value_val_typ_desc; \
                sc_array_add(jf->key, key_of); \
            } \
            sc_array_add(value_arr, value_val); \
            sc_map_put_sv(jf->map, key_prefix, value_rep); \
        } while(0)

#define INFOF_JSON_KV(value_fmt, value_val, value_typ) \
        INFOF( \
            "key = %s, value = %"#value_fmt", value_type = %s", \
                key_prefix, \
                value_val, \
                value_typ \
        )

#endif//#define JSON_FLATTEN_H
