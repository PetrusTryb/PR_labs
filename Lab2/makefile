default: build_static build_shared

build_static:
	gcc -c -Wall -o hash_map.o ../lab1/hash_map.c
	ar rc hash_map.a hash_map.o
	ar tv hash_map.a
	gcc -Wall -o main_static.out ../lab1/main.c hash_map.a

build_shared:
	gcc -c -Wall -fPIC -o hash_map.o ../lab1/hash_map.c
	gcc -shared -Wall -o libhash_map.so hash_map.o
	gcc -Wall -o main_shared.out ../lab1/main.c -L. -lhash_map
	ldd main_shared.out