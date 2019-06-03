#include "libgc.h"

int main(int argc, char const *argv[])
{
	void * p1, * p2, * p3, * p4;
	p1 = GC_malloc(10);
	p2 = GC_malloc(10);
	p3 = GC_malloc(10);
	p4 = GC_malloc(10);
	GC_free(p1);
	GC_free(p2);
	GC_free(p3);
	GC_free(p4);
	return 0;
}
