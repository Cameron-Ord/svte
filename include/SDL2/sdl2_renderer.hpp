#ifndef SDL2_RENDERER_HPP
#define SDL2_RENDERER_HPP

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "../core/core_iterator.hpp"
#include "../core/core_structs.hpp"

#include <unordered_map>
#include <unordered_set>

class Buffer;

typedef struct {
    int w, h;
    SDL_Texture *texture;
    int bad;
}CSprite;

struct RndrThreshold {
    int h_th_min, h_th_max;
    int w_th_min, w_th_max;

    RndrThreshold(
        const int w, const int h, 
        const float wmin, const float wmax, 
        const float hmin, const float hmax
    ) : h_th_min(h * hmin), h_th_max(h * hmax),
        w_th_min(w * wmin), w_th_max(w * wmax){}

    RndrThreshold& wupdate(const int width, const float wmin, const float wmax) { 
        w_th_min = width * wmin, w_th_max = width * wmax; 
        return *this;
    }

    RndrThreshold& hupdate(const int height, const float hmin, const float hmax){
        h_th_min = height * hmin, h_th_max = height * hmax;
        return *this;
    }
    
};
typedef struct Thresholds Thresholds;


struct RndrItem{
    const class Buffer *b;
    RndrThreshold th;
    SDL_Rect viewport;

    int vertical_padding;
    int horizontal_padding;
    int col_offset, row_offset;

    int gety(const int row, const int block) { return row * (block + vertical_padding); }
    int getx(const int col, const int block) { return col * (block + horizontal_padding); }

    int set_buf(const class Buffer *cbuf) { return ((b = cbuf) != NULL); }
    int valid_ptr(void){ return b != NULL; }

    void vp_update_all(const int x, const int y, const int w, const int h) {  
        viewport.x = x, viewport.y = y, viewport.w = w, viewport.h = h;
    }

    RndrItem& th_update(RndrThreshold new_th){
        th = new_th;
        return *this;
    }

    RndrItem& vp_update_w(const int w) {  
        viewport.w = w;
        return *this;
    }

    
    RndrItem& vp_update_h(const int h) {  
        viewport.h = h;
        return *this;
    }

    RndrItem(const int width, const int height) 
    : th(width, height, 0.1, 0.9, 0.1, 0.9), viewport({0, 0, width, height}) {
        col_offset = 0, row_offset = 0;
        vertical_padding = 2, horizontal_padding = 2;
    }

};
typedef struct RndrItem RndrItem;


class VectorFont {
    public:
        VectorFont(SDL_Renderer *rend);
        void vec_set_err(const int errval) { error = errval; }
        int vec_get_err(void) { return error; }
        int vec_open_font(const std::string path, const int ptsize);
        int vec_alloc_texture_array(void);
        void vec_set_font(TTF_Font *f) { font = f; }
        TTF_Font *vec_get_font(void) { return font; }
        void vec_set_char(void);
        int vec_create_textures(SDL_Renderer *rend);
        void vec_create_char_texture(SDL_Renderer *rend, CSprite& sprite, SDL_Surface *surface);
        SDL_Surface *vec_create_char_surface(const char *str);
        const CSprite& vec_index_texture(const unsigned char c) const;
        const int& vec_col_block(void) const { return col_block; }
        const int& vec_row_block(void) const { return row_block; }

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
        void rndr_set_err(const int errval) { error = errval; }
        int rndr_get_err(void) { return error; }

        class VectorFont& _vf(void) { return vf; }

        SDL_Renderer *rndr_create_renderer(SDL_Window *w, const int flags);
        SDL_Renderer *rndr_get_renderer(void) { return rend; }
        void rndr_set_renderer(SDL_Renderer *r) { rend = r; }
        void rndr_clear(void) { SDL_RenderClear(rend); }
        void rndr_set_colour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) { SDL_SetRenderDrawColor(rend, r, g, b, a); }
        void rndr_present(void) { SDL_RenderPresent(rend); }
        int rndr_commit_buffer(BCommit commit, const int width, const int height);
        void rndr_draw_id(const int32_t id);

        void rndr_update_viewports(const int width, const int height);
        void rndr_update_offsets_by_id(const int32_t id);
        void rndr_set_viewport(const SDL_Rect *vp_rect);
        
        void rndr_draw_buffer(RndrItem& item);
        void rndr_put_cursor(RndrItem& item);
        void rndr_draw_line(ConstBufStrIt& line, RndrItem& item, const int y);
        void rndr_put_char(const int x, const int y, const int w, const int h, SDL_Texture *t);
        void rndr_offsets(RndrItem& item);

        void rndr_update_offsets(void);

    private:
        int error;
        SDL_Renderer *rend;
        std::unordered_set<int32_t> used;
        std::unordered_map<int32_t, RndrItem> rndrbuffers;
        std::vector<int32_t> commited_ids;
        class VectorFont vf;
};

#endif
