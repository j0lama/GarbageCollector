#include <stdio.h>
#include <stdlib.h>

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"

typedef struct _Node Node;

struct _Node
{
	Node * prev;
	void * ptr;
	const char * filename;
	const char * function;
	int line;
	Node * next;
};

void GC_collect (void) __attribute__ ((destructor));

Node * first = NULL;
Node * last = NULL;

void * GC_malloc_ (const char * filename, const char * function, int line, size_t size)
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
		new->filename = filename;
		new->function = function;
		new->line = line;
		return new->ptr;
	}

	/*Not first memory chunk*/
	last->next = new;
	new->prev = last;
	new->next = NULL;
	last = new;
	new->filename = filename;
	new->function = function;
	new->line = line;
	return new->ptr;
}

void GC_free (void * chunk)
{
	Node * aux;
	if(chunk == NULL)
		return;

	aux = first;
	/*There is no allocated chunks*/
	if(aux == NULL)
		return;

	/*Search the chuck*/
	while(aux->next != NULL && aux->ptr != chunk)
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

void printNode(Node * node)
{
	printf("%s%s:%d%s: Unfreed memory chunck in '%s'.\n", RED, node->filename, node->line, RESET, node->function);
}

void GC_collect (void)  
{ 
    Node * node, * aux;
    node = first;
    /*Free all the allocated chunks*/
    while(node != NULL)
    {
    	printNode(node);
    	aux = node->next;
    	free(node->ptr);
    	free(node);
    	node = aux;
    }
    return;
} 
