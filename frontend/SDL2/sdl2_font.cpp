#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_error_codes.hpp"

#include <iostream>

int Renderer::rndr_open_font(const std::string path, const int ptsize){
    rndr_set_font(TTF_OpenFont(path.c_str(), ptsize));
    if(!rndr_get_font()){
        std::cerr << "Could not open vector font -> " << SDL_GetError() << std::endl;
        return BAD_FONT;
    }
    return SDL2_NIL;
}

void Renderer::rndr_set_char(void){
    for(unsigned int i = ASCII_MIN; i < ASCII_END; i++){
        ch[i].w = 0;
        ch[i].h = 0;
        ch[i].texture = nullptr;
        ch[i].bad = BAD_TEXTURE;
    }
}

int Renderer::rndr_create_textures(void){
    for(unsigned int i = ASCII_START; i < ASCII_END; i++){
        char c = (char)i;
        const char str[2] = { c, '\0' };
        rndr_create_char_texture(ch[i], rndr_create_char_surface(str));
        if(ch[i].bad != SDL2_NIL){
            return ch[i].bad;
        }
    }
    return SDL2_NIL;
}

SDL_Surface* Renderer::rndr_create_char_surface(const char *str){
    SDL_Color col = {255, 255, 255, 255};
    SDL_Surface *surf = TTF_RenderText_Solid(rndr_font, str, col);
    if(!surf){
        std::cerr << "Failed to create surface -> " << SDL_GetError() << std::endl;
        return nullptr;
    }
    return surf;
}

void Renderer::rndr_create_char_texture(CSprite& sprite, SDL_Surface *surface){
    if(!surface){
        std::cerr << "No workable surface" << std::endl;
        return;
    }

    sprite.w = surface->w;
    sprite.h = surface->h;

    std::cout << surface->w << " " << col_block << std::endl;

    if(surface->w > col_block){
        col_block = surface->w;
        std::cout << "greater"<< std::endl;
    }

    if(surface->h > row_block){
        row_block = surface->h;
    }

    sprite.texture = SDL_CreateTextureFromSurface(rend, surface);
    if(!sprite.texture){
        std::cerr << "Failed to create texture -> " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_FreeSurface(surface);
    sprite.bad = SDL2_NIL;
}

const CSprite& Renderer::rndr_index_texture(const unsigned char c){
    const unsigned char access = c % ASCII_MAX;
    if(access >= ASCII_START && access < ASCII_MAX){
        return ch[access];
    } else {
        return ch['?'];
    }
}