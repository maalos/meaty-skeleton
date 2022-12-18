#include <stdio.h>
#include <kernel/tty.h>
#include <math.h>
#include <io.h>
#include <drivers/keyboard.h>
#include <kernel/pic.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <drivers/screen.h>

void kernel_main(void) {
  terminal_initialize();
  gdt_init();
  idt_init();
  PIC_remap(0x20, 0x28);

  printf("Hello, kernel World!\n");

  //printf("Press any key to try dividing by zero");

  fb_clear();
  fb_putpixel(10, 10, 0xFF0000);

  getchar();

  //uint16_t out_ax, out_bx, out_cx, out_dx, out_si, out_di;

  //run_real_mode_interrupt(0x10, 0x0700, 0x3C84, 0x0000, 0x0000, 0x0000, 0x0000, &out_ax, &out_bx, &out_cx, &out_dx, &out_si, &out_di);


  //run_real_mode_interrupt(0x10, 0x0000010C, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

  //printf("1/0=%f", (1/0));

  //printf("%s", getline());

  //putchar(sc_to_char(getchar()));
}
