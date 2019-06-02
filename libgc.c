#include <stdio.h>
#include <stdlib.h>
#include "libgc.h"

struct _Node
{
	Node * prev;
	void * ptr;
	Node * next;
};

Node * first = NULL;
Node * last = NULL;

void * GC_malloc(size_t size)
{
	Node * new;
	/*Node memory allocation*/
	new = (Node*) malloc(sizeof(Node));
	if(new == NULL)
		return NULL;

	/*Memory chunk allocation*/
	new->ptr = malloc(size);
	if(new->ptr == NULL)
	{
		free(new);
		return NULL;
	}

	/*First memory chunk*/
	if(last == NULL)
	{
		first = last = new;
		new->prev = new->next = NULL;
		return new->ptr;
	}

	/*Not first memory chunk*/
	last->next = new;
	new->prev = last;
	new->next = NULL;
	last = new;
	return new->ptr;
}

void GC_free(void * chunk)
{
	
}