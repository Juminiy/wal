#include "../thirdparty/uthash/uthash.h"

#include <stddef.h>
#include <stdbool.h>

struct stu_info {
    int id;
    int age;
    char * name;
    float height;
    float weight;
    int grade_no;
    int class_no;
    bool sex;
};

struct stu_info_hh_wrap {
    struct stu_info stu;
    UT_hash_handle hh;
};

typedef struct stu_info stu_info;
typedef struct stu_info_hh_wrap* stu_table;
typedef struct stu_info_hh_wrap* stu_elem;
typedef int (*comp_fn)(const stu_elem, const stu_elem);

void put_stu(stu_table , stu_info );
stu_info get_stu(stu_table , int );
void del_stu(stu_table , int );
bool ok(stu_table , int );
int size(stu_table );
int* keys(stu_table );
stu_info* values(stu_table );
void del_stus(stu_table );

void test_stu_map();