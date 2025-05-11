#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_error_codes.hpp"


Renderer::Renderer(SDL_Window *w, const int* const width, const int* const height) 
    : rend(nullptr), _width(width), _height(height), error(NIL) {
    rndr_set_err(rndr_create_renderer(w, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
}

Renderer::~Renderer(void){}

SDL_Renderer *Renderer::rndr_get_renderer(void){
    return rend;
}

int Renderer::rndr_get_err(void){
    return error;
}

void Renderer::rndr_set_err(const int err){
    error = err;
}

int Renderer::rndr_create_renderer(SDL_Window *w, const int flags){
    rend = SDL_CreateRenderer(w, -1, flags);
    if(!rend){
        return BAD_RENDERER;
    }
    return NIL;
}

void Renderer::rndr_fill_bg(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a){
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void Renderer::rndr_clear(void){
    SDL_RenderClear(rend);
}

void Renderer::rndr_present(void){
    SDL_RenderPresent(rend);
}
