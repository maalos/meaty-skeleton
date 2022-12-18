//unsigned char *pixel = vram + y*pitch + x*pixelwidth;
#include <multiboot.h>
#include <stdio.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

static int xpos;
static int ypos;
static int pixelwidth;
static int pitch;

unsigned char *video;

multiboot_info_t *mbi;

void fb_setaddr(unsigned long magic, unsigned long addr) {
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    printf("Invalid magic number: 0x%x\n", (unsigned) magic);
    return;
  }

	mbi = (multiboot_info_t *) addr;

  video = mbi->framebuffer_addr;
  pixelwidth = mbi->framebuffer_bpp;
  pitch = mbi->framebuffer_pitch;
}

void fb_clear(void) {
  int i;

  for (i = 0; i < 1280 * 1024 * 3; i++)
    *(video + i) = 0;

  xpos = 0;
  ypos = 0;
}

void fb_putpixel(int x, int y, int color) {
	unsigned where = x * pixelwidth + y * pitch;
	video[where] = color & 255;
	video[where + 1] = (color >> 8) & 255;
	video[where + 2] = (color >> 16) & 255;
}
