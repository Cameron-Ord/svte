#include "../inc/font.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <cstring>
#include <iostream>

Chars::Chars(void) { set_table(); }

static SDL_Surface *font_surface(TTF_Font *f, const char *buf, SDL_Color col) {
  return TTF_RenderText_Solid(f, buf, col);
}

static SDL_Texture *font_texture(SDL_Renderer *rend, SDL_Surface *s) {
  return SDL_CreateTextureFromSurface(rend, s);
}

int Chars::table_create_textures(SDL_Renderer *rend, const Fonts *f) {
  SDL_Color col = {255, 255, 255, 255};
  TTF_Font *fnts[] = {f->def, f->title};
  for (int i = 32; i < ASCII_TABLE_SIZE; i++) {
    Char_Tables *ct = &chtbls[i];
    Ascii_Char *ascii_buf[] = {&ct->def, &ct->title};

    for (int i = 0; i < 2; i++) {
      SDL_Surface *s = font_surface(fnts[i], ct->str, col);
      if (!s) {
        std::cerr << "Failed to create surface!" << " " << SDL_GetError()
                  << std::endl;
        return -1;
      }
      if (!(ascii_buf[i]->t = font_texture(rend, s))) {
        std::cerr << "Failed to create texture!" << " " << SDL_GetError()
                  << std::endl;
        return -1;
      }
      ascii_buf[i]->width = s->w, ascii_buf[i]->height = s->h;
      SDL_FreeSurface(s);
    }
  }
  return 1;
}

static int force_bounds(const int i) {
  if (i >= ASCII_TABLE_SIZE) {
    return 0;
  } else if (i < 0) {
    return 0;
  }
  return 1;
}

Char_Tables *Chars::get_tbls(void) { return chtbls; }
void Chars::set_table(void) {
  for (int i = 32; i < ASCII_TABLE_SIZE; i++) {
    chtbls[i].c = i;
    const char str[] = {(char)i, '\0'};
    memcpy(chtbls[i].str, str, 2);
  }
}

Char_Tables *Chars::char_lookup(unsigned char c) {
  if (!force_bounds((int)c)) {
    return &chtbls['?'];
  }

  return &chtbls[c];
}
