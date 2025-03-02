#include "../inc/renderer.hpp"

#include <cstdio>
#include <cstdlib>

#include <SDL2/SDL_render.h>

Renderer renderer_instance;

Renderer::~Renderer(void) {}

void Renderer::bg_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void Renderer::clear_render(void) { SDL_RenderClear(rend); }

void Renderer::render_present(void) { SDL_RenderPresent(rend); }

SDL_Renderer *Renderer::get_renderer(void) { return renderer_instance.rend; }

const void *Renderer::create_renderer(SDL_Window *w) {
  const int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  rend = SDL_CreateRenderer(w, -1, flags);
  if (!rend) {
    fprintf(stderr, "Failed to create renderer! -> %s\n", SDL_GetError());
    return NULL;
  }
  return rend;
}

Renderer *get_renderer_inst(void) { return &renderer_instance; }
