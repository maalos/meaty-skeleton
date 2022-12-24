//unsigned char *pixel = vram + y*pitch + x*pixelwidth;
#include <multiboot.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <kernel/paging.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))
#define offset 0xC0000000

static int xpos;
static int ypos;
static int pixelwidth;
static int pitch;

uint16_t bpl = 0;

multiboot_info_t *mbi;

unsigned char *framebuffer;
//unsigned char *backbuffer;

void fb_swap() {
  //memcpy(backbuffer, framebuffer, mbi->framebuffer_height * mbi->framebuffer_pitch);
}

void fb_setaddr(unsigned long magic, unsigned long *addr) {
  printf("trying to get the address, got 0x%x so far\n", addr);
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    printf("The kernel was not loaded by a multiboot compliant bootloader");
    asm("hlt");
  }
}

void fb_init(unsigned long magic, unsigned long *addr) {
  map_page(addr, 0xC0000000, 0x07);

  mbi = (multiboot_info_t *) 0xC0000000;

  framebuffer = (void *) (uint32_t) mbi->framebuffer_addr;

  //framebuffer = mbi->framebuffer_addr;
  //framebuffer = malloc(mbi->framebuffer_height * mbi->framebuffer_pitch);

  // Calculate the number of pages needed to hold the framebuffer
  //unsigned int num_pages = (1920 * 1080 * 4) / 4096;

  // Calculate the physical address of the first page of the framebuffer
  //unsigned long phys_addr = 0xC0010058 & ~0xFFF;

  // Calculate the virtual address where you want to map the framebuffer
  //void *virt_addr = (void *)0xFFF00000;

  // Set the flags for the page mapping (e.g. read/write, user/supervisor, etc.)
  //unsigned int flags = 0x07;  // Read/Write, Supervisor

  //for (unsigned int i = 0; i < num_pages; i++) {
    // Map each page of the framebuffer
    //map_page((void *)phys_addr, virt_addr, flags);

    // Increment the physical and virtual addresses to the next page
    //phys_addr += 4096;
    //virt_addr += 4096;
  //}

  //map_page(mbi->framebuffer_addr, mbi->framebuffer_addr, 0x0111);
  pixelwidth = mbi->framebuffer_bpp / 8;
  pitch = mbi->framebuffer_pitch;
  //for (int i = 0; i < mbi->framebuffer_height * mbi->framebuffer_pitch; i++)
  //  *(framebuffer + i) = 0x111111;

  //printf("\nFramebuffer address is 0x%x\n", mbi->framebuffer_addr);
  // let's try mapping the framebuffer
  //for(uint64_t i = mbi->framebuffer_addr; i < mbi->framebuffer_addr + (mbi->framebuffer_height * mbi->framebuffer_pitch); i += 4096) {
    //map_page(i, mbi->framebuffer_addr, 0x00000111);
  //}
}

void fb_clear() {
  for (int i = 0; i < mbi->framebuffer_height * mbi->framebuffer_pitch; i++)
    *(framebuffer + i) = 0x111111;

  xpos = 0;
  ypos = 0;
}

void fb_putpixel(int x, int y, int color) {
	unsigned where = (x * pixelwidth) + (y * pitch);
	framebuffer[where] = color & 255;
	framebuffer[where + 1] = (color >> 8) & 255;
	framebuffer[where + 2] = (color >> 16) & 255;
}

void fb_rect(int color, uint32_t pos_w, uint32_t pos_h, uint32_t w, uint32_t h) {
  for (uint32_t i = pos_w; i < pos_w + w; i++) {
    for (uint32_t j = pos_h; j < pos_h + h; j++) {
      //putpixel(vram, 64 + j, 64 + i, (r << 16) + (g << 8) + b);
      framebuffer[j*pixelwidth]     =  color        & 255;
      framebuffer[j*pixelwidth + 1] = (color >>  8) & 255;
      framebuffer[j*pixelwidth + 2] = (color >> 16) & 255;
    }
    framebuffer+=pitch;
  }
}

#define PIXEL uint32_t   /* pixel pointer */
#define USHRT_MAX  65535
#define PSF_FONT_MAGIC 0x864ab572
#define scanline (pitch)

