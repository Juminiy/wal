#pragma JSON_TEST_H

#ifndef JSON_TEST_H
#define JSON_TEST_H

#include "../thirdparty/yyjson/yyjson.h" 

void test_read_json_string_sample_code(void );
void test_read_json_string(void );

void test_write_json_string_sample_code(void );
void test_write_json_string(void );

void test_read_json_file_sample_code(void );
void test_read_json_file(void );

void test_write_json_file_sample_code(void );
void test_write_json_file(void );

#define UNKNOWN_JSON_TYPE "unknown of json type"
#define UNKNOWN_JSON_SUB_TYPE "unknown of json sub type"

void iter_yyjson_doc_root(const yyjson_val *root);
void iter_yyjson_doc_obj(const yyjson_val* obj);
void iter_yyjson_doc_arr(const yyjson_val* arr);
void test_iter_json_file(const char *);

#endif//#define JSON_TEST_H
