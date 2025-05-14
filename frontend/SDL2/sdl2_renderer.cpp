#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

#include <iostream>


Renderer::Renderer(SDL_Window *w, const int* const width, const int* const height) 
    : error(SDL2_NIL), vertical_padding(2), horizontal_padding(2), row_block(0), col_block(0), rend(nullptr), rndr_font(nullptr), _width(width), _height(height), ch(nullptr) {
    rndr_set_err(rndr_create_renderer(w, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    rndr_set_err(rndr_alloc_sprite_array(ASCII_MAX));
    rndr_set_err(rndr_open_font("dogicapixel.ttf", 16));
    rndr_set_char();
    rndr_set_err(rndr_create_textures());
}

Renderer::~Renderer(void){}

int Renderer::rndr_alloc_sprite_array(const size_t size){
    ch = new CSprite[size];
    if(!ch){
        return BAD_ALLOC;
    }
    return SDL2_NIL;
}

void Renderer::rndr_dealloc_sprite_array(void){
    if(ch){
        delete[] ch;
    }
}

void Renderer::rndr_set_vertical_padding(const int val){
    vertical_padding = val;
}

void Renderer::rndr_set_horizontal_padding(const int val){
    horizontal_padding = val;
}

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

int Renderer::rndr_insert_pos(const int id){
    std::unordered_set<int32_t>::iterator it = used.find(id);
    if(it != used.end()){
        std::cout << "ID Already exists.. {rndr offset map}" << std::endl;
        return 0;
    }

    used.insert(id);
    StartPos init = {0, 0};
    offsets.insert({id, init});
    return 1; 
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

int Renderer::rndr_eval_pos(const int id, const int rrow, const int rcol){
    return 1;
}


int Renderer::rndr_do_pipeline(std::vector<std::string>::const_iterator it, std::vector<std::string>::const_iterator end, const int row, const int col){
    rndr_draw_buffer(it, end);
    rndr_draw_cursor(row, col);
    return 1;
}

void Renderer::rndr_draw_cursor(const int& row, const int& col){
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    const int y = row * row_block + vertical_padding;
    const int x = col * col_block + horizontal_padding;
    SDL_Rect crect = {x, y, row_block, col_block};
    SDL_RenderFillRect(rend, &crect);
}


void Renderer::rndr_draw_buffer(std::vector<std::string>::const_iterator it, std::vector<std::string>::const_iterator end){
    SDL_RenderSetViewport(rend, NULL);
    int row = 0;
    
    for(; it != end; ++it){
        const int y = row * row_block + vertical_padding;
        rndr_draw_line(it->begin(), it->end(), y);
        row+=1;
    }
}

void Renderer::rndr_draw_line(std::string::const_iterator it, std::string::const_iterator end, const int& y){
    int col = 0;
    const unsigned char skipchar = ' ';

    for(; it != end; ++it){
        const unsigned char c = *it;
        const CSprite& spr = rndr_index_texture(c); 
        const int x = col * col_block + horizontal_padding;

        if(c != skipchar){
            rndr_draw_char(x, y, spr.w, spr.h, spr.texture);
        }
        col+=1;
    }
}

void Renderer::rndr_draw_char(const int& x, const int& y, const int& w, const int& h, SDL_Texture *texture){
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(rend, texture, NULL, &rect);  
}
