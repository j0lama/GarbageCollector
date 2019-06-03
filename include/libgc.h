#ifndef LIBGC_H
#define LIBGC_H

#include <stddef.h>

typedef struct _Node Node;

void * GC_malloc(size_t size);
void GC_free(void * chunk);

#endif
