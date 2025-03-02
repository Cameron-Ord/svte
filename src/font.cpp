#include "../inc/font.hpp"
#include <SDL2/SDL_ttf.h>

Font font_instance;
Font *get_font_inst(void) { return &font_instance; }

Font::Font(void) {
  size = 16;
  set_table();
}

TTF_Font *Font::get_font(void) { return f; }

const void *Font::open_font(const char *fn) {
  f = TTF_OpenFont(fn, size);
  if (!f) {
    fprintf(stderr, "Failed to open font! -> %s\n", TTF_GetError());
  }
  return f;
}

void Font::set_table(void) {
  for (int i = 32; i < ASCII_TABLE_SIZE; i++) {
    tbl[i].c = i;
    const char str[] = {(char)i, '\0'};
    memcpy(tbl[i].str, str, 2);
  }
}
