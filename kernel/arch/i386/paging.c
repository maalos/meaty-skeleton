#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_PRESENT (1 << 0)  // Page is present in physical memory
#define PAGE_SUPERVISOR (1 << 1)  // Page is accessible by supervisor-level code
#define PAGE_READWRITE (1 << 2)  // Page is readable and writable

struct Page {
  void *ptr;  // pointer to the start of the memory page
  size_t size;  // size of the memory page
  struct Page *next;  // pointer to the next page in the stack
};

struct Stack {
  struct Page *top;  // pointer to the top page in the stack
  size_t size;  // number of pages in the stack
};

// Gets the current page directory address
void *get_cr3(void) {
  uint32_t cr3;
  asm volatile("movl %%cr3, %0" : "=r"(cr3));
  return (void *)(uintptr_t)cr3;
}

// Sets the current page directory address
void set_cr3(void *cr3) {
  asm volatile("movl %0, %%cr3" :: "r"((uint32_t)(uintptr_t)cr3));
}

// God knows whether this shit even works lol
void *ismapped(void *virtualaddr) {
  unsigned long pdindex = (unsigned long)virtualaddr >> 22;
  unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
  unsigned long *pd = (unsigned long *)0xFFFFF000;
  unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);

  if (pt[ptindex] & 0x01)
    return 1;
  
  return 0;
}

void *get_physaddr(void *virtualaddr) {
  unsigned long pdindex = (unsigned long)virtualaddr >> 22;
  unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
  // Here you need to check whether the PD entry is present.
  unsigned long *pd = (unsigned long *)0xFFFFF000;
  unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
 
  // Here you need to check whether the PT entry is present. 
  return (void *)((pt[ptindex] & ~0xFFF) + ((unsigned long)virtualaddr & 0xFFF));
}

void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
    // Make sure that both addresses are page-aligned.
 
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
    unsigned long *pd = (unsigned long *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.
    // When it is not present, you need to create a new empty PT and
    // adjust the PDE accordingly.
 
    unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
    // When it is, then there is already a mapping present. What do you do now?
 
    pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
 
    // Now you need to flush the entry in the TLB
    // or you might not notice the change.
}

void stack_init(struct Stack *stack) {
  stack->top = NULL;
  stack->size = 0;
}

void stack_push(struct Stack *stack, struct Page *page) {
  page->next = stack->top;
  stack->top = page;
  stack->size++;
}

struct Page *stack_pop(struct Stack *stack) {
  if (stack->size == 0) return NULL;  // stack is empty

  struct Page *page = stack->top;
  stack->top = page->next;
  stack->size--;
  return page;
}

struct Page *page_alloc(size_t size, unsigned int flags) {
  struct Page *page = malloc(sizeof(struct Page));
  page->ptr = malloc(size);
  page->size = size;
  page->next = NULL;

  // Set the correct flags for the page using the map_page function
  map_page(page->ptr, page->ptr, flags);

  return page;
}

void page_free(struct Page *page) {
  // Clear the flags for the page using the map_page function
  map_page(page->ptr, page->ptr, 0);

  free(page->ptr);
  free(page);
}

void recursive_paging_init(void) {
  // Get the current physical and virtual page directory address
  uint32_t pd_phys = get_cr3();
  uint32_t *pd_ptr = (uint32_t*)(pd_phys + 0xC0000000);
  
  // Set the page directory self-reference
  pd_ptr[1023] = pd_phys | 0x03;
}

