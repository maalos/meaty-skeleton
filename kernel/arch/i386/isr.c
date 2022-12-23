#include <stdio.h>
#include <kernel/paging.h>
#include <stdint.h>

//struct interrupt_frame
//{
//    uword_t ip;
//    uword_t cs;
//    uword_t flags;
//    uword_t sp;
//    uword_t ss;
//};
 
//__attribute__ ((interrupt))
//void panic_full(struct interrupt_frame *frame, const chat* text) { // TODO USE THIS ONE INSTEAD OF THE OTHER ONES
//  asm("cli");
//  printf("\nKernel panic\nCaught an exception: %s\n| IP: 0x%x | CS: 0x%x | FLAGS: 0x%x | SP: 0x%x | SS: 0x%x | Halting the system", text, frame->ip, frame->cs, frame->flags, frame->sp, frame->ss);
//  while(1);
//}

void panic(const char* text) {
	asm ("cli");
	printf("\nKernel panic\nCaught an exception: %s\nHalting the system", text);
	while(1);
}

void page_fault_handler(const char* text) {
	uint32_t virt_pagefault_addr;
	asm volatile("movl %%cr2, %0" : "=r"(virt_pagefault_addr));
	printf("\nPage fault at virtual address 0x%x\n", &virt_pagefault_addr);
	asm("cli");
	while(1);
	panic("Halting from Page fault handler");
}

void isr_00(void) {
  panic("Divide error exception");
}

void isr_01(void) {
  panic("Debug exception");
}

void isr_02(void) {
  panic("Non-maskable interrupt exception");
}

void isr_03(void) {
  panic("Breakpoint exception");
}

void isr_04(void) {
  panic("Overflow exception");
}

void isr_05(void) {
  panic("Bound range exceeded exception");
}

void isr_06(void) {
  panic("Invalid opcode exception");
}

void isr_07(void) {
  panic("Device not available exception");
}

void isr_08(void) {
  panic("Double fault exception");
}

void isr_09(void) {
  panic("Coprocessor segment overrun exception");
}

void isr_10(void) {
  panic("Invalid TSS exception");
}

void isr_11(void) {
  panic("Segment not present exception");
}

void isr_12(void) {
  panic("Stack-segment fault exception");
}

void isr_13(void) {
  panic("General protection fault exception");
}

void isr_14(void) {
  page_fault_handler("Page fault exception");
}

void isr_16(void) {
	panic("x87 Floating-Point exception");
}

void isr_17(void) {
  panic("Alignment check exception");
}

void isr_18(void) {
  panic("Machine check exception");
}

void isr_19(void) {
  panic("SIMD floating-point exception");
}

void isr_20(void) {
  panic("Virtualization exception");
}

void isr_30(void) {
  panic("Security exception");
}

void isr_128(void) {
  panic("System call interrupt");
}


