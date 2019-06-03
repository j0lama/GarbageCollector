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
	Node * aux;
	if(chunk == NULL)
		return;

	aux = first;
	/*There is no allocated chunks*/
	if(aux == NULL)
		return;

	/*Search the chuck*/
	while(aux->next != NULL || aux->ptr != chunk)
		aux = aux->next;

	/*Invalid chunk pointer*/
	if(aux->ptr != chunk)
		return;

	/*Free the only allocated chunk*/
	if(aux->prev == NULL && aux->next ==NULL)
	{
		free(aux->ptr);
		free(aux);
		first = last = NULL;
	}
	else if(aux->prev == NULL) /*Free the first node*/
	{
		first = aux->next;
		first->prev = NULL;
		free(aux->ptr);
		free(aux);
	}
	else if(aux->next == NULL) /*Free the last node*/
	{
		last = aux->prev;
		last->next = NULL;
		free(aux->ptr);
		free(aux);
	}
	else /*Free an intermediate chunk*/
	{
		(aux->prev)->next = aux->next;
		(aux->next)->prev = aux->prev;
		free(aux->ptr);
		free(aux);
	}
	return;
}
