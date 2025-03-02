#include "../inc/font.hpp"

#include <cstring>

static int force_bounds(const int i) {
  if (i >= ASCII_TABLE_SIZE) {
    return 0;
  } else if (i < 0) {
    return 0;
  }
  return 1;
}

Char_Tables *Font::get_tbls(void) { return chtbls; }
void Font::set_table(void) {
  for (int i = 32; i < ASCII_TABLE_SIZE; i++) {
    chtbls[i].c = i;
    const char str[] = {(char)i, '\0'};
    memcpy(chtbls[i].str, str, 2);
  }
}

Char_Tables *Font::char_lookup(unsigned char c, const int selector) {
  if (!force_bounds((int)c)) {
    return &chtbls['?'];
  }

  return &chtbls[c];
}
