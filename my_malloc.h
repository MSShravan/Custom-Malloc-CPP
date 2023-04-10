#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _node {
  size_t size;
  struct _node * next;
} Node;

#define META sizeof(Node)

//First Fit malloc/free
void * ff_malloc(size_t size);
void ff_free(void * ptr);
//Best Fit malloc/free
void * bf_malloc(size_t size);
void bf_free(void * ptr);

unsigned long get_data_segment_size();             //in bytes
unsigned long get_data_segment_free_space_size();  //in byte
