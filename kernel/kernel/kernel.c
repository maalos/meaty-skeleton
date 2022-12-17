#include <stdio.h>
#include <kernel/tty.h>
#include <math.h>
#include <io.h>
#include <drivers/keyboard.h>
#include <kernel/pic.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>

void kernel_main(void) {
  terminal_initialize();
  gdt_init();
  idt_init();
  PIC_remap(0x20, 0x28);

  printf("Hello, kernel World!\n");

  printf("Attempting division by zero, 2/0=?");

  printf("2/0=%f", (2/0));

  //char c = getchar();
  //printf("%c", c);

  //printf("%s", getline());

  //putchar(sc_to_char(getchar()));
}
