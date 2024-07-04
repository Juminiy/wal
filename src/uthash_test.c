#include "../include/uthash_test.h"
#include "../include/utils.h"
#include "../include/log.h"

#include <stddef.h>
#include <stdbool.h>

// Not MTS
void put_stu(stu_table tbl, stu_info stu)
{   
    stu_elem elem;
    HASH_FIND_INT(tbl, &stu.id, elem);
    if (elem == NULL) {
        elem = (stu_elem)malloc(sizeof * elem);
    }
    elem->stu = stu;
    HASH_ADD_INT(tbl, stu.id, elem);
}

stu_info get_stu(stu_table tbl, int key)
{
    stu_elem elem;
    HASH_FIND_INT(tbl, &key, elem);
    if (elem){
        return elem->stu;
    }
    stu_info stu;
    return stu;
}

void del_stu(stu_table tbl, int key)
{
    stu_elem elem;
    HASH_FIND_INT(tbl, &key, elem);
    if (elem) {
        HASH_DEL(tbl, elem);
        free(elem);
    }
}

bool ok(stu_table tbl, int key)
{
    stu_elem elem;
    HASH_FIND_INT(tbl, &key, elem);
    if (elem)
        return true;
    return false;
}

int size(stu_table tbl)
{
    return HASH_COUNT(tbl);
}

int* keys(stu_table tbl)
{
    int *key_of = (int*)malloc(sizeof(int) * size(tbl));
    int index_of = 0;
    stu_elem cur, iter;
    HASH_ITER(hh, tbl, cur, iter) {
        key_of[index_of++] = cur->stu.id;
    }
    return key_of;
}

stu_info* values(stu_table tbl)
{
    stu_info *value_of = (stu_info*)malloc(sizeof(stu_info) * size(tbl));
    int index_of = 0;
    stu_elem cur, iter;
    HASH_ITER(hh, tbl, cur, iter) {
        value_of[index_of++] = cur->stu;
    }
    return value_of;
}

void del_stus(stu_table tbl)
{
    stu_elem cur, iter;
    HASH_ITER(hh, tbl, cur, iter) {
        HASH_DEL(tbl, cur);
        free(cur);
    }
}

stu_info gen_stu()
{
    stu_info stu;
    stu.id = RAND_Int(0, 1024);
    stu.age = RAND_Int(5, 24);
    stu.class_no = RAND_Int(1, 30);
    stu.grade_no = RAND_Int(0, 10);
    stu.height = RAND_F32(1, 3);
    stu.weight = RAND_F32(40, 100);
    stu.name = (char*)malloc(3 * sizeof(char)); 
    RAND_Str(stu.name, 3);
    
    return stu;
}

char *stu_str(stu_info stu)
{
    char *stu_buf = (char *)malloc(sizeof(char) * 64);
    snprintf(
        stu_buf, 
        64, 
        "id [%d], name [%s], age [%d], het [%f], wet [%f]",
        stu.id,
        stu.name,
        stu.age,
        stu.height,
        stu.weight
    );
}

void test_stu_map()
{
    stu_table stus = NULL;
    // stu_info stu;
    for (int i = 0; i < 64; i++)
    {
        put_stu(stus, gen_stu());
    }

    stu_info* stu_infos = values(*stus);
    for(int i = 0; i < 64; i++)
    {
        INFO(stu_str(stu_infos[i]));
    }

    free(stu_infos);
}