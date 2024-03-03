typedef unsigned int hash_t;

typedef struct map_value map_value;
struct map_value{
    void* data;
    map_value* next;
    map_value* prev;
};

typedef struct map_key map_key;
struct map_key{
    hash_t hash;
    map_value* values;
    map_key* next;
    map_key* prev;
};

typedef struct hash_map hash_map;
struct hash_map{
    map_key* keys;
    hash_t(*hash_function)(void*);
    int(*compare_function)(hash_t, hash_t);
	void(*print_function)(void*);
};

hash_map* map_init(hash_t(*hash_function)(void*), int(*compare_function)(hash_t, hash_t), void(*print_function)(void*));
void map_insert(hash_map* map, void* data);
void* map_get(hash_map* map, hash_t hash);
void* map_pop(hash_map* map, hash_t hash);
void map_print(hash_map* map);
void map_destroy(hash_map* map);