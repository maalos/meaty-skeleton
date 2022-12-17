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

  printf("Press any key to try dividing by zero");

  getchar();

  printf("1/0=%f", (1/0));

  //printf("%s", getline());

  //putchar(sc_to_char(getchar()));
}
