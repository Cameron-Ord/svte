#include "../inc/font.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

Font font_instance;
Font *get_font_inst(void) { return &font_instance; }

static SDL_Surface *font_surface(TTF_Font *f, const char *buf, SDL_Color col) {
  return TTF_RenderText_Solid(f, buf, col);
}

static SDL_Texture *font_texture(SDL_Renderer *rend, SDL_Surface *s) {
  return SDL_CreateTextureFromSurface(rend, s);
}

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

int Font::table_create_textures(SDL_Renderer *rend) {
  SDL_Color col = {255, 255, 255, 255};
  for (int i = 32; i < ASCII_TABLE_SIZE; i++) {
    SDL_Surface *s = font_surface(f, tbl[i].str, col);
    SDL_Texture *t = font_texture(rend, s);
    tbl[i].width = s->w, tbl[i].height = s->h, tbl[i].t = t;
    SDL_FreeSurface(s);
  }
  return 1;
}

void Font::set_table(void) {
  for (int i = 32; i < ASCII_TABLE_SIZE; i++) {
    tbl[i].c = i;
    const char str[] = {(char)i, '\0'};
    memcpy(tbl[i].str, str, 2);
  }
}
