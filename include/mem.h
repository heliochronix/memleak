/**
 * mem.h - Memory (de)allocation accounting library header
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
 * on any system, assuming it is compiled with GCC.
 */

#ifndef _MEM_H_
#define _MEM_H_

/* Allocation accounting information structure */
typedef struct alloc {
    const void *alloc_ptr;  /* Allocated memory pointer */
    const void *ret_addr;   /* Return address, indicates function calling malloc */
    size_t alloc_size;      /* Allocated size */
    struct alloc *next;     /* Next allocation entry */
} alloc_t;

/* Function pointer signature for report printing */
typedef void (*report_func_t)(const alloc_t *entry);

/* Memory usage reporting function. Returns the number of allocated blocks.
 * Optionally invokes report_func to allow users to print the data.
 */
unsigned int mem_report(report_func_t report_func);

/* Memory accounting cleanup. Removes all accounting data structures */
void mem_cleanup(void);

#endif /* _MEM_H_ */
