#include "../inc/font.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <cassert>
#include <cstring>

Font font_instance;
Font *get_font_inst(void) { return &font_instance; }

static SDL_Surface *font_surface(TTF_Font *f, const char *buf, SDL_Color col) {
  return TTF_RenderText_Solid(f, buf, col);
}

static SDL_Texture *font_texture(SDL_Renderer *rend, SDL_Surface *s) {
  return SDL_CreateTextureFromSurface(rend, s);
}

Font::Font(void) { set_table(); }

Fonts *Font::get_font(void) { return &fonts; }

TTF_Font *Font::open_font(const char *fn, const int size) {
  TTF_Font *f = TTF_OpenFont(fn, size);
  if (!f) {
    fprintf(stderr, "Failed to open font! -> %s\n", TTF_GetError());
  }
  return f;
}

int Font::table_create_textures(SDL_Renderer *rend) {
  SDL_Color col = {255, 255, 255, 255};
  TTF_Font *fnts[] = {fonts.def, fonts.title};
  for (int i = 32; i < ASCII_TABLE_SIZE; i++) {
    Char_Tables *ct = &chtbls[i];
    Ascii_Char *ascii_buf[] = {&ct->def, &ct->title};

    for (int i = 0; i < 2; i++) {
      SDL_Surface *s = font_surface(fnts[i], ct->str, col);
      ascii_buf[i]->t = font_texture(rend, s);
      ascii_buf[i]->width = s->w, ascii_buf[i]->height = s->h;
      SDL_FreeSurface(s);
    }
  }
  return 1;
}
