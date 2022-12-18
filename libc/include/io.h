#ifndef _IO_H
#define _IO_H 1
#include <stdint.h>

void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
void io_wait(void);
void run_real_mode_interrupt(uint8_t interrupt_number, uint16_t ax, uint16_t bx, uint16_t cx, uint16_t dx, uint16_t si, uint16_t di, uint16_t *out_ax, uint16_t *out_bx, uint16_t *out_cx, uint16_t *out_dx, uint16_t *out_si, uint16_t *out_di);

#endif /* ifndef _IO_H  */
