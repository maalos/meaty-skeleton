#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h>

// The GDT entry structure
struct gdt_entry
{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed));

// The GDT pointer structure
struct gdt_ptr
{
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

// The GDT table and its associated functions
void gdt_init(void);
// Initializes the GDT table

void gdt_set_entry(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
// Sets a GDT entry with the given parameters

#endif
