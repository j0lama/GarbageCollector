#ifndef LIBGC_H
#define LIBGC_H

#include <stddef.h>

#ifdef GC_malloc
#undef GC_malloc
#endif

#ifdef GC_free
#undef GC_free
#endif

#define GC_malloc(...) GC_malloc_(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);
#define GC_free(...) GC_free_(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);

void GC_enable_logs(void);
void * GC_malloc_(const char * filename, const char * function, int line, size_t size);
void GC_free_ (const char * filename, const char * function, int line, void * chunk);

#endif
