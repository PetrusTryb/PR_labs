#!/bin/bash
pkill -9 main.out
pkill -9 W\_*
gcc main.c -o main.out
if [ $? -ne 0 ]; then
	echo "Compilation failed"
	exit 1
fi
./main.out $1
