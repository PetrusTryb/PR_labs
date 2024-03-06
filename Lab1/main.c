#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash_map.h"

typedef struct pg_student pg_student;
struct pg_student {
    char name[16];
    int semester;
    int ects_debt;
};

hash_t f_hash(void* x) {
    pg_student* s = (pg_student*)x;
    hash_t h = 0;
    for (int i = 0; i < 16; i++) {
        h += s->name[i] * 10;
    }
    h+=s->semester*1000;
    return h;
}

int compare(hash_t hash_a, hash_t hash_b) {
    if (hash_a > hash_b)
        return 1;
    if (hash_a < hash_b)
        return -1;
    return 0;
}

void print(void* data) {
    pg_student* s = (pg_student*)data;
    printf("{name: %s, sem: %d, debt: %d}", s->name, s->semester, s->ects_debt);
}

int main()
{
    hash_map* map = map_init(f_hash, compare, print);
    pg_student* s1, * s2, * s3;
    s1 = (pg_student*)calloc(1,sizeof(pg_student));
    s2 = (pg_student*)calloc(1,sizeof(pg_student));
    s3 = (pg_student*)calloc(1,sizeof(pg_student));
    if(map == NULL || s1 == NULL || s2 == NULL || s3 == NULL)
        return 1;
    strcpy(s1->name, "Trybisz");
    strcpy(s2->name, "Nowak");
    strcpy(s3->name, "Trybisz");
    s1->semester = 1;
    s2->semester = 1;
    s3->semester = 1;
    s1->ects_debt = 20;
    s2->ects_debt = 30;
    s3->ects_debt = 5;
    map_print(map);
    map_insert(map, s1);
    print(map_get(map, f_hash(s1)));
    printf("\n");
    map_print(map);
    map_insert(map, s2);
    map_print(map);
    map_insert(map, s3);
    map_print(map);
    print(map_pop(map, f_hash(s1)));
    printf("\n");
    map_print(map);
    map_insert(map, s1);
    map_insert(map, s2);
    map_insert(map, s3);
    map_print(map);
    map_destroy(map);
    free(s1);
    free(s2);
    free(s3);
    return 0;
}
