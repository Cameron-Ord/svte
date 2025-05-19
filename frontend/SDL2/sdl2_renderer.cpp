#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

#include "../../include/core/core_buffer.hpp"


#include <iostream>

Renderer::Renderer(SDL_Window *w)
    : error(SDL2_NIL), rend(rndr_create_renderer(w, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)), vf(rndr_get_renderer())
{
    rndr_set_err((rend != nullptr) ? SDL2_NIL : SDL2_ERR);
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

int Renderer::rndr_commit_buffer(BCommit commit, const int width, const int height)
{
    if(commit.id < 0 || !commit.cbuf){
        std::cerr << "Bad ID or NULL pointer" << std::endl;
        return 0;
    }

    std::unordered_set<int32_t>::iterator it = used.find(commit.id);
    if (it != used.end()) {
        std::cout << "ID already exists" << std::endl;
        return 0;
    }

    RndrItem item(width, height);
    if(!item.set_buf(commit.cbuf)){
        std::cerr << "Passed a NULL pointer using class Buffer" << std::endl;
        return 0;
    }

    used.insert(commit.id);
    rndrbuffers.insert({commit.id, item});
    commited_ids.push_back(commit.id);

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
    if (it != rndrbuffers.end()) {
        rndr_set_viewport(&it->second.viewport);
        rndr_draw_buffer(it->second);
        rndr_set_colour(255, 255, 255, 125);
        rndr_put_cursor(it->second);
    }
}

void Renderer::rndr_update_offsets_by_id(const int32_t id)
{
    std::unordered_map<int32_t, RndrItem>::iterator it = rndrbuffers.find(id);
    if(it != rndrbuffers.end()){
        rndr_offsets(it->second);
    }

}

void Renderer::rndr_update_offsets(void){
    for(size_t i = 0; i < commited_ids.size(); i++){
        std::unordered_map<int32_t, RndrItem>::iterator it = rndrbuffers.find(commited_ids[i]);
        if(it != rndrbuffers.end()){
            rndr_offsets(it->second);
        }
    }
}

void Renderer::rndr_offsets(RndrItem& item){
    const int line_size = item.b->buf_get_line_size(item.b->buf_get_row());
    const int buf_size = item.b->buf_get_size();

    auto gety = [this, &item]() { return item.gety(item.b->buf_get_row() - item.row_offset, vf.vec_row_block()); };

    while(gety() < item.th.h_th_min && item.row_offset > 0){
        item.row_offset--;
    }

    while(gety() > item.th.h_th_max && item.row_offset < buf_size){
        item.row_offset++;
    }

    auto getx = [this, &item]() { return item.getx(item.b->buf_get_col() - item.col_offset, vf.vec_col_block()); };

    while(getx() < item.th.w_th_min && item.col_offset > 0){
        item.col_offset--;
    }

    while(getx() > item.th.w_th_max && line_size > 0 && item.col_offset <= line_size){
        item.col_offset++;
    }
}

void Renderer::rndr_draw_buffer(RndrItem& item)
{
    ConstBufRowIt lines(item.b->buf_get_buffer());
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

void Renderer::rndr_put_cursor(RndrItem& item)
{
    const int x = item.getx(item.b->buf_get_col() - item.col_offset, vf.vec_col_block());
    const int y = item.gety(item.b->buf_get_row() - item.row_offset, vf.vec_row_block());
    SDL_Rect rect = {x, y, vf.vec_col_block(), vf.vec_row_block()};
    SDL_RenderFillRect(rend, &rect);
}
