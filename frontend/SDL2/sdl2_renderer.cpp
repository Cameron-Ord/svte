#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_error_codes.hpp"


Renderer::Renderer(SDL_Window *w, const int* const width, const int* const height) 
    : error(SDL2_NIL), rend(nullptr), rndr_font(nullptr), _width(width), _height(height) {
    rndr_set_err(rndr_create_renderer(w, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    rndr_set_err(rndr_open_font("dogicapixel.ttf", 16));
    rndr_set_char();
    rndr_set_err(rndr_create_textures());
}

Renderer::~Renderer(void){}

TTF_Font *Renderer::rndr_get_font(void){
    return rndr_font;
}

void Renderer::rndr_set_font(TTF_Font *f){
    rndr_font = f;
}

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
    return SDL2_NIL;
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
