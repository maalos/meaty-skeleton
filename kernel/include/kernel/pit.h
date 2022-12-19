#ifndef _KERNEL_PIT_H
#define _KERNEL_PIT_H

#include <stdint.h>

// Initialize the PIT
void pit_init();

// Set the frequency of the interval timer
void set_pit_frequency(uint16_t frequency);

// Wait for the specified number of milliseconds
void delay(uint32_t milliseconds);

// Get the current time in milliseconds
uint32_t get_current_time();

#endif
