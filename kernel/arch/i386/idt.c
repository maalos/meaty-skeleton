#include <kernel/idt.h>
#include <logging.h>
#include <stdio.h>
#include <string.h>

void divide_by_zero_handler(void) {
  asm ("cli");
  log(0, "Exception: Division by zero");
  while (1);
}

// The IDT table
struct idt_entry idt[256];

// The IDT pointer
struct idt_ptr ip;

// Initialize the IDT
void idt_init(void)
{
  log(0, "Initializing IDT");
  // Set the IDT pointer
  ip.limit = (sizeof(struct idt_entry) * 256) - 1;
  ip.base = (uint32_t)&idt;

  // Clear the IDT table
  memset(&idt, 0, sizeof(struct idt_entry) * 256);

  // Set an IDT entry for the division by zero exception
  idt_set_entry(0, (uint32_t)divide_by_zero_handler, 0x08, 0x8E);
}

// Set an IDT entry
void idt_set_entry(int32_t num, uint32_t offset, uint16_t selector, uint8_t type_attr)
{
  // Set the offset of the IDT entry
  idt[num].offset_low = offset & 0xFFFF;
  idt[num].offset_high = (offset >> 16) & 0xFFFF;

  // Set the selector and type/attribute fields of the IDT entry
  idt[num].selector = selector;
  idt[num].type_attr = type_attr;

  // Set the zero field of the IDT entry to 0
  idt[num].zero = 0;

  printf("Set an IDT entry no. %d, offset %u, selector %u, type_attr %u\n", num, offset, selector, type_attr);
}
