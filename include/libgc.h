#ifndef LIBGC_H
#define LIBGC_H

#include <stddef.h>

#ifdef GC_malloc
#undef GC_malloc
#endif

#define GC_malloc(...) GC_malloc_(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);

void * GC_malloc_(const char * filename, const char * function, int line, size_t size);
void GC_free(void * chunk);

#endif
