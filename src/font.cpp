#include "../inc/font.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <cassert>
#include <cstring>

Chars *Font::_chars(void) { return &chars; }
Chars Font::create_char_inst(void) { return Chars(); }

Font::Font(void) : chars(create_char_inst()) {
  fprintf(stdout, "Font instance created\n");
}

Fonts *Font::get_font(void) { return &fonts; }

TTF_Font *Font::open_font(const char *fn, const int size) {
  TTF_Font *f = TTF_OpenFont(fn, size);
  if (!f) {
    fprintf(stderr, "Failed to open font! -> %s\n", TTF_GetError());
  }
  return f;
}
