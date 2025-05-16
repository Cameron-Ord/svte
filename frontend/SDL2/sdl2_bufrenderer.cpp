#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

#include "../../include/core/core_buffer.hpp"

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

void BufRenderer::br_update_offsets(const int row_block, const int col_block){
    //actual <> interpreted
    const int arow = constbuf->buf_get_row();
    const int acol = constbuf->buf_get_col();

    const int irow = arow - row_offset;
    const int icol = acol - col_offset;

    const int y = irow * row_block + vertical_padding;
    const int x = icol * col_block + horizontal_padding;

    if(y < thresholds.h_th_min && row_offset > 0){
        row_offset -= 1;
    } else if (y > thresholds.h_th_max && row_offset < constbuf->buf_get_size()){
        row_offset += 1;
    }


    if(x < thresholds.w_th_min && col_offset > 0){
        col_offset -= 1;
    } else if(x > thresholds.h_th_max){
        const int strsize = constbuf->buf_get_line_size(arow);
        if(strsize > 0 && col_offset < strsize){
            col_offset += 1;
        }
    }
}



void BufRenderer::br_draw_buffer(
    SDL_Renderer *rend,
    const class VectorFont *vfont)
{
    ConstRangeVecStr rows(
        constbuf->buf_row_end_const(),
        constbuf->buf_row_begin_const() + row_offset
    );

    int row = 0;
    for (; rows.begin != rows.end; ++rows.begin) {
        const int y = row * vfont->vec_row_block() + vertical_padding;

        if(y > viewport.h){
            return;
        }

        br_draw_line(
            rend,
            vfont,
            ConstRangeStr(
                rows.begin->end(),
                rows.begin->begin() + col_offset
            ),
            y
        );
        row += 1;
    }
}

void BufRenderer::br_draw_line(
    SDL_Renderer *rend,
    const class VectorFont *vfont,
    ConstRangeStr row,
    const int y)
{
    int col = 0;
    const unsigned char skipchar = ' ';

    for (; row.begin != row.end; ++row.begin) {
        const unsigned char c = *row.begin;
        const CSprite &spr = vfont->vec_index_texture(c);
        const int x = col * vfont->vec_col_block() + horizontal_padding;

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
    const int icol = constbuf->buf_get_col() - col_offset;
    const int irow = constbuf->buf_get_row() - row_offset;
    const int x = icol * width + horizontal_padding;
    const int y = irow * height + vertical_padding;

    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(rend, &rect);
}



void BufRenderer::br_set_thresholds(Thresholds th){
    thresholds = th;
}

void BufRenderer::br_set_col_offset(const int val){
    col_offset = val;
}

void BufRenderer::br_set_row_offset(const int val){
    row_offset = val;
}

int BufRenderer::br_row_offset(void)
{
    return row_offset;
}

int BufRenderer::br_col_offset(void)
{
    return col_offset;
}

const SDL_Rect *BufRenderer::br_get_viewport(void)
{
    return &viewport;
}

void BufRenderer::br_set_viewport_pos(const int x, const int y)
{
    viewport.x = x, viewport.y = y;
}

void BufRenderer::br_set_viewport_dims(const int width, const int height)
{
    viewport.w = width, viewport.h = height;
}

int BufRenderer::br_valid_ptr(void)
{
    return constbuf != NULL;
}

int BufRenderer::br_get_err(void)
{
    return error;
}

void BufRenderer::br_set_err(const int errval)
{
    error = errval;
}

int BufRenderer::br_set_buf(const class Buffer *cbuf)
{
    return ((constbuf = cbuf) != NULL);
}

std::vector<std::string>::const_iterator BufRenderer::br_get_row_start(void)
{
    return constbuf->buf_row_begin_const();
}

std::vector<std::string>::const_iterator BufRenderer::br_get_row_end(void)
{
    return constbuf->buf_row_end_const();
}