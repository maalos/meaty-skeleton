#include <kernel/idt.h>
#include <logging.h>
#include <stdio.h>
#include <string.h>
#include <kernel/isr.h>

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

  // Set an IDT entry for the divide error exception (number 0)
  idt_set_entry(0, (uint32_t)isr_00, 0x08, 0x8E);

  // Set an IDT entry for the debug exception (number 1)
  idt_set_entry(1, (uint32_t)isr_01, 0x08, 0x8E);

  // Set an IDT entry for the non-maskable interrupt exception (number 2)
  idt_set_entry(2, (uint32_t)isr_02, 0x08, 0x8E);

  // Set an IDT entry for the breakpoint exception (number 3)
  idt_set_entry(3, (uint32_t)isr_03, 0x08, 0x8E);

  // Set an IDT entry for the overflow exception (number 4)
  idt_set_entry(4, (uint32_t)isr_04, 0x08, 0x8E);

  // Set an IDT entry for the bound range exceeded exception (number 5)
  idt_set_entry(5, (uint32_t)isr_05, 0x08, 0x8E);

  // Set an IDT entry for the invalid opcode exception (number 6)
  idt_set_entry(6, (uint32_t)isr_06, 0x08, 0x8E);

  // Set an IDT entry for the device not available exception (number 7)
  idt_set_entry(7, (uint32_t)isr_07, 0x08, 0x8E);

  // Set an IDT entry for the double fault exception (number 8)
  idt_set_entry(8, (uint32_t)isr_08, 0x08, 0x8E);

  // Set an IDT entry for the coprocessor segment overrun exception (number 9)
  idt_set_entry(9, (uint32_t)isr_09, 0x08, 0x8E);

  // Set an IDT entry for the invalid TSS exception (number 10)
  idt_set_entry(10, (uint32_t)isr_10, 0x08, 0x8E);

  // Set an IDT entry for the segment not present exception (number 11)
  idt_set_entry(11, (uint32_t)isr_11, 0x08, 0x8E);

  // Set an IDT entry for the stack-segment fault exception (number 12)
  idt_set_entry(12, (uint32_t)isr_12, 0x08, 0x8E);

  // Set an IDT entry for the general protection fault exception (number 13)
  idt_set_entry(13, (uint32_t)isr_13, 0x08, 0x8E);

  // Set an IDT entry for the page fault exception (number 14)
  idt_set_entry(14, (uint32_t)isr_14, 0x08, 0x8E);

  // Set an IDT entry for the x87 FPU floating-point error exception (number 16)
  idt_set_entry(16, (uint32_t)isr_16, 0x08, 0x8E);

  // Set an IDT entry for the alignment check exception (number 17)
  idt_set_entry(17, (uint32_t)isr_17, 0x08, 0x8E);

  // Set an IDT entry for the machine check exception (number 18)
  idt_set_entry(18, (uint32_t)isr_18, 0x08, 0x8E);

  // Set an IDT entry for the SIMD floating-point exception (number 19)
  idt_set_entry(19, (uint32_t)isr_19, 0x08, 0x8E);

  // Set an IDT entry for the virtualization exception (number 20)
  idt_set_entry(20, (uint32_t)isr_20, 0x08, 0x8E);

  // Set an IDT entry for the security exception (number 30)
  idt_set_entry(30, (uint32_t)isr_30, 0x08, 0x8E);

  // Set an IDT entry for the keyboard interrupt (number 33)
  //idt_set_entry(33, (uint32_t)irq_01, 0x08, 0x8E);

  // Load the IDT into the register
  log(0, "Loading the IDT into the register");
  asm volatile("lidt %0" : : "m" (ip));
  log(0, "The IDT should now be in the register");
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

  printf("Set an IDT entry no. %02d, offset %u, selector %u, type_attr %u\n", num, offset, selector, type_attr);
}
