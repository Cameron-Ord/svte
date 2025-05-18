#ifndef SDL2_RENDERER_HPP
#define SDL2_RENDERER_HPP

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "../core/core_iterator.hpp"
#include "../core/core_structs.hpp"

#include <unordered_map>
#include <unordered_set>


typedef struct {
    int w, h;
    SDL_Texture *texture;
    int bad;
}CSprite;

struct Thresholds {
    int h_th_min;
    int h_th_max;
    int w_th_min;
    int w_th_max;

    Thresholds(
        const int w, const int h, 
        const float wmin, const float wmax, 
        const float hmin, const float hmax
    ) : h_th_min(h * hmin), h_th_max(h * hmax),
        w_th_min(w * wmin), w_th_max(w * wmax){}
};
typedef struct Thresholds Thresholds;

class VectorFont {
    public:
        VectorFont(SDL_Renderer *rend);
        void vec_set_err(const int errval);
        int vec_get_err(void);
        int vec_open_font(const std::string path, const int ptsize);
        int vec_alloc_texture_array(void);
        void vec_set_font(TTF_Font *f);
        TTF_Font *vec_get_font(void);
        void vec_set_char(void);
        int vec_create_textures(SDL_Renderer *rend);
        void vec_create_char_texture(SDL_Renderer *rend, CSprite& sprite, SDL_Surface *surface);
        SDL_Surface *vec_create_char_surface(const char *str);
        const CSprite& vec_index_texture(const unsigned char c) const;
        const int& vec_col_block(void) const;
        const int& vec_row_block(void) const;

    private:
        int error;
        TTF_Font *font;
        int row_block;
        int col_block;
        CSprite *ch;

};

class Renderer {
    public:
        Renderer(SDL_Window *w);
        void rndr_set_blendmode(const SDL_BlendMode mode);
        void rndr_set_err(const int errval);
        int rndr_get_err(void);
        int rndr_create_renderer(SDL_Window *w, const int flags);
        SDL_Renderer *rndr_get_renderer(void);
        void rndr_set_renderer(SDL_Renderer *r);
        void rndr_clear(void);
        void rndr_set_colour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
        void rndr_present(void);
        int rndr_commit_buffer(BCommit commit, const int width, const int height);
        void rndr_draw_id(const int32_t id, const class VectorFont* vfont);
        class BufRenderer *rndr_grab_bufrenderer(const int32_t id);
        void rndr_update_viewports(const int width, const int height);
        void rndr_update_offsets_by_id(const int32_t id, const int row_block, const int col_block);
        void rndr_update_offsets(const int row_block, const int col_block);
        void rndr_set_viewport(const SDL_Rect *vp_rect);

    private:
        int error;
        SDL_Renderer *rend;
        std::unordered_set<int32_t> used;
        std::unordered_map<int32_t, class BufRenderer> renderers;
        std::vector<int32_t> commited_ids;
};

class Buffer;

class BufRenderer {
    public:
        BufRenderer(const class Buffer *cbuf, const int width, const int height);
        void br_draw_buffer(SDL_Renderer *rend, const class VectorFont* vfont);
        void br_draw_line(SDL_Renderer *rend, const class VectorFont* vfont, ConstBufStrIt& row, const int y);
        void br_put_char(SDL_Renderer *rend, const int x, const int y, const int w, const int h, SDL_Texture *t);
        void br_put_cursor(SDL_Renderer *rend, const int width, const int height);
        void br_update_offsets(const int row_block, const int col_block);
        int br_getx(const int col, const int blocksize);
        int br_gety(const int row, const int blocksize);

        int br_row_offset(void) { return row_offset; }
        int br_col_offset(void) { return col_offset; }
        void br_set_col_offset(const int val) { col_offset = val; }
        void br_set_row_offset(const int val) { row_offset = val; }
        void br_set_thresholds(Thresholds th) { thresholds = th; }
        const SDL_Rect *br_get_viewport(void) { return &viewport; }
        int br_get_err(void) { return error; }
        void br_set_err(const int errval) { error = errval; }
        int br_set_buf(const class Buffer *cbuf) { return ((constbuf = cbuf) != NULL); }
        int br_valid_ptr(void){ return constbuf != NULL; }
        void br_set_viewport_dims(const int width, const int height) {viewport.w = width, viewport.h = height; }
        void br_set_viewport_pos(const int x, const int y) { viewport.x = x, viewport.y = y; }
    private:
        int error;
        int vertical_padding;
        int horizontal_padding;
        int col_offset, row_offset;
        const class Buffer *constbuf;
        SDL_Rect viewport;
        Thresholds thresholds;
};
#endif
