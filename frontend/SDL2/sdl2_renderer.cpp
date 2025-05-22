
#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"
#include "../../include/SDL2/sdl2_window.hpp"

#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"

#include <iostream>

Renderer::Renderer(SDL_Window *w, const Editor *const edptr) : error(SDL2_NIL), rend(rndr_create_renderer(w, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
                                                               vf(rndr_get_renderer()), ed(edptr), vertical_padding(2), horizontal_padding(2)
{
    rndr_set_err((rend != nullptr) ? SDL2_NIL : SDL2_ERR);
    rndr_set_err((edptr != nullptr) ? SDL2_NIL : SDL2_ERR);
}

void Renderer::rndr_set_blendmode(SDL_BlendMode mode)
{
    SDL_SetRenderDrawBlendMode(rend, mode);
}

void Renderer::rndr_update_viewport(RndrItem& item, const WindowPartition *wp, const uint8_t i){
    item.vp_update(
        wp->get(i).x, wp->get(i).y, 
        wp->get(i).w, wp->get(i).h
    ).th_update(RndrThreshold(wp->get(i).w, wp->get(i).h, 0.1, 0.8, 0.1, 0.8));
}

Renderer &Renderer::rndr_draw_cmd(void)
{
    const EditorCmd &ec = ed->ed_get_cmd();
    ConstBufStrIt line(ec.cmdstr);
    line.offset(rcmd.col_offset).valid();
    int col = 0, y = 0;
    rndr_draw_line(col, line, &rcmd, y);
    return *this;
}

void Renderer::rndr_draw_filename(const std::string& fn){
    ConstBufStrIt line(fn);
    int col = 0, y = 0;
    rndr_draw_line(col, line, &filename, y);
}

Renderer& Renderer::rndr_update_viewports(const WindowPartition *wp)
{
    for (size_t i = 0; i < commited_ids.size(); i++) {
        std::unordered_map<int32_t, RndrItem>::iterator it = rndrbuffers.find(commited_ids[i]);
        if (it != rndrbuffers.end()) {
            rndr_update_viewport(it->second, wp, BUF_BOX);
        }
    }

    rndr_update_viewport(rcmd, wp, CMD_BOX);
    rndr_update_viewport(filename, wp, FN_BOX);
    return *this;
}

int Renderer::rndr_commit_buffer(const int32_t id, const WindowPartition *wp)
{
    if (id < 0) {
        return 0;
    }

    std::unordered_set<int32_t>::iterator it = used.find(id);
    if (it != used.end()) {
        std::cout << "ID already exists" << std::endl;
        return 0;
    }

    //No need to initialize with proper values -
    //Gets set in rndr_update_viewports which is called after a commit.
    RndrItem item;
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

Renderer& Renderer::rndr_set_viewport(const SDL_Rect *vp_rect)
{
    if (vp_rect) {
        SDL_RenderSetViewport(rend, vp_rect);
    } else {
        SDL_RenderSetViewport(rend, NULL);
    }

    return *this;
}

const RndrItem *Renderer::rndr_get_buffer_item(const int32_t id){
    std::unordered_map<int32_t, RndrItem>::iterator it = rndrbuffers.find(id);
    if(it != rndrbuffers.end()){
        return &it->second;
    } else {
        return nullptr;
    }
}


void Renderer::rndr_update_offsets_by_id(const int32_t id)
{
    std::unordered_map<int32_t, RndrItem>::iterator it = rndrbuffers.find(id);
    const Buffer *const b = ed->ed_fetch_buffer_const(id);
    if (it != rndrbuffers.end() && b != nullptr) {
        rndr_buf_offsets(it->second, b);
    }
}

Renderer& Renderer::rndr_update_offsets(void)
{
    for (size_t i = 0; i < commited_ids.size(); i++) {
        std::unordered_map<int32_t, RndrItem>::iterator it = rndrbuffers.find(commited_ids[i]);
        const Buffer *const b = ed->ed_fetch_buffer_const(commited_ids[i]);
        if (it != rndrbuffers.end() && b != nullptr) {
            rndr_buf_offsets(it->second, b);
        }
    }
    return *this;
}

void Renderer::rndr_cmd_offsets(void)
{
    const int line_size = ed->ed_get_cmd().cmdstr.size();
    const int col = ed->ed_get_cmd().cursor;

    auto getx = [this, &col]() { return rcmd.getx(col - rcmd.col_offset, vf.vec_col_block(), horizontal_padding); };

    while (getx() < rcmd.th.w_th_min && rcmd.col_offset > 0) {
        rcmd.col_offset--;
    }

    while (getx() > rcmd.th.w_th_max && line_size > 0 && rcmd.col_offset <= line_size) {
        rcmd.col_offset++;
    }
}

void Renderer::rndr_buf_offsets(RndrItem &item, const Buffer *const b)
{
    const int line_size = b->buf_get_line_size(b->buf_get_row());
    const int buf_size = b->buf_get_size();

    auto gety = [this, b, &item]() { return item.gety(b->buf_get_row() - item.row_offset, vf.vec_row_block(), vertical_padding); };

    while (gety() < item.th.h_th_min && item.row_offset > 0) {
        item.row_offset--;
    }

    while (gety() > item.th.h_th_max && item.row_offset < buf_size) {
        item.row_offset++;
    }

    auto getx = [this, b, &item]() { return item.getx(b->buf_get_col() - item.col_offset, vf.vec_col_block(), horizontal_padding); };

    while (getx() < item.th.w_th_min && item.col_offset > 0) {
        item.col_offset--;
    }

    while (getx() > item.th.w_th_max && line_size > 0 && item.col_offset <= line_size) {
        item.col_offset++;
    }
}

Renderer& Renderer::rndr_draw_buffer(const RndrItem *const item, const Buffer *const b)
{
    const std::vector<std::vector<Token>>& token_buffer = b->buf_get_token_buffer();
    int row = 0;

    for(size_t i = 0 + item->row_offset; i < token_buffer.size(); i++){
        ConstTokenRowIt token_row(token_buffer[i]);

        token_row.offset(item->col_offset).valid();
        int token_offset = token_row.substr_offset;

        int col = 0;
        for(; token_row.begin != token_row.end; token_row.increment()){
            const int y = item->gety(row, vf.vec_row_block(), vertical_padding);

            if(y > item->viewport.h){
                return *this;
            }

            ConstBufStrIt token(token_row.begin->token);
            token.offset(token_offset).valid();
            rndr_draw_line(col, token, item, y);

            //Set it to zero once the first token is rendered.
            token_offset = 0;
        }
        row++;
    }

    return *this;
}

void Renderer::rndr_draw_line(
    int& col,
    ConstBufStrIt &line,
    const RndrItem *const item,
    const int& y)
{
    const unsigned char skipchar = ' ';
    for (; line.begin != line.end; line.increment()) {
        const unsigned char c = *line.begin;
        const CSprite &spr = vf.vec_index_texture(c);
        const int x = item->getx(col, vf.vec_col_block(), horizontal_padding);

        if (x > item->viewport.w) {
            return;
        }

        if (c != skipchar) {
            rndr_put_char(x, y, spr.w, spr.h, spr.texture);
        }
        col++;
    }
}

void Renderer::rndr_put_char(const int x, const int y, const int w, const int h, SDL_Texture *t)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(rend, t, NULL, &rect);
}

void Renderer::rndr_put_cursor(const RndrItem *const item, const int &row, const int &col)
{
    if (!ed->ed_taking_cmd()) {
        const int x = item->getx(col - item->col_offset, vf.vec_col_block(), horizontal_padding);
        const int y = item->gety(row - item->row_offset, vf.vec_row_block(), vertical_padding);
        SDL_Rect rect = {x, y, vf.vec_col_block(), vf.vec_row_block()};
        SDL_RenderFillRect(rend, &rect);
    }
}

void Renderer::rndr_cmd_cursor(void)
{
    if (ed->ed_taking_cmd()) {
        const int c = ed->ed_get_cmd().cursor;
        const int x = rcmd.getx(c - rcmd.col_offset, vf.vec_col_block(), horizontal_padding);
        const int y = 0;
        SDL_Rect rect{x, y, vf.vec_col_block(), vf.vec_row_block()};
        SDL_RenderFillRect(rend, &rect);
    }
}
