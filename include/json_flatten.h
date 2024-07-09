#pragma JSON_FLATTEN_H

#ifndef JSON_FLATTEN_H
#define JSON_FLATTEN_H

#include "../thirdparty/yyjson/yyjson.h" 
#include "../thirdparty/c_hashmap/hashmap.h"
#include "../thirdparty/sc/array/sc_array.h"

#define UNKNOWN_JSON_TYPE "unknown of json type"
#define UNKNOWN_JSON_SUB_TYPE "unknown of json sub type"

struct key_rep {
    // example: a.b.c.d.e.f
    char * full_path;
    // example: f
    char * short_path;
    // example: f_of
    char * modify_path;
    // "raw", "string" : "char*", 
    // "null": "NULL", 
    // "true", "false": "bool", 
    // "uint": "uint64_t", 
    // "sint": "int64_t", 
    // "real": "double"
    char * type_desc_gnuc99;
};

sc_array_def(struct key_rep, key_rep);

struct json_flatten {
    // {key_rep(32B char*), ...}
    struct sc_array_key_rep key;
    // map [key_rep.full_path] any_ptr
    // any_ptr is sc_array_type
    map_t data;                  
};


struct json_flatten * init_json_flatten();
void free_json_flatten(struct json_flatten * );

void iter_yyjson_doc_root(const yyjson_val * , const char * , struct json_flatten *);
void iter_yyjson_doc_obj(const yyjson_val * , const char * , struct json_flatten *);
void iter_yyjson_doc_arr(const yyjson_val * , const char * , struct json_flatten *);

void iter_json_string(const char * );
void iter_json_file(const char * );


#endif//#define JSON_FLATTEN_H
