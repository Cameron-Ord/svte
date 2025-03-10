#include "../inc/renderer.hpp"
#include "../inc/font.hpp"
#include "../inc/window.hpp"

#include <cstdio>
#include <cstdlib>

FontRenderer Renderer::create_font_renderer(void) {
  return FontRenderer(NULL, dim);
}

Renderer::Renderer(Window *w)
    : rend(NULL), dim(w->get_dimensions()), frender(create_font_renderer()) {
  fprintf(stdout, "Renderer instance created\n");
}

Renderer::~Renderer(void) {}

FontRenderer *Renderer::_frender(void) { return &frender; }

void Renderer::bg_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void Renderer::clear_render(void) { SDL_RenderClear(rend); }

void Renderer::render_present(void) { SDL_RenderPresent(rend); }

SDL_Renderer *Renderer::get_renderer(void) { return rend; }

const void *Renderer::create_renderer(SDL_Window *w) {
  const int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  rend = SDL_CreateRenderer(w, -1, flags);
  if (!rend) {
    fprintf(stderr, "Failed to create renderer! -> %s\n", SDL_GetError());
    return NULL;
  }

  SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
  return rend;
}

void Renderer::render_cursor(const Vec2i *curs, Chars *ch) {
  const Vec2i *d = ch->get_char_dims();
  SDL_Rect curs_rect = {curs->x * d->x, curs->y * d->y, d->x, d->y};
  SDL_SetRenderDrawColor(rend, 255, 255, 255, 255 / 2);
  SDL_RenderFillRect(rend, &curs_rect);
}
