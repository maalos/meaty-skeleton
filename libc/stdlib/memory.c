#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct block_header {
  size_t size;
  struct block_header *next;
  int free;
} block_header;

#define BLOCK_HEADER_SIZE sizeof(block_header)

static block_header *head = NULL;

void *sbrk(int increment) {
  static void *current_break = NULL;

  if (current_break == NULL) {
    current_break = (void *)4096;
  }

  void *previous_break = current_break;
  current_break += increment;
  return previous_break;
}


void *malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  block_header *current = head;
  block_header *previous = NULL;

  size_t total_size = ALIGN(size + BLOCK_HEADER_SIZE);

  while (current) {
    if (current->free && current->size >= size) {
      current->free = 0;
      return (void *)(current + 1);
    }

    previous = current;
    current = current->next;
  }

  block_header *block = sbrk(0);
  void *request = sbrk(total_size);
  if (request == (void *) -1) {
    return NULL;
  }

  block->size = size;
  block->next = NULL;
  block->free = 0;

  if (previous) {
    previous->next = block;
  } else {
    head = block;
  }

  return (void *)(block + 1);
}

void *calloc(size_t nmemb, size_t size) {
  size_t total_size = nmemb * size;
  void *ptr = malloc(total_size);
  if (ptr) {
    memset(ptr, 0, total_size);
  }
  return ptr;
}

void *realloc(void *ptr, size_t size) {
  if (ptr == NULL) {
    return malloc(size);
  }

  if (size == 0) {
    free(ptr);
    return NULL;
  }

  block_header *block = (block_header *)ptr - 1;
  if (block->size >= size) {
    return ptr;
  }

  void *new_ptr = malloc(size);
  if (new_ptr == NULL) {
    return NULL;
  }

  memcpy(new_ptr, ptr, block->size);
  free(ptr);
  return new_ptr;
}

void free(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  block_header *block = (block_header *)ptr - 1;
  block->free = 1;
}

