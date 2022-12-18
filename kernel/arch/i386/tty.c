#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 240;  // likely 240 for 1920
static const size_t VGA_HEIGHT = 135; // likely 135 for 1080
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

// TODO: CLEAN UP THIS FUCKING MESS

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	unsigned char uc = c;
  if (c == '\n') {
	  ++terminal_row;
    terminal_column = -1;
  } else
    //terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
    fb_putchar((int)uc, terminal_column, terminal_row, 0xFFFFFF, 0x000000);

  if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
  }
	if (terminal_row == VGA_HEIGHT) {
		terminal_row = VGA_HEIGHT - 1;
    //terminal_scroll();
    fb_scroll();
  }
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

// TODO: make the below function work ong

void terminal_scroll(void) {
  memmove(terminal_buffer, terminal_buffer + VGA_WIDTH, VGA_WIDTH * (VGA_HEIGHT - 1) * sizeof(uint16_t));  
  size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH;
  for (size_t x = 0; x < VGA_WIDTH; ++x) {
    terminal_buffer[index + x] = vga_entry(' ', terminal_color);
  }
}
