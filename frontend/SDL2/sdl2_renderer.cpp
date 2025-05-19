#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"


#include <iostream>

Renderer::Renderer(SDL_Window *w, const Editor* const edptr)
    : error(SDL2_NIL), rend(rndr_create_renderer(w, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)), vf(rndr_get_renderer()), ed(edptr)
{
    rndr_set_err((rend != nullptr) ? SDL2_NIL : SDL2_ERR);
    rndr_set_err((edptr != nullptr) ? SDL2_NIL : SDL2_ERR);
}

void Renderer::rndr_set_blendmode(SDL_BlendMode mode){
    SDL_SetRenderDrawBlendMode(rend, mode);
}

//At some point maybe support multiple buffers but right now I just want to make what I have good first
//so just only support one buffer displaying at a time for the size of window. Can still switch between them though.
void Renderer::rndr_update_viewports(const int width, const int height)
{
    for (size_t i = 0; i < commited_ids.size(); i++) {
        std::unordered_map<int32_t, RndrItem>::iterator it = rndrbuffers.find(commited_ids[i]);
        if (it != rndrbuffers.end()) {
            it->second.vp_update_w(width).vp_update_h(height).th_update(
                RndrThreshold(width, height, 0.1, 0.9, 0.1, 0.9)
            );
        }
    }
}

int Renderer::rndr_commit_buffer(const int32_t id, const int width, const int height)
{
    if(id < 0){
        return 0;
    }

    std::unordered_set<int32_t>::iterator it = used.find(id);
    if (it != used.end()) {
        std::cout << "ID already exists" << std::endl;
        return 0;
    }

    RndrItem item(width, height);
    used.insert(id);
    rndrbuffers.insert({id, item});
    commited_ids.push_back(id);

    return 1;
}

SDL_Renderer *Renderer::rndr_create_renderer(SDL_Window *w, const int flags)
{
    if (!w) {
        std::cerr << "Passed a NULL pointer using SDL_Window" << std::endl;
        return nullptr;
    }

    rndr_set_renderer(SDL_CreateRenderer(w, -1, flags));
    if (!rndr_get_renderer()) {
        std::cerr << "Failed to create renderer! -> " << SDL_GetError() << std::endl;
        return nullptr;
    }
    return rndr_get_renderer();
}

void Renderer::rndr_set_viewport(const SDL_Rect *vp_rect)
{
    if (vp_rect) {
        SDL_RenderSetViewport(rend, vp_rect);
    } else {
        SDL_RenderSetViewport(rend, NULL);
    }
}

void Renderer::rndr_draw_id(const int32_t id)
{
    std::unordered_map<int32_t, RndrItem>::iterator it = rndrbuffers.find(id);
    const  Buffer* const b = ed->ed_fetch_buffer_const(id);
    if (it != rndrbuffers.end() && b != nullptr) {
        rndr_set_viewport(&it->second.viewport);
        rndr_draw_buffer(it->second, b);
        rndr_set_colour(255, 255, 255, 125);
        rndr_put_cursor(it->second, b->buf_get_row(), b->buf_get_col());
    }
}

void Renderer::rndr_update_offsets_by_id(const int32_t id)
{
    std::unordered_map<int32_t, RndrItem>::iterator it = rndrbuffers.find(id);
    const  Buffer* const b = ed->ed_fetch_buffer_const(id);
    if(it != rndrbuffers.end() && b != nullptr){
        rndr_offsets(it->second, b);
    }

}

void Renderer::rndr_update_offsets(void){
    for(size_t i = 0; i < commited_ids.size(); i++){
        std::unordered_map<int32_t, RndrItem>::iterator it = rndrbuffers.find(commited_ids[i]);
        const  Buffer* const b = ed->ed_fetch_buffer_const(commited_ids[i]);
        if(it != rndrbuffers.end() && b != nullptr){
            rndr_offsets(it->second, b);
        }
    }
}

void Renderer::rndr_offsets(RndrItem& item, const  Buffer* const b){
    const int line_size = b->buf_get_line_size(b->buf_get_row());
    const int buf_size = b->buf_get_size();

    auto gety = [this, b, &item]() { return item.gety(b->buf_get_row() - item.row_offset, vf.vec_row_block()); };

    while(gety() < item.th.h_th_min && item.row_offset > 0){
        item.row_offset--;
    }

    while(gety() > item.th.h_th_max && item.row_offset < buf_size){
        item.row_offset++;
    }

    auto getx = [this, b, &item]() { return item.getx(b->buf_get_col() - item.col_offset, vf.vec_col_block()); };

    while(getx() < item.th.w_th_min && item.col_offset > 0){
        item.col_offset--;
    }

    while(getx() > item.th.w_th_max && line_size > 0 && item.col_offset <= line_size){
        item.col_offset++;
    }
}

void Renderer::rndr_draw_buffer(RndrItem& item, const  Buffer* const b)
{
    ConstBufRowIt lines(b->buf_get_buffer());
    lines.offset(item.row_offset).valid();

    int row = 0;
    for (; lines.begin != lines.end; lines.increment()) {
        const int y = item.gety(row, vf.vec_row_block());

        if(y > item.viewport.h){
            return;
        }

        ConstBufStrIt line(*lines.begin); 
        line.offset(item.col_offset).valid();

        rndr_draw_line(line, item, y);
        row += 1;
    }
}

void Renderer::rndr_draw_line(
    ConstBufStrIt& line,
    RndrItem& item,
    const int y)
{
    int col = 0;
    const unsigned char skipchar = ' ';

    for (; line.begin != line.end; line.increment()) {
        const unsigned char c = *line.begin;
        const CSprite &spr = vf.vec_index_texture(c);
        const int x = item.getx(col, vf.vec_col_block());

        if(x > item.viewport.w){
            return;
        }

        if (c != skipchar) {
            rndr_put_char(x, y, spr.w, spr.h, spr.texture);
        }
        col += 1;
    }
}

void Renderer::rndr_put_char(const int x, const int y, const int w, const int h, SDL_Texture *t)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(rend, t, NULL, &rect);
}

void Renderer::rndr_put_cursor(RndrItem& item, const int& row, const int& col)
{
    const int x = item.getx(col - item.col_offset, vf.vec_col_block());
    const int y = item.gety(row - item.row_offset, vf.vec_row_block());
    SDL_Rect rect = {x, y, vf.vec_col_block(), vf.vec_row_block()};
    SDL_RenderFillRect(rend, &rect);
}
