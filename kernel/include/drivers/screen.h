#ifndef _DRIVER_SCREEN_H
#define _DRIVER_SCREEN_H

#include <stdint.h>

void fb_swap();
void fb_clear();
void fb_putpixel(int x, int y, int color);
void fb_init();
void fb_setaddr(unsigned long magic, unsigned long addr);
void fb_rect(int color, uint32_t pos_w, uint32_t pos_h, uint32_t w, uint32_t h);
void fb_putchar(unsigned short int c, int cx, int cy, uint32_t fg, uint32_t bg);
void fb_scroll();
uint32_t fb_get_size();

#endif
