#include "Inkplate.h"
#include "Util.h"

char* replaceChar(char* str, char find, char replace) {
    char *current_pos = strchr(str ,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos, find);
    }
    return str;
}

char* replaceUmlauts(char *str) {
  return replaceChar(replaceChar(str, 'ä', 'a'), 'ö', 'o');
}
