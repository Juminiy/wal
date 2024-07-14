#pragma once

#ifndef JSON_FLATTEN_H
#define JSON_FLATTEN_H

#include "../include/utils.h"

#include <yyjson.h>
#include <sc_array.h>
#include <sc_map.h>

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

    // full_path is a alloc buffer with '\0'
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
        (same address with key_rep.full_path)                     


    */

    // function: to record full_path of value_arr and value_typ
    // layout: map [key_rep.full_path] struct val_rep*
    struct sc_map_sv *map;

};


struct json_flatten * init_json_flatten(void );
void free_json_flatten(struct json_flatten * );
void iter_json_flatten(struct json_flatten *);
char* json_flatten_to_buffer(struct json_flatten *, yyjson_write_flag);

char * json_flatten_of(const char * );
void iter_json_string(const char * );
void iter_json_file(const char * );

void iter_yyjson_doc_root(yyjson_val * , const char * , struct json_flatten *);
void iter_yyjson_doc_obj(yyjson_val * , const char * , struct json_flatten *);
void iter_yyjson_doc_arr(yyjson_val * , const char * , struct json_flatten *);

char* split_dot_get_last_val(const char * );


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

#define ITER_FLATTEN_JSON_PAIR(map_key_of, map_value_of, fn_of) \
        do { \
            struct val_rep *val_of = (struct val_rep *)(map_value_of); \
            yyjson_type typ = UNSAFE_YYJSON_GET_TYPE(val_of->tag); \
            yyjson_subtype subtyp = UNSAFE_YYJSON_GET_SUBTYPE(val_of->tag); \
            const char *raw_val;    struct sc_array_str *raw_arr = NULL; \
            bool bool_val;          struct sc_array_bool *bool_arr = NULL; \
            uint64_t u64_val;       struct sc_array_64 *u64_arr = NULL; \
            int64_t s64_val;        struct sc_array_s64 *s64_arr = NULL; \
            double f64_val;         struct sc_array_double *f64_arr = NULL; \
            const char *str_val;    struct sc_array_str *str_arr = NULL; \
            switch (typ) \
            { \
            case YYJSON_TYPE_RAW: \
                raw_arr = (struct sc_array_str *)(val_of->arr); \
                sc_array_foreach(raw_arr, raw_val) { fn_of; } \
                break; \
            case YYJSON_TYPE_BOOL: \
                bool_arr = (struct sc_array_bool *)(val_of->arr); \
                sc_array_foreach(bool_arr, bool_val) { fn_of; } \
                break; \
            case YYJSON_TYPE_NUM: \
                switch (subtyp) \
                { \
                case YYJSON_SUBTYPE_UINT: \
                    u64_arr = (struct sc_array_64 *)(val_of->arr); \
                    sc_array_foreach(u64_arr, u64_val) { fn_of; } \
                    break; \
                case YYJSON_SUBTYPE_SINT: \
                    s64_arr = (struct sc_array_s64 *)(val_of->arr); \
                    sc_array_foreach(s64_arr, s64_val) { fn_of; } \
                    break; \
                case YYJSON_SUBTYPE_REAL: \
                    f64_arr = (struct sc_array_double *)(val_of->arr); \
                    sc_array_foreach(f64_arr, f64_val) { fn_of; } \
                    break; \
                } \
                break; \
            case YYJSON_TYPE_STR: \
                str_arr = (struct sc_array_str *)(val_of->arr); \
                sc_array_foreach(str_arr, str_val) { fn_of; } \
                break; \
            } \
        } while(0)


#endif//#define JSON_FLATTEN_H
