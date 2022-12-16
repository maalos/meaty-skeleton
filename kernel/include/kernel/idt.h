#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

// The IDT entry structure
struct idt_entry
{
  uint16_t offset_low;
  uint16_t selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_high;
} __attribute__((packed));

// The IDT pointer structure
struct idt_ptr
{
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

// The IDT table and its associated functions
void idt_init(void);
void idt_set_entry(int32_t num, uint32_t offset, uint16_t selector, uint8_t type_attr);

#endif
