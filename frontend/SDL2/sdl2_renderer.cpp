#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

#include "../../include/core/core_buffer.hpp"

#include <iostream>

Renderer::Renderer(SDL_Window *w)
: error(SDL2_NIL), rend(nullptr) {
    rndr_set_err(
        rndr_create_renderer(
            w, 
            SDL_RENDERER_ACCELERATED | 
            SDL_RENDERER_PRESENTVSYNC)
    );
}

int Renderer::rndr_commit_buffer(const int32_t id, const class Buffer *cbuf){
    std::unordered_set<int32_t>::iterator it = used.find(id);
    if(it != used.end()){
        std::cout << "ID already exists" << std::endl;
        return 0;
    }

    class BufRenderer brend(cbuf);
    if(!brend.brndr_get_err()){
        std::cerr << "Passed a NULL pointer using class Buffer" << std::endl;
        return 0;
    }

    used.insert(id);
    renderers.insert({id, brend});
    return 1;
}

void Renderer::rndr_set_err(const int errval){
    error = errval;
}

int Renderer::rndr_get_err(void){
    return error;
}

int Renderer::rndr_create_renderer(SDL_Window *w, const int flags){
    if(!w){
        std::cerr << "Passed a NULL pointer using SDL_Window" << std::endl;
        return SDL2_ERR;
    }

    rndr_set_renderer(SDL_CreateRenderer(w, -1, flags));
    if(!rndr_get_renderer()){
        std::cerr << "Failed to create renderer! -> " << SDL_GetError() << std::endl;
        return SDL2_ERR; 
    }
    return SDL2_NIL;
}

SDL_Renderer *Renderer::rndr_get_renderer(void){
    return rend;
}

void Renderer::rndr_set_renderer(SDL_Renderer *r){
    rend = r;
}

void Renderer::rndr_clear(void){
    SDL_RenderClear(rend);
}

void Renderer::rndr_present(void){
    SDL_RenderPresent(rend);
}

void Renderer::rndr_set_colour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a){
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}


// BUFRENDERER

BufRenderer::BufRenderer(const class Buffer *cbuf) : error(SDL2_NIL) {
    brndr_set_err(brndr_set_buf(cbuf));
}

int BufRenderer::brndr_get_err(void){
    return error;
}

void BufRenderer::brndr_set_err(const int errval){
    error = errval;
}

int BufRenderer::brndr_set_buf(const class Buffer *cbuf){
    return ((constbuf = cbuf) != NULL);
}


