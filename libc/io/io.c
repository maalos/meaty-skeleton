#include <stdint.h>
#include <stddef.h>

// outb writes a byte to the specified port
void outb(unsigned short port, unsigned char data) {
  asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

// inb reads a byte from the specified port
unsigned char inb(unsigned short port) {
  unsigned char data;
  asm volatile ("inb %1, %0" : "=a" (data) : "dN" (port));
  return data;
}

void io_wait(void) {
  outb(0x80, 0);
}

// Function to run a real mode interrupt from protected mode
void run_real_mode_interrupt(uint8_t interrupt_number, uint16_t ax, uint16_t bx, uint16_t cx, uint16_t dx, uint16_t si, uint16_t di, uint16_t *out_ax, uint16_t *out_bx, uint16_t *out_cx, uint16_t *out_dx, uint16_t *out_si, uint16_t *out_di)
{
 	// Save the current processor state
  uint16_t current_cs, current_ds, current_es, current_fs, current_gs, current_ss;
  __asm__ __volatile__(    "mov %%cs, %0\n"
    "mov %%ds, %1\n"
    "mov %%es, %2\n"
    "mov %%fs, %3\n"
    "mov %%gs, %4\n"
    "mov %%ss, %5\n": "=m"(current_cs), "=m"(current_ds), "=m"(current_es), "=m"(current_fs), "=m"(current_gs), "=m"(current_ss)
 );

 	// Set up a real mode stack in memory
  uint16_t real_mode_stack[256];
  uint16_t real_mode_stack_size = sizeof(real_mode_stack) / 2;
  uint16_t real_mode_stack_segment = 0x1000;
  uint16_t real_mode_stack_pointer = real_mode_stack_size - 1;

 	// Set up a real mode code segment in memory
  uint8_t real_mode_code[32];
  uint16_t real_mode_code_size = sizeof(real_mode_code);
  uint16_t real_mode_code_segment = 0x100;

 	// Set up a real mode data segment in memory
  uint8_t real_mode_data[32];
  uint16_t real_mode_data_size = sizeof(real_mode_data);
  uint16_t real_mode_data_segment = 0x200;

 	// Set up a real mode environment in memory
  uint16_t real_mode_environment[256];
  uint16_t real_mode_environment_size = sizeof(real_mode_environment) / 2;
  uint16_t real_mode_environment_segment = 0x300;

 	// Set up a real mode far pointer to the interrupt vector table
  uint16_t real_mode_interrupt_vector_table_segment = 0;
  uint16_t real_mode_interrupt_vector_offset = interrupt_number * 4;

 	// Set up a real mode far pointer to the interrupt handler
  uint16_t real_mode_interrupt_handler_segment = real_mode_code_segment;
  uint16_t real_mode_interrupt_handler_offset = 0;

 	// Set up the real mode code to call the interrupt
  real_mode_code[0] = 0xCD;	// int instruction
  real_mode_code[1] = interrupt_number;
  real_mode_code[2] = 0xCB;	// far ret instruction

 	// Set up the real mode environment to restore the processor state
  real_mode_environment[0] = current_cs;
  real_mode_environment[1] = (uint16_t) &real_mode_code[2];
  real_mode_environment[2] = current_ds;
  real_mode_environment[3] = current_es;
  real_mode_environment[4] = current_fs;
  real_mode_environment[5] = current_gs;
  real_mode_environment[6] = current_ss;
  real_mode_environment[7] = (uint16_t) &real_mode_stack[real_mode_stack_pointer];

 	// Set up the real mode stack with the necessary data
  real_mode_stack[--real_mode_stack_pointer] = di;
  real_mode_stack[--real_mode_stack_pointer] = si;
  real_mode_stack[--real_mode_stack_pointer] = dx;
  real_mode_stack[--real_mode_stack_pointer] = cx;
  real_mode_stack[--real_mode_stack_pointer] = bx;
  real_mode_stack[--real_mode_stack_pointer] = ax;
  real_mode_stack[--real_mode_stack_pointer] = real_mode_code_segment;
  real_mode_stack[--real_mode_stack_pointer] = real_mode_interrupt_handler_offset;
  real_mode_stack[--real_mode_stack_pointer] = real_mode_interrupt_vector_table_segment;
  real_mode_stack[--real_mode_stack_pointer] = real_mode_interrupt_vector_offset;
  real_mode_stack[--real_mode_stack_pointer] = real_mode_environment_segment;
  real_mode_stack[--real_mode_stack_pointer] = (uint16_t) &real_mode_environment[0];

 	// Set up the real mode data segment with the necessary data
  *(uint16_t*) &real_mode_data[0] = real_mode_stack_segment;
  *(uint16_t*) &real_mode_data[2] = real_mode_stack_pointer;
  *(uint16_t*) &real_mode_data[4] = real_mode_code_segment;
  *(uint16_t*) &real_mode_data[6] = real_mode_interrupt_handler_offset;
  *(uint16_t*) &real_mode_data[8] = real_mode_interrupt_vector_table_segment;
  *(uint16_t*) &real_mode_data[10] = real_mode_interrupt_vector_offset;
  *(uint16_t*) &real_mode_data[12] = real_mode_environment_segment;
  *(uint16_t*) &real_mode_data[14] = (uint16_t) &real_mode_environment[0];

 	// Load the real mode data segment
  __asm__ __volatile__(    "mov %0, %%ds\n"
    "mov %0, %%es\n":
    : "r"(real_mode_data_segment)
 );

 	// Load the real mode stack segment and stack pointer
  __asm__ __volatile__(    "mov %0, %%ss\n"
    "mov %1, %%sp\n":
    : "r"(real_mode_stack_segment), "r"(real_mode_stack_pointer)
 );

 	// Load the real mode code segment and instruction pointer
  __asm__ __volatile__(    "ljmp %0, $real_mode_entry\n"
    "real_mode_entry:\n":
    : "i"(real_mode_code_segment), "i"(real_mode_interrupt_handler_offset)
 );

 	// Save the output registers
  __asm__ __volatile__(    "mov %%ax, %0\n"
    "mov %%bx, %1\n"
    "mov %%cx, %2\n"
    "mov %%dx, %3\n"
    "mov %%si, %4\n"
    "mov %%di, %5\n": "=m"(*out_ax), "=m"(*out_bx), "=m"(*out_cx), "=m"(*out_dx), "=m"(*out_si), "=m"(*out_di)
 );
}
