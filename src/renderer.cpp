#include "../inc/renderer.hpp"
#include "../inc/editor.hpp"
#include "../inc/defines.hpp"
#include "../inc/window.hpp"

#include <SDL2/SDL_render.h>

const int padding = 2;

Renderer::Renderer(SDL_Window *w, const int *width, const int *height)
    : rend(NULL), win_width(width), win_height(height), chars(Chars())
{
    fprintf(stdout, "Renderer instance created\n");
    if(!create_renderer(w)){
        STATE = RNDR_STATE_BAD;
    } else {
        STATE = RNDR_STATE_OK;
    }
}

Renderer::~Renderer(void) {}

void Renderer::renderer_update_viewports(const std::vector<int32_t> *open){
    std::vector<int32_t>::const_iterator it;
    for(it = open->begin(); it != open->end(); ++it){
        const int32_t id = *it;
        std::unordered_map<int32_t, Buffer_Viewport>::iterator it = vps.find(id);
        if(it != vps.end()){
            const int reserve_space = (*chars.ch_max_height() * 2) + (padding * 2);

            const int scaled_height = *win_height - reserve_space;
            const int scaled_width = *win_width * 1.0f;

            it->second.viewport.w = scaled_width;
            it->second.viewport.h = scaled_height;

            it->second.sviewport.y = scaled_height;
            it->second.sviewport.h = reserve_space;
            it->second.sviewport.w = scaled_width;
        } else {
            continue;
        }
    }
}

const Buffer_Viewport* Renderer::renderer_get_viewport(const int id){
    std::unordered_map<int32_t, Buffer_Viewport>::iterator it = vps.find(id);
    if(it != vps.end()){
        return &it->second;
    } else {
        return nullptr;
    }
}
const int Renderer::renderer_set_viewport_row_start(const int id, const int row){}
const int Renderer::renderer_set_viewport_col_start(const int id, int col){}

//This is just a drop in example, I dont have a dedicated method for commands yet.
int Renderer::renderer_draw_status(const int32_t id, const std::string *cmd){
    const Buffer_Viewport *vp = renderer_get_viewport(id);
    if(!vp){
        return RNDR_NO_VP;
    }
    SDL_RenderSetViewport(rend, &vp->sviewport);
    const std::string cmd_construct = "SVTE:" + *cmd;
    renderer_draw_row(&cmd_construct, 0, 0);
    return 1;
}

void Renderer::renderer_create_buffer_viewport(const int32_t id){
    //Reserve two rows for status panel.
    const int reserve_space = (*chars.ch_max_height() * 2) + (padding * 2);

    const int scaled_height = *win_height - reserve_space;
    const int scaled_width = *win_width * 1.0f;

    SDL_Rect main_viewport = {0, 0, scaled_width, scaled_height};
    SDL_Rect status_viewport = {0, scaled_height, scaled_width, reserve_space};

    Buffer_Viewport b_viewport = {main_viewport, status_viewport, 0, 0};
    vps.insert({id, b_viewport});
}

void Renderer::renderer_fill_bg(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void Renderer::renderer_clear(void) { SDL_RenderClear(rend); }

void Renderer::renderer_present(void) { SDL_RenderPresent(rend); }

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

int Renderer::renderer_draw_cursor(const int row, const int col)
{
    const int width = *chars.ch_max_width();
    const int height = *chars.ch_max_height();

    const int x = col * width + padding;
    const int y = row * height + padding;

    SDL_Rect cursor_rect = {x, y, width, height};
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 150);
    SDL_RenderFillRect(rend, &cursor_rect);
    return 1;
}

int Renderer::renderer_draw_char(const int x, const int y, const int w, const int h, SDL_Texture *tex)
{
    SDL_Rect char_rect = {x, y, w, h};
    SDL_RenderCopy(rend, tex, NULL, &char_rect);
    return 1;
}

int Renderer::renderer_draw_file(class Buffer *buf)
{
    const Buffer_Viewport *vp = renderer_get_viewport(buf->buf_get_id());
    if(!vp){
        return RNDR_NO_VP;
    }
    SDL_RenderSetViewport(rend, &vp->viewport);
    
    const int row_height = *chars.ch_max_height();
    int row = 0;
    const std::vector<Line> *buffer = buf->buf_get_buffer();

    std::vector<Line>::const_iterator buf_it = buffer->begin() + vp->curs_row_start_position;
    for (; buf_it != buffer->end(); ++buf_it) {
        const int y = row * row_height + padding;
        if(y >= vp->viewport.h){
            return (row * row_height) + (row * padding);
        }
        renderer_draw_row(&buf_it->str, vp->curs_col_start_position, y);
        row += 1;
    }
    return (row * row_height) + (row * padding);
}

void Renderer::renderer_draw_row(const std::string *str, const int start, const int y){
    const int col_width = *chars.ch_max_width();
    int col = 0;

    std::string::const_iterator str_it = str->begin() + start;
    for (; str_it != str->end(); ++str_it) {
        const char c = *str_it;
        const Char_Table *ct = chars.ch_lookup(c);
        const int x = col * col_width + padding;

        if (*str_it != SPACECHAR) {
            renderer_draw_char(x, y, ct->base.width, ct->base.height, ct->base.t);
        }
        col += 1;
    }
}

