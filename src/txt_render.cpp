#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/vecdef.hpp"

#include <iostream>

FontRenderer::FontRenderer(SDL_Renderer *r, const Vec2i *dimensions)
    : rend(NULL), dim(dimensions) {
  fprintf(stdout, "Font renderer instance created\n");
}

void FontRenderer::frender_set_renderer(SDL_Renderer *r) { rend = r; }

void FontRenderer::put_cursor(SDL_Rect *curs_rect) {
  SDL_SetRenderDrawColor(rend, 255, 255, 255, 225);
  SDL_RenderFillRect(rend, curs_rect);
}

void FontRenderer::render_buffer(const Buf *buf, Chars *ch) {
  const unsigned char nl = '\n';
  const int pad = 4;
  const Vec2i *d = ch->get_char_dims();
  int rowy = 0, colx = 0;

  for (size_t i = 0; i < buf->size && buf->buf[i] != '\0'; i++) {
    const int x = (colx * d->x) + pad;
    const int y = (rowy * d->y) + pad;

    if (buf->buf[i] != nl && i != buf->size - 1) {
      Char_Tables *ct = ch->char_lookup(buf->buf[i]);
      SDL_Rect char_rect = {x, y, ct->def.width, ct->def.height};
      SDL_RenderCopy(rend, ct->def.t, NULL, &char_rect);
      colx += 1;
    }

    if (i == buf->pos) {
      SDL_Rect curs_rect = {x, y, d->x, d->y};
      put_cursor(&curs_rect);
    }

    if (buf->buf[i] == nl) {
      colx = 0;
      rowy += 1;
    }
  }
}

FontRenderer::~FontRenderer(void) {}
