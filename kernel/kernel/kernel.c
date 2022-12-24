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
#include <kernel/paging.h>
#include <kernel/serial.h>

void kernel_main(unsigned long magic, unsigned long *addr) {
  struct Stack stack;

  serial_init(); // hardware debugging goes first ;p
  gdt_init();
  idt_init();

  stack_init(&stack);
  recursive_paging_init();

  fb_init(magic, addr);

  pit_init();
  PIC_remap(0x20, 0x28);

  //printf("Hello, kernel World!\n");

  //fb_swap();

  //fb_putpixel(20, 20, 0xFF0000);

  int *t;
  t = fb_get_size();
  printf("Framebuffer info: %ux%ux%u at 0x%x", t[1], t[2], t[3], t[0]);

  //fb_scroll();

  //while(1)
  //printf("\n%u", get_current_time());

  //delay(1000);

  //fb_rect(0xFF0000, 100, 200, 200, 100);

  //printf("\n%u", get_current_time());

  //fb_swap();
}
