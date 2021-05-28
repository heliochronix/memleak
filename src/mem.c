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
 * on any system, assuming it is compiled with GCC.
 */

#include <stdlib.h>
#include "mem.h"

/* Linked list head */
alloc_t *head = NULL;

/* Symbols for actual malloc and free library functions */
void *__real_malloc(size_t size);
void __real_free(void *ptr);

/* malloc implementation. Signature and use is identical to stdlib malloc */
void *__wrap_malloc(size_t size)
{
    void *ptr = NULL;
    alloc_t *next = head;

    /* Allocate accounting entry */
    head = __real_malloc(sizeof(alloc_t));
    if (head == NULL) {
        goto malloc_fail;
    }
    /* Allocate requested memory */
    ptr = __real_malloc(size);
    if (ptr == NULL) {
       __real_free(head);
       goto malloc_fail;
    }
    /* Populate accounting information and link */
    head->alloc_ptr = ptr;
    head->ret_addr = __builtin_return_address(0);
    head->alloc_size = size;
    head->next = next;

    /* Return pointer to requested memory */
    return ptr;

malloc_fail:
    /* Failure to allocate, restore state */
    head = next;
    return NULL;
}

/* free implementation. Signature and use is identical to stdlib free */
void __wrap_free(void *ptr)
{
    alloc_t *entry = head;
    alloc_t *prev = NULL;

    /* If NULL, no need to do anything */
    if (ptr == NULL) {
        return;
    }

    /* Find accounting entry for pointer */
    while (entry && (entry->alloc_ptr != ptr)) {
        prev = entry;
        entry = entry->next;
    }

    if (entry != NULL) {
        /* Unlink and deallocate accounting memory */
        if (prev != NULL) {
            prev->next = entry->next;
        } else {
            head = entry->next;
        }
        __real_free(entry);
    }
    /* Free the block of memory */
    __real_free(ptr);
    return;
}

/* Memory report function. Returns number of blocks allocated.
 * Optionally invokes report_func if supplied.
 */
unsigned int mem_report(report_func_t report_func)
{
    unsigned int count = 0;
    alloc_t *entry = head;
    alloc_t *next = NULL;

    /* Find all allocated blocks */
    while (entry) {
        next = entry->next;
        if (report_func) {
            report_func(entry);
        }
        entry = next;
        count++;
    }

    /* Return number of blocks */
    return count;
}

/* Memory accounting cleanup. Removes all accounting data structures */
void mem_cleanup(void)
{
    alloc_t *entry = head;
    alloc_t *next = NULL;

    while (entry) {
        next = entry->next;
        __real_free(entry);
        entry = next;
    }
    head = NULL;
}
