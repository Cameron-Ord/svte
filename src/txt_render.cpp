#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/vecdef.hpp"

FontRenderer::FontRenderer(SDL_Renderer *r, const Vec2i *dimensions)
    : rend(NULL), dim(dimensions) {
  fprintf(stdout, "Font renderer instance created\n");
}

void FontRenderer::frender_set_renderer(SDL_Renderer *r) { rend = r; }

void FontRenderer::put_cursor(SDL_Rect *curs_rect) {
  SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
  SDL_RenderFillRect(rend, curs_rect);
}

void FontRenderer::render_buffer(const Buf *buf, Chars *ch) {
  const unsigned char nl = '\n';
  const Vec2i *d = ch->get_char_dims();
  int rowy = 0, colx = 0;

  for (size_t i = 0; i < buf->size; i++) {
    if (buf->buf[i] == nl) {
      colx = 0;
      rowy += 1;
      continue;
    }

    Char_Tables *ct = ch->char_lookup(buf->buf[i]);

    const int x = colx * d->x;
    const int y = rowy * d->y;

    SDL_Rect char_rect = {x, y, ct->def.width, ct->def.height};
    SDL_RenderCopy(rend, ct->def.t, NULL, &char_rect);

    if (i == buf->pos) {
      SDL_Rect curs_rect = {x, y, ct->def.width, ct->def.height};
      put_cursor(&curs_rect);
    }

    colx += 1;
  }
}

FontRenderer::~FontRenderer(void) {}
