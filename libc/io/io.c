// outb writes a byte to the specified port
void outb(unsigned short port, unsigned char data) {
  asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

// inb reads a byte from the specified port
unsigned char inb(unsigned short port) {
  unsigned char data;
  asm volatile ("inb %1, %0" : "=a" (data) : "dN" (port));
  return data;
}

void io_wait(void) {
    outb(0x80, 0);
}
