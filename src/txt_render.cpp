#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/vecdef.hpp"

#include <iostream>

#ifndef NULLCHAR
#define NULLCHAR '\0'
#endif

FontRenderer::FontRenderer(SDL_Renderer *r, const Vec2i *dimensions)
    : rend(NULL), dim(dimensions) {
  fprintf(stdout, "Font renderer instance created\n");
}

void FontRenderer::frender_set_renderer(SDL_Renderer *r) { rend = r; }

void FontRenderer::render_curs(const Buf *buf, const Vec2i *dims) {
  const int pad = 4;
  int rowy = 0, colx = 0;

  for (size_t i = 0; i <= buf->size; i++) {
    const int x = (colx * dims->x) + pad;
    const int y = (rowy * dims->y) + pad;

    if (buf->buf[i] != NEWLINE) {
      colx += 1;
    } else if (buf->buf[i] == NEWLINE) {
      colx = 0;
      rowy += 1;
    }

    if (i == buf->pos) {
      SDL_Rect curs_rect = {x, y, dims->x, dims->y};
      SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
      SDL_RenderFillRect(rend, &curs_rect);
      return;
    }
  }
}

void FontRenderer::render_buffer(const Buf *buf, Chars *ch) {
  const int pad = 4;
  const Vec2i *d = ch->get_char_dims();
  int rowy = 0, colx = 0;

  for (size_t i = 0; i < buf->size && buf->buf[i] != NULLCHAR; i++) {
    const int x = (colx * d->x) + pad;
    const int y = (rowy * d->y) + pad;

    if (buf->buf[i] != NEWLINE) {
      Char_Tables *ct = ch->char_lookup(buf->buf[i]);
      SDL_Rect char_rect = {x, y, ct->def.width, ct->def.height};
      SDL_RenderCopy(rend, ct->def.t, NULL, &char_rect);
      colx += 1;
    }

    if (buf->buf[i] == NEWLINE) {
      colx = 0;
      rowy += 1;
    }
  }
}

FontRenderer::~FontRenderer(void) {}
