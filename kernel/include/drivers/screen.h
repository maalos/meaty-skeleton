#ifndef _DRIVER_SCREEN_H
#define _DRIVER_SCREEN_H

#include <stdint.h>

void fb_swap();
void fb_clear();
void fb_putpixel(int x, int y, int color);
void fb_init();
void fb_setaddr(unsigned long magic, unsigned long addr);
void fb_rect(unsigned char r, unsigned char g, unsigned char b, unsigned char sw, unsigned char sh, unsigned char w, unsigned char h);
void fb_putchar(unsigned short int c, int cx, int cy, uint32_t fg, uint32_t bg);
void fb_scroll();
uint32_t fb_get_size();

#endif
