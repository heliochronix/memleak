/**
 * test_malloc.c - Memory (de)allocation accounting library testing application
 *
 * This testing application exercises the memory accounting library functionality
 * by allocating and deallocating blocks of memory while optionally "forgetting"
 * to deallocate some blocks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mem.h"

#ifndef DEFAULT_COUNT
#define DEFAULT_COUNT 10
#endif
#ifndef DEFAULT_MISSED
#define DEFAULT_MISSED 0
#endif

/* Simulated data structure */
struct data {
    void *ptr;
    int value;
    char str[256];
};

/* Entry printing function for mem_report */
static inline void print_entry(const alloc_t *entry) {
    printf("%p allocated %lu bytes at %p\n", entry->ret_addr, entry->alloc_size, entry->alloc_ptr);
}

int main(int argc, char **argv)
{
    int c, count = DEFAULT_COUNT, miss = DEFAULT_MISSED;
    bool verbose = false, free_data = true;;
    struct data **data;

    /* Process arguments */
    while ((c = getopt(argc, argv, "b:m:sv")) != -1) {
        switch (c) {
        case 'b':
            count = strtoul(optarg, NULL, 0);
            break;
        case 'm':
            miss = strtoul(optarg, NULL, 0);
            break;
        case 's':
            free_data = false;
            break;
        case 'v':
            verbose = true;
            break;
        case '?':
        default:
            goto usage;
            break;
        }
    }
    /* Sanity check */
    if (miss > count) {
        goto usage;
    }

    /* Create data array */
    data = malloc(count * sizeof(struct data));

    /* Allocate data variables */
    for (int i = 0; i < count; i++) {
        data[i] = malloc(sizeof(struct data));
        data[i]->ptr = data[i];
        data[i]->value = i;
        sprintf(data[i]->str, "Data entry %i string", i);
    }
    /* Report current usage if verbose */
    if (verbose) {
        printf("Printing currently allocated memory...\n");
        mem_report(print_entry);
        printf("Done!\n\n");
    }

    /* Deallocate all but designated number of missed blocks */
    for (int i = 0; i < count - miss; i++) {
        free(data[i]);
        data[i] = NULL;
    }
    if (free_data) {
        free(data);
    }

    /* Final report and cleanup */
    printf("Program terminating. Finding all left over memory...\n");
    printf("Found %u blocks still allocated\n", mem_report(print_entry));
    mem_cleanup();
    exit(EXIT_SUCCESS);

usage:
    fprintf(stderr, "Usage: %s [-b <block count>] [-m <missed blocks>] [-s] [-v]\n", argv[0]);
    exit(EXIT_FAILURE);
}
