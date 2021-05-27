/**
 * mem.c - Memory (de)allocation accounting library implementation
 *
 * The memory (de)allocation accounting library keeps track of all allocations
 * performed via malloc, and all free operations on allocated memory. It provides
 * a mem_report function that returns the current number of allocated blocks, and
 * optionally invokes a user-supplied function pointer that can print the details
 * if desired.
 *
 * The report_func parameter is used in order to keep the library platform
 * independent, as stdio functions like printf may not be available in all C
 * library implementations. This library is implemented such that it can be used
 * on any system, assuming it is compiled with GCC. Availability of assert.h is
 * the only assumed C library besides stdlib's malloc and free.
 */

#include <stdlib.h>
#include <assert.h>
#include "mem.h"

/* Symbols for actual malloc and free library functions */
void *__real_malloc(size_t size);
void __real_free(void *ptr);

/* malloc implementation. Signature and use is identical to stdlib malloc */
void *__wrap_malloc(size_t size)
{
    void *ptr = NULL;

    /* Return pointer to requested memory */
    return ptr;
}

/* free implementation. Signature and use is identical to stdlib free */
void __wrap_free(void *ptr)
{
}

/* Memory report function. Returns number of blocks allocated.
 * Optionally invokes report_func if supplied.
 */
unsigned int mem_report(report_func_t report_func)
{
    unsigned int count = 0;

    /* Return number of blocks */
    return count;
}
