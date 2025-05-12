#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_error_codes.hpp"

#include <iostream>

const uint8_t ZERO_START = 0;
const uint8_t ASCII_START = 32;
const uint8_t ASCII_END = 128;

int Renderer::rndr_open_font(const std::string path, const int ptsize){
    rndr_set_font(TTF_OpenFont(path.c_str(), ptsize));
    if(!rndr_get_font()){
        std::cerr << "Could not open vector font -> " << SDL_GetError() << std::endl;
        return BAD_FONT;
    }
    return SDL2_NIL;
}

void Renderer::rndr_set_char(void){
    for(unsigned int i = ZERO_START; i < ASCII_END; i++){
        ch[i] = nullptr;
    }
}

int Renderer::rndr_create_textures(void){
    for(unsigned int i = ASCII_START; i < ASCII_END; i++){
        char c = (char)i;
        const char str[2] = { c, '\0' };
        ch[i] = rndr_create_char_texture(rndr_create_char_surface(str));
        if(!ch[i]){
            return BAD_TEXTURE;
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

SDL_Texture* Renderer::rndr_create_char_texture(SDL_Surface *surface){
    if(!surface){
        std::cerr << "No workable surface" << std::endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surface);
    if(!texture){
        std::cerr << "Failed to create texture -> " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return nullptr;
    }

    SDL_FreeSurface(surface);
    return texture;
}