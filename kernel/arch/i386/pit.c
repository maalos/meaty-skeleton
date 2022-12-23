#include <io.h>
#include <stdint.h>
#include <stdio.h> //debugging purposes

#define PIT_CMD_PORT 0x43  // command port for PIT
#define PIT_CH0_PORT 0x40  // channel 0 data port for PIT

// Initialize the PIT
void pit_init() {
  // send command byte to PIT
  outb(PIT_CMD_PORT, 0x30);
}

// Set the frequency of the interval timer
void set_pit_frequency(uint16_t frequency) {
  uint16_t divisor = 1193180 / frequency;  // calculate the divisor
  printf("Set timer freq to %u, divisor %u", frequency, divisor);

  // send command byte to PIT
  outb(PIT_CMD_PORT, 0x36);

  // send the divisor to PIT channel 0
  outb(PIT_CH0_PORT, divisor & 0xff);
  outb(PIT_CH0_PORT, (divisor >> 8) & 0xff);
}

// Get the current time in milliseconds
uint32_t get_current_time() {
  // send command byte to PIT
  outb(PIT_CMD_PORT, 0x00);

  // read current value from PIT channel 0
  uint32_t count = inb(PIT_CH0_PORT);
  count |= inb(PIT_CH0_PORT) << 8;

  // calculate current time in milliseconds
  return (1193180 - count) / 1193;
}

void delay(uint32_t milliseconds) {
  // Wait for the specified number of milliseconds
  // set the frequency of the interval timer to 1000 Hz
  set_pit_frequency(200);
  printf("Starting timer");
  uint32_t start_time = get_current_time();
  while (get_current_time() - start_time < milliseconds)
    //putchar('d');
    ;
  printf("timer done");
}
