#include <kernel/idt.h>

// The IDT table
struct idt_entry idt[256];

// The IDT pointer
struct idt_ptr ip;

// Initialize the IDT
void idt_init(void)
{
  // Set the IDT pointer
  ip.limit = (sizeof(struct idt_entry) * 256) - 1;
  ip.base = (uint32_t)&idt;

  // Clear the IDT table
  memset(&idt, 0, sizeof(struct idt_entry) * 256);
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
}
