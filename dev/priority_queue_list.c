#include <stdio.h>
#include <stdlib.h>
#include "priority_queue_list.h"

void qlist(pqueue *head, void (*print_data)(void *)) {
	pqueue *p;
	
	for (p = head; p != NULL; p = p->next) {
		printf("%d: ", p->k);
		print_data(p->data);
		printf("\n");
	}
	
}

void qinsert(pqueue **phead, void *data, int k) {
	if(*phead==NULL){
		*phead = (struct pqueue * )malloc(sizeof(struct pqueue));
		(*phead)->data = data;
		(*phead)->next = NULL;
		(*phead)->prev = NULL;
		(*phead)->k = k;
	}
	else{
		if(k>(*phead)->k){
			qinsert(&(*phead)->next,data,k);
		}
		else{
			(*phead)->prev = (struct pqueue * )malloc(sizeof(struct pqueue));
			(*phead)->prev->data = data;
			(*phead)->prev->k=k;
			(*phead)->prev->next=(*phead);
			(*phead)->prev->prev=NULL;
			*phead=(*phead)->prev;
		}
	}
}


void qremove(pqueue **phead, int k) {
	pqueue *p = *phead;
	while(p->k!=k){
		if(p->k<k){
			if(p->next==NULL)
				return;
			p=p->next;
		}
		else{
			if(p->prev==NULL)
				return;
			p=p->prev;
		}
	}
	if((*p).next!=NULL){
		(*p).next->prev = p->prev;
		if((*p).prev!=NULL)
			(*p).prev->next=p->next;
		(*p)=(*p->next);
	}
	else if((*p).prev!=NULL){
		(*p).prev->next = p->next;
		if((*p).next!=NULL)
			(*p).next->prev=p->prev;
		(*p)=(*p->prev);
	}
}

