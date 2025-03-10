#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/vecdef.hpp"

FontRenderer::FontRenderer(SDL_Renderer *r, const Vec2i *dimensions)
    : rend(NULL), dim(dimensions) {
  fprintf(stdout, "Font renderer instance created\n");
}

void FontRenderer::frender_set_renderer(SDL_Renderer *r) { rend = r; }

void FontRenderer::render_buffer(const Buf *buf, Chars *ch) {
  const unsigned char nl = '\n';
  const Vec2i *d = ch->get_char_dims();
  int rowy = 0;

  for (size_t i = 0; i < buf->lcount; i++) {
    String str = buf->lines[i];
    int colx = 0;
    for (size_t j = 0; j < str.len; j++) {
      if (str.str[j] == nl) {
        continue;
      }
      Char_Tables *ct = ch->char_lookup(str.str[j]);

      const int y = d->y * rowy; // + 2
      const int x = d->x * colx; // + 2

      SDL_Rect char_rect = {x, y, ct->def.width, ct->def.height};
      SDL_RenderCopy(rend, ct->def.t, NULL, &char_rect);
      colx += 1;
    }
    rowy += 1;
  }
}

FontRenderer::~FontRenderer(void) {}
