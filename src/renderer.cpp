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
        STATE = RDR_STATE_BAD;
    } else {
        STATE = RDR_STATE_OK;
    }
}

Renderer::~Renderer(void) {}

const Buffer_Viewport* Renderer::renderer_get_viewport(const int id){}
const int Renderer::renderer_set_viewport_row_start(const int id, const int row){}
const int Renderer::renderer_set_viewport_col_start(const int id, int col){}

void Renderer::renderer_create_buffer_viewport(const int32_t id){
    Buffer_Viewport b_viewport = {(SDL_Rect){0, 0, *win_width, *win_height}, 0, 0};
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
    const int x = col * max_width + padding;
    const int y = row * max_height + padding;

    SDL_Rect cursor_rect = {x, y, max_width, max_height};
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
    int row = 0, row_height = *ch->ch_max_height(), col_width = *ch->ch_max_width();

    const std::vector<Line> *buffer = buf->buf_get_buffer();
    for (size_t i = 0; i < buffer->size(); i++) {
        std::string str = (*buffer)[i].str;
        int col = 0;
        const int y = row * row_height + padding;

        for (size_t t = 0; t < str.size(); t++) {
            const char c = str[t];
            const Char_Table *ct = ch->ch_lookup(c);
            const int x = col * col_width + padding;

            if (str[t] != SPACECHAR) {
                renderer_draw_char(x, y, ct->base.width, ct->base.height, ct->base.t);
            }
            col += 1;
        }
        row += 1;
    }
    return (row * row_height) + (row * padding);
}
