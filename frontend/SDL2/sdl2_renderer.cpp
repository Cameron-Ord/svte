#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"


#include <iostream>

Renderer::Renderer(SDL_Window *w)
    : error(SDL2_NIL), rend(nullptr)
{
    rndr_set_err(
        rndr_create_renderer(
            w,
            SDL_RENDERER_ACCELERATED |
                SDL_RENDERER_PRESENTVSYNC));
}

//At some point maybe support multiple buffers but right now I just want to make what I have good first
//so just only support one buffer displaying at a time for the size of window. Can still switch between them though.
void Renderer::rndr_update_viewports(
    const int width, const int height
)
{
    for (size_t i = 0; i < commited_ids.size(); i++) {
        const int32_t id = commited_ids[i];
        class BufRenderer *br = rndr_grab_bufrenderer(id);
        if (br) {
            br->br_set_viewport_dims(width, height);
            br->br_set_thresholds(Thresholds(width, height, 0.1, 0.9, 0.1, 0.9));
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

    class BufRenderer brend(commit.cbuf, width, height);
    if (!brend.br_get_err()) {
        std::cerr << "Passed a NULL pointer using class Buffer" << std::endl;
        return 0;
    }

    used.insert(commit.id);
    renderers.insert({commit.id, brend});
    commited_ids.push_back(commit.id);
    return 1;
}

void Renderer::rndr_set_err(const int errval)
{
    error = errval;
}

int Renderer::rndr_get_err(void)
{
    return error;
}

int Renderer::rndr_create_renderer(SDL_Window *w, const int flags)
{
    if (!w) {
        std::cerr << "Passed a NULL pointer using SDL_Window" << std::endl;
        return SDL2_ERR;
    }

    rndr_set_renderer(SDL_CreateRenderer(w, -1, flags));
    if (!rndr_get_renderer()) {
        std::cerr << "Failed to create renderer! -> " << SDL_GetError() << std::endl;
        return SDL2_ERR;
    }
    return SDL2_NIL;
}

SDL_Renderer *Renderer::rndr_get_renderer(void)
{
    return rend;
}

void Renderer::rndr_set_renderer(SDL_Renderer *r)
{
    rend = r;
}

void Renderer::rndr_clear(void)
{
    SDL_RenderClear(rend);
}

void Renderer::rndr_present(void)
{
    SDL_RenderPresent(rend);
}

void Renderer::rndr_set_colour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void Renderer::rndr_set_viewport(const SDL_Rect *vp_rect)
{
    if (vp_rect) {
        SDL_RenderSetViewport(rend, vp_rect);
    } else {
        SDL_RenderSetViewport(rend, NULL);
    }
}

void Renderer::rndr_draw_id(const int32_t id, const class VectorFont *vfont)
{
    class BufRenderer *br = rndr_grab_bufrenderer(id);
    if (br && br->br_valid_ptr()) {
        rndr_set_viewport(br->br_get_viewport());
        br->br_draw_buffer(rend, vfont);
        rndr_set_colour(255, 255, 255, 255);
        br->br_put_cursor(rend, vfont->vec_col_block(), vfont->vec_row_block());
    }
}

void Renderer::rndr_update_offsets_by_id(const int32_t id, const int row_block, const int col_block)
{
    BufRenderer *br = rndr_grab_bufrenderer(id);
    if(!br){
        return;
    }
    br->br_update_offsets(row_block, col_block);
}

void Renderer::rndr_update_offsets(const int row_block, const int col_block){
    for (size_t i = 0; i < commited_ids.size(); i++) {
        const int32_t id = commited_ids[i];
        class BufRenderer *br = rndr_grab_bufrenderer(id);
        if (br) {
            br->br_update_offsets(row_block, col_block);
        }
    }
}

class BufRenderer *Renderer::rndr_grab_bufrenderer(const int32_t id)
{
    std::unordered_map<int32_t, class BufRenderer>::iterator it = renderers.find(id);
    if (it != renderers.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}