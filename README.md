# Memory Leak Detection System

This is a simple heap usage accounting library that is implemented via malloc
and free wrappers and an accounting data structure. It accomplishes this by
leveraging the `--wrap` linker option in GCC. This method of implementation
was chosen over others (such as glibc hooks or POSIX `dlsyn`) because it allows
the implementation to be platform independent. For example, it could be used
with a simple RTOS that has minimal C library functionality. It does, however,
depend on GCC as the compiler to provide both the `--wrap` linker functionality
mentioned previous, as well as the `__builtin_return_address(0)` call that
permits logging of an address in the calling function. It also makes use of
`assert` when an attempt to free unaccounted for memory occors, though this is
not strictly needed.

The implementation is fairly simple, leveraging a linear linked list of
accounting entries, one per malloc. Whenever `malloc` is called, `__wrap_malloc`
is invoked, which allocates a new node on the linked list and logs the resulting
memory block pointer, and the return address, in this node. The return address
can be used to determine the calling function that invoked malloc for a given
pointer. `free` simple invokes `__wrap_free` which unlinks the matching node
and frees the respective accounty entry and allocated memory. Some problems
and solutions for the current implementation are:

- Not thread safe: There is no synchronization mechanism in place for concurrent
  access to the accounting data structure. To fix this, a lock/unlock interface
  would need to be implemented that allows the users to define a means of
  implementing synchronization that is appropriate for the implementing system.
  This could be done with C preprocessor macros.
- Dependent on GCC: This limits the applicability of the library if it is to be
  used on projects built with other toolchains. This can be fixed by checking
  compiler macros and implementing preprocessor code to use the  appropriate
  functions for each toolchain.
- Dependent on assert: This again limits the applicability of the library
  if the given implementation of the library does not have support for the C
  library assert function. This can be fixed similar to the thread synchronization
  mechanism by providing a user definable function to call in case of an
  assert-like condition.
- May not cover `calloc` and `realloc`: Depending on the implemented C library,
  `calloc` and `realloc` may not make use of `malloc` and `free`, so these cases
  may not be accounted for and could result in asserts occuring when `free` is
  invoked for these allocations. This and the previous problem could be avoided
  all-together if instead of asserting on an unaccounted for block, it instead
  just silently attempts to perform the free operation on the pointer and returns.
- Does not account for itself: The library's accounting data structure is not a
  part of the accounting process.
