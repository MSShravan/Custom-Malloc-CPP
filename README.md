# Custom implementation of Malloc and Free functions in C

In this project, I have implemented my own version of several memory allocation
functions from the C standard library. My implementation is done in C code.
This project implement versions of malloc() and free():

void *malloc(size_t size);

void free(void *ptr);


2 versions of malloc and free are implemented, each based on a different strategy for
determining the memory region to allocate. The two strategies are:
1. First Fit: Examine the free space tracker (e.g. free list), and allocate an address from
the first free region with enough space to fit the requested allocation size.
2. Best Fit: Examine all of the free space information, and allocate an address from the
free region which has the smallest number of bytes greater than or equal to the
requested allocation size.
