#include <stdio.h>
#include <kernel/tty.h>
#include <math.h>
#include <io.h>
#include <drivers/keyboard.h>
#include <kernel/pic.h>

void kernel_main(void) {
  PIC_remap(0x20, 0x28);
  terminal_initialize();

  printf("Hello, kernel World!\n");

  //char c = getchar();
  //printf("%c", c);

  //printf("%s", getline());

  //putchar(sc_to_char(getchar()));
}
