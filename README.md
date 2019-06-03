# *libgc* - A Garbage Collector utility for C
Garbage Collector written in C that frees all the allocated memory automatically at the end of the execution thanks to the *destructor* attribute associate to the collect function.

The *destructor* attribute allows to execute the collect function after the main return collecting and freeing all the allocated memory.

**libgc** has the following interface:

- ***void * GC_malloc(size_t size)***: Returns a pointer to the allocated memory chunk of size *size*.

- ***void GC_free(void * chunk)***: Frees the chunk passing in the *chunk* argument.

### Compilation and Installation

To compile and install **libgc** use make:
```bash
make
sudo make install
```

### Use

This is an example of an application that use the interface provided by **libgc** to alloc memory chunks that its freed after the main execution:

```c
#include <libgc.h>

int main(int argc, char const *argv[])
{
	void * p1 = GC_malloc(20);
	void * p2 = GC_malloc(30);
	return 0;
}
```

To compile the previous example use the *-lgc* parameter as is shown below:
```bash
gcc main.c -o main -lgc
```

#### Instalation problems
If your compiler is not able to find **libgc** with the flag *-lgc* is because the linker hasnt got the path to the library cached. To solve the problem execute the following sentence:
```bash
sudo ldconfig
```

#### Execution example
The execution show below corresponds with a execution under Valgrind that shows at the end the possible memory leaks.
```bash
$ valgrind ./main 
==15677== Memcheck, a memory error detector
==15677== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==15677== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==15677== Command: ./main
==15677== 
==15677== 
==15677== HEAP SUMMARY:
==15677==     in use at exit: 0 bytes in 0 blocks
==15677==   total heap usage: 4 allocs, 4 frees, 98 bytes allocated
==15677== 
==15677== All heap blocks were freed -- no leaks are possible
==15677== 
==15677== For counts of detected and suppressed errors, rerun with: -v
==15677== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
