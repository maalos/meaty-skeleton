#include <drivers/keyboard.h>
#include <string.h>

int getchar(void) {
  return sc_to_char((int)getKey());
}

// THIS SHIT NEEDS TO BE FIXED

char* getline(void) {
  char line[] = "";
  while (1) {
    char c = getchar();
    if (c == '\n') break; // TODO, DOESN'T EVEN WORK :joy:
    strcat(line, c);
  }
  return line;
}