typedef struct {
  uint32_t magic;         /* magic bytes to identify PSF */
  uint32_t version;       /* zero */
  uint32_t headersize;    /* offset of bitmaps in file, 32 */
  uint32_t flags;         /* 0 if there's no unicode table */
  uint32_t numglyph;      /* number of glyphs */
  uint32_t bytesperglyph; /* size of each glyph */
  uint32_t height;        /* height in pixels */
  uint32_t width;         /* width in pixels */
} PSF_font;

// extracted from font:
// _binary_kernel_arch_i386_font_psf_start _binary_kernel_arch_i386_font_psf_end _binary_kernel_arch_i386_font_psf_size

extern char _binary_font_psf_start;
extern char _binary_font_psf_end;

uint16_t *unicode;

void psf_init() {
  uint16_t glyph = 0;
  /* cast the address to PSF header struct */

  PSF_font *font = (PSF_font*)&_binary_font_psf_start;

  /* is there a unicode table? */
  if (font->flags) {
    unicode = NULL;
    return;
  }

  /* get the offset of the table */
  char *s = (char *)((unsigned char*)&_binary_font_psf_start + font->headersize + font->numglyph * font->bytesperglyph);

  /* allocate memory for translation table */
  unicode = calloc(USHRT_MAX, 2);
  while(s > _binary_font_psf_end) {
    uint16_t uc = (uint16_t)((unsigned char *)s[0]);
    if(uc == 0xFF) {
      glyph++;
      s++;
      continue;
    } else if(uc & 128) {
      /* UTF-8 to unicode */
      if((uc & 32) == 0 ) {
        uc = ((s[0] & 0x1F)<<6)+(s[1] & 0x3F);
        s++;
      } else
        if((uc & 16) == 0 ) {
          uc = ((((s[0] & 0xF)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F);
            s+=2;
        } else
          if((uc & 8) == 0 ) {
            uc = ((((((s[0] & 0x7)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F))<<6)+(s[3] & 0x3F);
            s+=3;
          } else
            uc = 0;
    }

    /* save translation */
    unicode[uc] = glyph;
    s++;
  }
}

void fb_setxy(unsigned short int x, unsigned short int y) {
  // TODO
}

void fb_putchar(unsigned short int c, int cx, int cy, uint32_t fg, uint32_t bg) {
  // cast the address to PSF header struct
  PSF_font *font = (PSF_font*)&_binary_font_psf_start;

  // we need to know how many bytes encode one row
  int bytesperline=(font->width+7)/8;
  bpl = bytesperline;

  // unicode translation
  if(unicode != NULL) {
    c = unicode[c];
  }

  // get the glyph for the character. If there's no glyph for a given character, we'll display the first glyph.
  unsigned char *glyph = (unsigned char*)&_binary_font_psf_start + font->headersize + (c > 0 && c < font->numglyph ? c : 0) *font->bytesperglyph;

  // calculate the upper left corner on screen where we want to display. we only do this once, and adjust the offset later. This is faster.
  int offs = (cy * font->height * scanline) + (cx * (font->width + 1) * sizeof(PIXEL));

  // finally display pixels according to the bitmap
  int x, y, line, mask;
  for(y = 0; y < font->height; y++) {
    // save the starting position of the line
    line = offs;
    mask = 1 << (font->width - 1);

    // display a row
    for(x = 0; x < font->width; x++) {
      *((PIXEL*)(framebuffer + line)) = *((unsigned int*)glyph) & mask ? fg : bg;

      // adjust to the next pixel
      mask >>= 1;
      line += sizeof(PIXEL);
    }

    // adjust to the next line
    glyph += bytesperline;
    offs  += scanline;
  }
}

void fb_scroll() {
  uint64_t pixels = mbi->framebuffer_height * mbi->framebuffer_pitch;
  memmove(framebuffer, framebuffer + (16 * mbi->framebuffer_pitch), pixels); // POSSIBLE BUG WITH UNUSED framebuffer MEMORY GOING TOO FAR BEHIND THE framebuffer BUFFER ONG
}

uint32_t *fb_get_size() {
  static uint32_t data[4];
  data[0] = mbi->framebuffer_addr;
  data[1] = mbi->framebuffer_width;
  data[2] = mbi->framebuffer_height;
  data[3] = mbi->framebuffer_bpp;
  return data;
}
