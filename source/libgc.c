#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define RESET "\x1B[0m"
#define RED     "\033[1m\033[31m"
#define GREEN   "\033[1m\033[32m"
#define YELLOW  "\033[1m\033[33m"

#define TRUE 	1
#define FALSE 	0

typedef struct _Node Node;

struct _Node
{
	Node * prev;
	void * ptr;
	uint8_t free;
	const char * filename;
	const char * function;
	int line;
	Node * next;
};

/* Flag that enables screen logs */
uint8_t LOGS_FLAG = FALSE;


Node * first = NULL;
Node * last = NULL;

/* Run collector after main function */
void GC_collect (void) __attribute__ ((destructor));

void GC_enable_logs(void)
{
	LOGS_FLAG = TRUE;
}

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
		new->free = FALSE;
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
	new->free = FALSE;
	return new->ptr;
}

void GC_free_ (const char * filename, const char * function, int line, void * chunk)
{
	/* Remove unused warnings */
	(void) filename;
	(void) function;
	(void) line;

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

	/* Double free check */
	if(aux->free == TRUE)
	{
		if(LOGS_FLAG == TRUE)
			printf("%s%s:%d%s: Double free in '%s'.\n",RED, filename, line, RESET, function);
		return;
	}

	/*Free the only allocated chunk*/
	if(aux->prev == NULL && aux->next ==NULL)
	{
		free(aux->ptr);
		aux->free = TRUE;
	}
	else if(aux->prev == NULL) /*Free the first node*/
	{
		free(aux->ptr);
		aux->free = TRUE;
	}
	else if(aux->next == NULL) /*Free the last node*/
	{
		free(aux->ptr);
		aux->free = TRUE;
	}
	else /*Free an intermediate chunk*/
	{
		free(aux->ptr);
		aux->free = TRUE;
	}
	return;
}

void printNode(Node * node)
{
	printf("%s%s:%d%s: Unfreed memory chunck in '%s'.\n", YELLOW, node->filename, node->line, RESET, node->function);
}

void GC_collect (void)  
{ 
	Node * node, * aux;
	node = first;
	uint8_t flag = TRUE;

	/*Free all the allocated chunks*/
	while(node != NULL)
	{
		aux = node->next;
		if(node->free == FALSE)
		{
			if(LOGS_FLAG == TRUE)
				printNode(node);
			free(node->ptr);
			flag = FALSE;
		}
		free(node);
		node = aux;
	}
	if(LOGS_FLAG == TRUE && flag == TRUE)
		printf("%sOK%s: All memory chuncks freed.\n", GREEN, RESET);

	return;
} 
