#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#include <stddef.h>

#define PAGE_PRESENT (1 << 0)
#define PAGE_SUPERVISOR (1 << 1)
#define PAGE_READWRITE (1 << 2)

struct Page {
  void *ptr;  // pointer to the start of the memory page
  size_t size;  // size of the memory page
  struct Page *next;  // pointer to the next page in the stack
};

struct Stack {
  struct Page *top;  // pointer to the top page in the stack
  size_t size;  // number of pages in the stack
};

void *get_cr3(void);
void set_cr3(void *cr3);

void *get_physaddr(void *virtualaddr);
void map_page(void *physaddr, void *virtualaddr, unsigned int flags);

// Initializes a new stack
void stack_init(struct Stack *stack);

// Pushes a new page onto the stack
void stack_push(struct Stack *stack, struct Page *page);

// Removes and returns the top page from the stack
struct Page *stack_pop(struct Stack *stack);

// Allocates a new memory page of the specified size and sets the specified flags
struct Page *page_alloc(size_t size, unsigned int flags);

// Frees a memory page and clears the flags for the page
void page_free(struct Page *page);

// Initializes recursive paging for the current process
void recursive_paging_init(void);

#endif
