#include "my_malloc.h"

Node * head = NULL;
size_t total_sz = 0;
size_t free_sz = 0;
Node * data_start = NULL;

void * allocate_old(size_t size) {
  size_t mem_size = size + META;
  if (head->size == mem_size) {
    Node * curr = head;
    head = head->next;
    curr->next = NULL;
    return (char *)curr + META;
  }
  else if (head->size > mem_size && head->size - mem_size > META) {
    head->size -= mem_size;
    Node * curr = (Node *)((char *)head + head->size);
    curr->next = NULL;
    curr->size = mem_size;
    return (char *)curr + META;
  }
  Node * prev = head;
  while (prev != NULL && prev->next != NULL) {
    if (prev->next->size == mem_size) {
      Node * curr = prev->next;
      prev->next = curr->next;
      curr->next = NULL;
      return (char *)curr + META;
    }
    else if (prev->next->size > mem_size && prev->next->size - mem_size > META) {
      Node * curr = prev->next;
      curr->size -= mem_size;
      Node * ans = (Node *)((char *)curr + curr->size);
      ans->next = NULL;
      ans->size = mem_size;
      return (char *)ans + META;
    }
    prev = prev->next;
  }
  return NULL;
}

void * allocate_new(size_t size) {
  size_t mem_size = size + META;
  Node * node = sbrk(mem_size);
  node->next = NULL;
  node->size = mem_size;
  return (char *)node + META;
}

void * allocate_best_old(size_t size) {
  size_t mem_size = size + META;
  if (head->size == mem_size) {
    Node * ans = head;
    head = head->next;
    ans->next = NULL;
    return (char *)ans + META;
  }

  Node * prev = NULL;
  Node * curr = head;
  Node * best = NULL;
  size_t best_diff = 0;

  while (curr != NULL) {
    if (curr->size == mem_size) {
      prev->next = curr->next;
      curr->next = NULL;
      return (char *)curr + META;
    }
    else if (curr->size > mem_size && curr->size - mem_size > META) {
      if (best_diff == 0) {
        best_diff = curr->size - mem_size;
        best = curr;
      }
      else if (best_diff > curr->size - mem_size) {
        best_diff = curr->size - mem_size;
        best = curr;
      }
    }
    prev = curr;
    curr = curr->next;
  }
  if (best != NULL && best_diff != 0) {
    best->size -= mem_size;
    Node * ans = (Node *)((char *)best + best->size);
    ans->next = NULL;
    ans->size = mem_size;
    return (char *)ans + META;
  }
  return NULL;
}

void * bf_malloc(size_t size) {
  if (data_start == NULL) {
    data_start = sbrk(0);
  }
  if (head == NULL) {
    return allocate_new(size);
  }
  else {
    Node * node = allocate_best_old(size);
    if (node == NULL)
      return allocate_new(size);
    return node;
  }
}

void * ff_malloc(size_t size) {
  if (data_start == NULL) {
    data_start = sbrk(0);
  }
  if (head == NULL) {
    return allocate_new(size);
  }
  else {
    Node * node = allocate_old(size);
    if (node == NULL)
      return allocate_new(size);
    return node;
  }
}

void insert_before_head(Node * node) {
  node->next = head;
  head = node;
}

void insert_sorted(Node * node) {
  Node * prev = head;
  while (prev != NULL && prev->next != NULL && prev->next < node) {
    prev = prev->next;
  }
  node->next = prev->next;
  prev->next = node;
}

void coalesce(Node * node) {
  if (head == node) {
    if ((char *)head + head->size == (char *)head->next) {
      head->size += head->next->size;
      head->next = head->next->next;
      node->next = NULL;
      assert(node->next == NULL);
    }
    return;
  }
  Node * prev = head;
  while (prev != NULL && prev->next != node) {
    prev = prev->next;
  }
  if ((char *)prev + prev->size == (char *)node) {
    prev->size += node->size;
    prev->next = prev->next->next;
    node->next = NULL;
    assert(node->next == NULL);
    Node * next = prev->next;
    if ((char *)prev + prev->size == (char *)next) {
      prev->size += next->size;
      prev->next = next->next;
      next->next = NULL;
    }
  }
  else if ((char *)node + node->size == (char *)node->next) {
    Node * next = node->next;
    node->size += next->size;
    node->next = next->next;
    next->next = NULL;
  }
}

void ff_free(void * ptr) {
  Node * node = (Node *)((char *)ptr - META);
  node->next = NULL;
  if (head == NULL) {
    head = node;
    head->next = NULL;
    return;
  }
  else if (head > node) {
    insert_before_head(node);
  }
  else {
    insert_sorted(node);
  }
  coalesce(node);
}

void bf_free(void * ptr) {
  ff_free(ptr);
}

unsigned long get_data_segment_size() {
  size_t size = (char *)sbrk(0) - (char *)data_start - get_data_segment_free_space_size();
  return size;
}

unsigned long get_data_segment_free_space_size() {
  Node * curr = head;
  size_t size = 0;
  while (curr != NULL) {
    size += curr->size;
    curr = curr->next;
  }
  return size;
}
