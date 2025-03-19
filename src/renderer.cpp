#include "../inc/renderer.hpp"
#include "../inc/font.hpp"
#include "../inc/window.hpp"

#include <cstdio>
#include <cstdlib>

Renderer::Renderer()
    : rend(NULL)
{
    fprintf(stdout, "Renderer instance created\n");
}

Renderer::~Renderer(void) {}

void Renderer::bg_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void Renderer::clear_render(void) { SDL_RenderClear(rend); }

void Renderer::render_present(void) { SDL_RenderPresent(rend); }

SDL_Renderer *Renderer::get_renderer(void) { return rend; }

const void *Renderer::create_renderer(SDL_Window *w)
{
    const int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    rend = SDL_CreateRenderer(w, -1, flags);
    if (!rend) {
        fprintf(stderr, "Failed to create renderer! -> %s\n", SDL_GetError());
        return NULL;
    }

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    return rend;
}
