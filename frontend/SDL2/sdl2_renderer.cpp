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

//At some point maybe support multiple buffers but right now I just want to make what I have good first
//so just only support one buffer displaying at a time for the size of window. Can still switch between them though.
void Renderer::rndr_update_viewports(
    const std::vector<int32_t>& open,
    const int width, const int height
){
    for(size_t i = 0; i < open.size(); i++){
        const int32_t id = open[i];
        class BufRenderer *b = rndr_grab_bufrenderer(id);
        if(b){
            b->br_set_viewport_dims(width, height);
        }
    }
}

int Renderer::rndr_commit_buffer(const int32_t id, const class Buffer *cbuf, const int width, const int height){
    std::unordered_set<int32_t>::iterator it = used.find(id);
    if(it != used.end()){
        std::cout << "ID already exists" << std::endl;
        return 0;
    }

    class BufRenderer brend(cbuf, width, height);
    if(!brend.br_get_err()){
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

void Renderer::rndr_draw_id(const int32_t id, const class VectorFont* vfont){
    class BufRenderer *br = rndr_grab_bufrenderer(id);
    if(br && br->br_valid_ptr()){
        br->br_draw_buffer(rend, vfont, br->br_get_row_start(), br->br_get_row_end());
    }
}

class BufRenderer *Renderer::rndr_grab_bufrenderer(const int32_t id){
    std::unordered_map<int32_t, class BufRenderer>::iterator it = renderers.find(id);
    if(it != renderers.end()){
        return &it->second;
    } else {
        return nullptr;
    }
}

// BUFRENDERER

BufRenderer::BufRenderer(const class Buffer *cbuf, const int width, const int height) :
error(SDL2_NIL), vertical_padding(2), horizontal_padding(2), constbuf(nullptr) {
    br_set_err(br_set_buf(cbuf));
    br_set_viewport_dims(width, height);
    br_set_viewport_pos(0, 0);
}

void BufRenderer::br_set_viewport_pos(const int x, const int y){
    viewport.x = x, viewport.y = y;
}

void BufRenderer::br_set_viewport_dims(const int width, const int height){
    viewport.w = width, viewport.h = height;
}

int BufRenderer::br_valid_ptr(void){
    return constbuf != NULL;
}

int BufRenderer::br_get_err(void){
    return error;
}

void BufRenderer::br_set_err(const int errval){
    error = errval;
}

int BufRenderer::br_set_buf(const class Buffer *cbuf){
    return ((constbuf = cbuf) != NULL);
}

std::vector<std::string>::const_iterator BufRenderer::br_get_row_start(void){
    return constbuf->buf_row_begin_const();
}

std::vector<std::string>::const_iterator BufRenderer::br_get_row_end(void){
    return constbuf->buf_row_end_const();
}


void BufRenderer::br_draw_buffer(
    SDL_Renderer *rend, const class VectorFont* vfont,
    std::vector<std::string>::const_iterator it, 
    std::vector<std::string>::const_iterator end
){
    SDL_RenderSetViewport(rend, &viewport);
    int row = 0;
    
    for(; it != end; ++it){
        const int y = row * vfont->vec_row_block() + vertical_padding;
        br_draw_line(rend, vfont, it->begin(), it->end(), y);
        row+=1;
    }
}

void BufRenderer::br_draw_line(
    SDL_Renderer *rend, 
    const class VectorFont* vfont, 
    std::string::const_iterator it, 
    std::string::const_iterator end, 
    const int y
){
    int col = 0;
    const unsigned char skipchar = ' ';

    for(; it != end; ++it){
        const unsigned char c = *it;
        const CSprite& spr = vfont->vec_index_texture(c);
        const int x = col * vfont->vec_col_block() + horizontal_padding;

        if(c != skipchar){
            br_put_char(rend, x, y, spr.w, spr.h, spr.texture);
        }
        col+=1;
    }
}
void BufRenderer::br_put_char(SDL_Renderer *rend, const int x, const int y, const int w, const int h, SDL_Texture *t){
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(rend, t, NULL, &rect);  
}


