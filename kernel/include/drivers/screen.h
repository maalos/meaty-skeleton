#ifndef _DRIVER_SCREEN_H
#define _DRIVER_SCREEN_H

void fb_clear(void);
void fb_putpixel(int x, int y, int color);
void fb_setaddr(unsigned long magic, unsigned long addr);

#endif
