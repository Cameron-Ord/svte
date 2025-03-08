#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/vecdef.hpp"

FontRenderer::FontRenderer(SDL_Renderer *r, const Vec2i *dimensions)
    : rend(NULL), dim(dimensions) {
  fprintf(stdout, "Font renderer instance created\n");
}

void FontRenderer::frender_set_renderer(SDL_Renderer *r) { rend = r; }
void FontRenderer::frender_set_buf(const Buf *b, Chars *c) {
  const unsigned char nl = '\n';
  const int padding = 2;

  int x = 0, y = 0;
  int i = 0;
  while (b->buf[i] != '\0') {
    unsigned char ch = b->buf[i];

    if (ch == nl) {
      x = 0;
      y += 12;
    }

    if (ch != nl) {
      Char_Tables *ct = c->char_lookup(ch);
      SDL_Rect ch_rect = {x, y, ct->def.width, ct->def.height};
      SDL_RenderCopy(rend, ct->def.t, NULL, &ch_rect);

      x += ct->def.width + padding;
    }

    i++;
  }
}

FontRenderer::~FontRenderer(void) {}
