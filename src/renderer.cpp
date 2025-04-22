#include "../inc/renderer.hpp"
#include "../inc/font.hpp"
#include "../inc/window.hpp"
#include "../inc/editor.hpp"
#include "../inc/globaldef.hpp"

#include <cstdio>
#include <cstdlib>
#include <SDL2/SDL_render.h>
#include <iostream>
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

int Renderer::render_buffer(class Buffer *buf, class Chars *ch){
    int row = 0, row_height = *ch->ch_max_height();
    int col_width = *ch->ch_max_width(), padding = 2;

    std::vector<std::string> *buffer = buf->buf_get_buffer();
    for(size_t i = 0; i < buffer->size(); i++){
        std::string str = (*buffer)[i];
        const int y = row * row_height + padding;

        int col = 0;
        int xaccumulate = 0;
        
        for(size_t t = 0; t < str.size(); t++){
            const char c = str[t];
            const Char_Table * ct = ch->char_lookup(c);
            const int x = xaccumulate;

            if(str[t] != SPACECHAR){
                SDL_Rect char_rect = {x, y, ct->base.width, ct->base.height};
                SDL_RenderCopy(rend, ct->base.t, NULL, &char_rect);
            }

            const Cursor *curs = buf->buf_get_curs();
            if(curs->col == col && curs->row == row){
                SDL_Rect cursor_rect = {x, y, ct->base.width, ct->base.height};
                SDL_SetRenderDrawColor(rend, 255, 255, 255, 150);
                SDL_RenderFillRect(rend, &cursor_rect);
            }

            xaccumulate += ct->base.width + padding;
            col += 1;
        }
        row += 1;
    }
    return (row * row_height) + (row * padding);   
}
