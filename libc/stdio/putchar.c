#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

// TODO merge those functions

int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	terminal_write(&c, sizeof(c));
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}

char _putchar(char ch) {
  terminal_write(&ch, sizeof(ch));

  return ch;
}
