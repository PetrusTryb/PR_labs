#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "priority_queue_list.h"

typedef struct process process;
struct process {
	char name[40];
};

void print_process(void *data) {
	process *pr = (process *)data;
	printf("%s", pr->name);
}

int main(int argc, char **argv) {
	
	pqueue *queue = NULL;
	process *p1, *p2, *p3;
	p1 = (process *) malloc(sizeof(process));
	strcpy(p1->name, "Firefox");
	p2 = (process *) malloc(sizeof(process));
	strcpy(p2->name, "Geany IDE");
	p3 = (process *) malloc(sizeof(process));
	strcpy(p3->name, "Important Calculations");

	qinsert(&queue, p1, 5);
	qinsert(&queue, p2, 2);
	qinsert(&queue, p3, 10);
	qlist(queue, print_process);//2,5,10
	
	printf("\n");
	qremove(&queue, 2);
	qlist(queue, print_process);//5,10
	
	printf("\n");
	qinsert(&queue, p2, 11);
	qlist(queue, print_process);//5,10,11
	
	printf("\n");
	qremove(&queue, 5);
	qlist(queue, print_process);//10,11
	
	printf("\n");
	qremove(&queue, 11);
	qlist(queue, print_process);//10
	
	free(p1);
	free(p2);
	free(p3);	
		
	return 0;
}
