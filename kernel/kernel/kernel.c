#include <stdio.h>
#include <kernel/tty.h>
#include <math.h>
#include <io.h>
#include <drivers/keyboard.h>
#include <kernel/pic.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <drivers/screen.h>
#include <kernel/pit.h>

void kernel_main() {
  fb_clear();
  gdt_init();
  idt_init();
  pit_init();
  PIC_remap(0x20, 0x28);

  printf("Hello, kernel World!\n");

  //fb_swap();

  int *t;
  t = fb_get_size();
  printf("Framebuffer info: %ux%ux%u", t[0], t[1], t[2]);

  //fb_scroll();

  //while(1)
  //  printf("\n%u", get_current_time());

  fb_rect(30, 30, 30, 500, 500, 100, 100);

  //fb_swap();
}
