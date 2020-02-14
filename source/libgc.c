#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <execinfo.h>


#define RESET "\x1B[0m"
#define RED     "\033[1m\033[31m"
#define GREEN   "\033[1m\033[32m"
#define YELLOW  "\033[1m\033[33m"


#define BACKTRACE_DEPTH	10

typedef struct _Node Node;

struct _Node
{
	Node * prev;
	void * ptr;
	int isfree;
	const char * filename;
	const char * function;
	char ** backtraceDump;
	size_t size;
	int line;
	Node * next;
};

/* Flag that enables screen logs */
uint8_t LOGS_FLAG = 0;

/* Flag that enables backtrace */
uint8_t BACKTRACE_FLAG = 0;


Node * first = NULL;
Node * last = NULL;

/* Run collector after main function */
void GC_collect (void) __attribute__ ((destructor));

void GC_enable_logs(void)
{
	LOGS_FLAG = 1;
}

void GC_enable_backtrace(void)
{
	BACKTRACE_FLAG = 1;
}

void * GC_malloc_ (const char * filename, const char * function, int line, size_t size)
{
	Node * new;
	void * array[BACKTRACE_DEPTH];

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
		new->isfree = 0;
		if(BACKTRACE_FLAG == 1)
		{
			new->size = backtrace (array, BACKTRACE_DEPTH);
			new->backtraceDump = backtrace_symbols (array, new->size);
		}
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
	new->isfree = 0;
	if(BACKTRACE_FLAG ==1)
	{
		new->size = backtrace (array, BACKTRACE_DEPTH);
		new->backtraceDump = backtrace_symbols (array, new->size);
	}
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

	/*Search the chunck*/
	while(aux != NULL && aux->ptr != chunk)
		aux = aux->next;

	/*Invalid chunk pointer*/
	if(aux == NULL || aux->ptr != chunk)
		return;

	/* Double free check */
	if(aux->isfree == 1)
	{
		if(LOGS_FLAG == 1)
			printf("%s%s:%d%s: Double free in '%s'.\n",RED, filename, line, RESET, function);
		return;
	}

	/*Free the only allocated chunk*/
	if(aux->prev == NULL && aux->next ==NULL)
	{
		free(aux->backtraceDump);
		aux->isfree = 1;
	}
	else if(aux->prev == NULL) /*Free the first node*/
	{
		free(aux->backtraceDump);
		aux->isfree = 1;
	}
	else if(aux->next == NULL) /*Free the last node*/
	{
		free(aux->backtraceDump);
		aux->isfree = 1;
	}
	else /*Free an intermediate chunk*/
	{
		free(aux->backtraceDump);
		aux->isfree = 1;
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
	uint8_t flag = 1;
	int count = 0;

	/*Free all the allocated chunks*/
	while(node != NULL)
	{
		aux = node->next;
		if(node->isfree == 0)
		{
			if(LOGS_FLAG == 1)
				printNode(node);
			if(BACKTRACE_FLAG == 1)
			{
				for (uint32_t i = 0; i < node->size; i++)
					printf("[BT] %d: %s\n", i, node->backtraceDump[i]);
				free(node->backtraceDump);
			}
			flag = 0;
			count++;
		}
		free(node->ptr);
		free(node);
		node = aux;
	}
	if(LOGS_FLAG == 1 && flag == 1)
		printf("%sOK%s: All memory chuncks freed.\n", GREEN, RESET);
	else if(LOGS_FLAG == 1 && flag == 0)
		printf("\n%sWARNING%s: %d unfreed memory chuncks.\n", YELLOW, RESET, count);

	return;
} 
