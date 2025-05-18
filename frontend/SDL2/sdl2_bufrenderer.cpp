#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

#include "../../include/core/core_buffer.hpp"

#include <iostream>

// BUFRENDERER

BufRenderer::BufRenderer(const class Buffer *cbuf, const int width, const int height) : 
error(SDL2_NIL), vertical_padding(2), horizontal_padding(2), 
col_offset(0), row_offset(0), constbuf(nullptr), 
thresholds(Thresholds(width, height, 0.1, 0.9, 0.1, 0.9))
{
    br_set_err(br_set_buf(cbuf));
    br_set_viewport_dims(width, height);
    br_set_viewport_pos(0, 0);
}

int BufRenderer::br_getx(const int col, const int block){
    return col * (block + horizontal_padding);
}

int BufRenderer::br_gety(const int row, const int block){
    return row * (block + vertical_padding);
}

void BufRenderer::br_update_offsets(const int row_block, const int col_block){
    const int arow = constbuf->buf_get_row();
    const int acol = constbuf->buf_get_col();
    const int line_size = constbuf->buf_get_line_size(arow);
    const int buf_size = constbuf->buf_get_size();


    while(br_gety(arow - row_offset, row_block) < thresholds.h_th_min && row_offset > 0){
        row_offset--;
    }

    while(br_gety(arow - row_offset, row_block) > thresholds.h_th_max && row_offset < buf_size){
        row_offset++;
    }

    while(br_getx(acol - col_offset, col_block) < thresholds.w_th_min && col_offset > 0){
        col_offset--;
    }

    while(br_getx(acol - col_offset, col_block) > thresholds.w_th_max && line_size > 0 && col_offset <= line_size){
        col_offset++;
    }
}

void BufRenderer::br_draw_buffer(
    SDL_Renderer *rend,
    const class VectorFont *vf)
{
    ConstBufRowIt rows(*constbuf->buf_get_buffer());
    rows.offset(row_offset).valid();

    int row = 0;
    for (; rows.begin != rows.end; ++rows.begin) {
        const int y = br_gety(row, vf->vec_row_block());

        if(y > viewport.h){
            return;
        }

        ConstBufStrIt line(*rows.begin); 
        line.offset(col_offset).valid();

        br_draw_line(rend, vf, line, y);
        row += 1;
    }
}

void BufRenderer::br_draw_line(
    SDL_Renderer *rend,
    const class VectorFont *vf,
    ConstBufStrIt& row,
    const int y)
{
    int col = 0;
    const unsigned char skipchar = ' ';

    for (; row.begin != row.end; ++row.begin) {
        const unsigned char c = *row.begin;
        const CSprite &spr = vf->vec_index_texture(c);
        const int x = br_getx(col, vf->vec_col_block());

        if(x > viewport.w){
            return;
        }

        if (c != skipchar) {
            br_put_char(rend, x, y, spr.w, spr.h, spr.texture);
        }
        col += 1;
    }
}

void BufRenderer::br_put_char(SDL_Renderer *rend, const int x, const int y, const int w, const int h, SDL_Texture *t)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(rend, t, NULL, &rect);
}

void BufRenderer::br_put_cursor(SDL_Renderer *rend, const int width, const int height)
{
    const int x = br_getx(constbuf->buf_get_col() - col_offset, width);
    const int y = br_gety(constbuf->buf_get_row() - row_offset, height);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(rend, &rect);
}

void BufRenderer::br_draw_selection(SDL_Renderer *rend, const int rblock, const int cblock){
    ConstBufRowIt(*constbuf->buf_get_buffer());
}

void BufRenderer::br_selection_line(SDL_Renderer* rend, const int rblock, const int cblock){

}
