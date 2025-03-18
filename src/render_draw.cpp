#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/vecdef.hpp"

#ifndef NULLCHAR
#define NULLCHAR '\0'
#endif

#ifndef NEWLINE
#define NEWLINE '\n'
#endif

FontRenderer::FontRenderer(SDL_Renderer *r, const Vec2i *dimensions)
    : rend(NULL), dim(dimensions) {
  fprintf(stdout, "Font renderer instance created\n");
}

void FontRenderer::frender_set_renderer(SDL_Renderer *r) { rend = r; }

void FontRenderer::render_curs(const Buf *buf, const Vec2i *dims) {
  const int pad = 4;
  int rowy = 0, colx = 0;

  for (size_t i = 0; i <= buf->size; i++) {
    const int x = (colx * dims->x) + pad;
    const int y = (rowy * dims->y) + pad;

    const int is_nl = buf->buf[i] == NEWLINE;
    switch(is_nl){
        case 0:{
            colx += 1;
        }break;
        case 1:{
            colx = 0;
            rowy += 1;
        }break;
    }

    if (i == (size_t)buf->pos) {
        SDL_Rect curs_rect = {x, y, dims->x, dims->y};
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderFillRect(rend, &curs_rect);
        return;
    }
  }
}

int FontRenderer::render_buffer(const Buf *buf, Chars *ch) {
    const int padding = 2;
    const int m_ch_width = ch->get_char_dims()->x, m_ch_height = ch->get_char_dims()->y;
    int rowy = 0, colx = 0;

    for(size_t i = 0; i < buf->size && buf->buf[i] != NULLCHAR; i++){
        const int x = (colx * m_ch_width) + padding;
        const int y = (rowy * m_ch_height) + padding;
            
        const int is_nl = buf->buf[i] == NEWLINE;
        switch(is_nl){
            default:break;
            case 0:{
                Char_Tables *ct = ch->char_lookup(buf->buf[i]);
                SDL_Rect char_rect = {x, y, ct->def.width, ct->def.height};
                SDL_RenderCopy(rend, ct->def.t, NULL, &char_rect);
                colx += 1;
            }break;
            case 1:{
                colx = 0;
                rowy += 1;
            }break;
        }
    }
    return rowy * m_ch_height;
}

FontRenderer::~FontRenderer(void) {}
