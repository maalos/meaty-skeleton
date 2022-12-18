//unsigned char *pixel = vram + y*pitch + x*pixelwidth;
#include <multiboot.h>
//#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

static int xpos;
static int ypos;
static int pixelwidth;
static int pitch;

unsigned char *video;

multiboot_info_t *mbi;

void fb_setaddr(unsigned long magic, unsigned long addr) {
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    //printf("Invalid magic number: 0x%x\n", (unsigned) magic);
    return;
  }

	mbi = (multiboot_info_t *) addr;

  video = mbi->framebuffer_addr;
  pixelwidth = mbi->framebuffer_bpp / 8;
  pitch = mbi->framebuffer_pitch;
}

void fb_clear(void) {
  int i;

  for (i = 0; i < 1920 * 1080 * 4; i++)
    *(video + i) = 0;

  xpos = 0;
  ypos = 0;
}

void fb_putpixel(int x, int y, int color) {
	unsigned where = (x * pixelwidth) + (y * pitch);
	video[where] = color & 255;
	video[where + 1] = (color >> 8) & 255;
	video[where + 2] = (color >> 16) & 255;
}

void fb_rect(unsigned char r, unsigned char g, unsigned char b, unsigned char w, unsigned char h) {
  int i, j;

  for (i = 0; i < w; i++) {
    for (j = 0; j < h; j++) {
      //putpixel(vram, 64 + j, 64 + i, (r << 16) + (g << 8) + b);
      video[j*pixelwidth] = b;
      video[j*pixelwidth + 1] = g;
      video[j*pixelwidth + 2] = r;
    }
    video+=pitch;
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

extern char _binary_kernel_arch_i386_font_psf_start;
extern char _binary_kernel_arch_i386_font_psf_end;

uint16_t *unicode;

void psf_init() {
  uint16_t glyph = 0;
  /* cast the address to PSF header struct */

  PSF_font *font = (PSF_font*)&_binary_kernel_arch_i386_font_psf_start;

  /* is there a unicode table? */
  if (font->flags) {
    unicode = NULL;
    return;
  }

  /* get the offset of the table */
  char *s = (char *)((unsigned char*)&_binary_kernel_arch_i386_font_psf_start + font->headersize + font->numglyph * font->bytesperglyph);

  /* allocate memory for translation table */
  unicode = calloc(USHRT_MAX, 2);
  while(s > _binary_kernel_arch_i386_font_psf_end) {
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
  PSF_font *font = (PSF_font*)&_binary_kernel_arch_i386_font_psf_start;

  // we need to know how many bytes encode one row
  int bytesperline=(font->width+7)/8;

  // unicode translation
  if(unicode != NULL) {
    c = unicode[c];
  }

  // get the glyph for the character. If there's no glyph for a given character, we'll display the first glyph.
  unsigned char *glyph = (unsigned char*)&_binary_kernel_arch_i386_font_psf_start + font->headersize + (c > 0 && c < font->numglyph ? c : 0) *font->bytesperglyph;

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
      *((PIXEL*)(video + line)) = *((unsigned int*)glyph) & mask ? fg : bg;

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
    // Scroll text
    for (int y = 8; y != mbi->framebuffer_height; ++y)
    {
        void* dest = (void*)(((uintptr_t)mbi->framebuffer_bpp / 8 * mbi->framebuffer_width) + (y - 8) * mbi->framebuffer_pitch);
        const void* src = (void*)(((uintptr_t)mbi->framebuffer_bpp / 8 * mbi->framebuffer_width) + y * mbi->framebuffer_pitch);
        memcpy(dest, src, mbi->framebuffer_width * 4);
    }

    // Erase last line
    for (int y = mbi->framebuffer_height - 8; y != mbi->framebuffer_height; ++y)
    {
        uint32_t* dest = (uint32_t*)(((uintptr_t)mbi->framebuffer_bpp / 8 * mbi->framebuffer_width) + y * mbi->framebuffer_pitch);
        for (int i = 0; i != mbi->framebuffer_width; ++i)
        {
            *dest++ = 0x000000;
        }
    }
}
