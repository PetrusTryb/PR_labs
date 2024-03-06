#include <stdlib.h>
#include <stdio.h>
#include "hash_map.h"

hash_map *map_init(hash_t (*hash_function)(void *), int (*compare_function)(hash_t, hash_t), void (*print_function)(void *))
{
    hash_map *map = (hash_map *)calloc(1,sizeof(hash_map));
    if(map == NULL)
        return NULL;
    map->keys = NULL;
    map->hash_function = hash_function;
    map->compare_function = compare_function;
    map->print_function = print_function;
    return map;
}

void map_insert(hash_map *map, void *data)
{
    map_value *value = (map_value *)calloc(1,sizeof(map_value));
    if(value == NULL)
        return;
    value->data = data;
    value->next = NULL;
    value->prev = NULL;
    hash_t hash = map->hash_function(data);
    map_key *key = map->keys;
    while (key != NULL)
    {
        if (map->compare_function(key->hash, hash) == 0)
        {
            map_value *v = key->values;
            while (v->next != NULL)
            {
                v = v->next;
            }
            v->next = value;
            value->prev = v;
            return;
        }
        if (map->compare_function(key->hash, hash) > 0)
        {
            map_key *new_key = (map_key *)calloc(1,sizeof(map_key));
            new_key->hash = hash;
            new_key->values = value;
            new_key->next = key;
            new_key->prev = key->prev;
            if (key->prev != NULL)
            {
                key->prev->next = new_key;
            }
            else
            {
                map->keys = new_key;
            }
            key->prev = new_key;
            return;
        }
        if (key->next == NULL)
        {
            map_key *new_key = (map_key *)calloc(1,sizeof(map_key));
            new_key->hash = hash;
            new_key->values = value;
            new_key->next = NULL;
            new_key->prev = key;
            key->next = new_key;
            return;
        }
        key = key->next;
    }
    key = (map_key *)calloc(1,sizeof(map_key));
    key->hash = hash;
    key->values = value;
    key->next = NULL;
    key->prev = NULL;
    map->keys = key;
    return;
}

void* map_get(hash_map *map, hash_t hash)
{
    map_key *key = map->keys;
    while (key != NULL)
    {
        if (map->compare_function(key->hash, hash) == 0)
            return key->values->data;
        if (map->compare_function(key->hash, hash) > 0)
            return NULL;
        key = key->next;
    }
    return NULL;
}

void* map_pop(hash_map *map, hash_t hash)
{
    map_key *key = map->keys;
    while (key != NULL)
    {
        if (map->compare_function(key->hash, hash) == 0)
        {
            map_value *value = key->values;
            key->values = value->next;
            if (value->next != NULL)
                value->next->prev = NULL;
            void *data = value->data;
            free(value);
            if(key->values == NULL){
                if(key->prev != NULL)
                    key->prev->next = key->next;
                else
                    map->keys = key->next;
                if(key->next != NULL)
                    key->next->prev = key->prev;
                free(key);
            }
            return data;
        }
        if (map->compare_function(key->hash, hash) > 0)
            return NULL;
        key = key->next;
    }
    return NULL;
}

void map_print(hash_map *map)
{
    printf("{\n");
    map_key *key = map->keys;
    while (key != NULL)
    {
        map_value *value = key->values;
        printf("\t%d: [", key->hash);
        while (value != NULL)
        {
            map->print_function(value->data);
            value = value->next;
            printf(", ");
        }
        key = key->next;
        printf("],\n");
    }
    printf("}\n");
}

void map_destroy(hash_map *map)
{
    map_key *key = map->keys;
    while (key != NULL)
    {
        map_value *value = key->values;
        while (value != NULL)
        {
            map_value *v = value;
            value = value->next;
            free(v);
        }
        map_key *k = key;
        key = key->next;
        free(k);
    }
    free(map);
}
