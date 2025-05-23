#ifndef SDL2_RENDERER_HPP
#define SDL2_RENDERER_HPP

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "../core/core_token.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

typedef struct WindowPartition WindowPartition;
class Buffer;
class Editor;
typedef struct EditorCmd EditorCmd;

struct RndrThreshold
{
    int h_th_min, h_th_max;
    int w_th_min, w_th_max;

    RndrThreshold(
        const int w, const int h,
        const float wmin, const float wmax,
        const float hmin, const float hmax) : h_th_min(h * hmin), h_th_max(h * hmax),
                                              w_th_min(w * wmin), w_th_max(w * wmax) {}

    RndrThreshold &wupdate(const int width, const float wmin, const float wmax)
    {
        w_th_min = width * wmin, w_th_max = width * wmax;
        return *this;
    }

    RndrThreshold &hupdate(const int height, const float hmin, const float hmax)
    {
        h_th_min = height * hmin, h_th_max = height * hmax;
        return *this;
    }
};
typedef struct Thresholds Thresholds;

struct RndrItem
{
    RndrThreshold th;
    SDL_Rect viewport;

    int col_offset, row_offset;

    const int gety(const int row, const int block, const int vpad) const { return row * (block + vpad); }
    const int getx(const int col, const int block, const int hpad) const { return col * (block + hpad); }

    RndrItem &vp_update(const int x, const int y, const int w, const int h)
    {
        viewport.x = x, viewport.y = y, viewport.w = w, viewport.h = h;
        return *this;
    }

    RndrItem &th_update(RndrThreshold new_th)
    {
        th = new_th;
        return *this;
    }

    RndrItem(void)
        : th(0, 0, 0.0, 0.0, 0.0, 0.0), viewport({0, 0, 0, 0})
    {
        col_offset = 0, row_offset = 0;
    }
};
typedef struct RndrItem RndrItem;

struct ColourRef {
    std::vector<std::pair<uint8_t, SDL_Color>> map;

    ColourRef() {}

    void set_map(void);
};

typedef struct
{
    int w, h;
    std::unordered_map<uint8_t, SDL_Texture*> tmap;
    int bad;
} CSprite;

class VectorFont
{
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
    void vec_create_char_texture(const uint8_t key, SDL_Renderer *rend, CSprite &sprite, SDL_Surface *surface);
    SDL_Surface *vec_create_char_surface(const char *str, const SDL_Color* col);
    const CSprite &vec_index_texture(const unsigned char c) const;
    const int &vec_col_block(void) const { return col_block; }
    const int &vec_row_block(void) const { return row_block; }

  private:
    int error;
    TTF_Font *font;
    int row_block;
    int col_block;
    CSprite *ch;
    ColourRef colours;
};

class Renderer
{
  public:
    Renderer(SDL_Window *w, const Editor *const ed);
    void rndr_set_blendmode(const SDL_BlendMode mode);
    void rndr_set_err(const int errval) { error = errval; }
    int rndr_get_err(void) { return error; }

    class VectorFont &_vf(void) { return vf; }

    SDL_Renderer *rndr_create_renderer(SDL_Window *w, const int flags);
    SDL_Renderer *rndr_get_renderer(void) { return rend; }
    void rndr_set_renderer(SDL_Renderer *r) { rend = r; }
    void rndr_clear(void) { SDL_RenderClear(rend); }
    Renderer& rndr_set_colour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) { 
        SDL_SetRenderDrawColor(rend, r, g, b, a); 
        return *this;
    }
    void rndr_present(void) { SDL_RenderPresent(rend); }
    int rndr_commit_buffer(const int32_t id, const WindowPartition *wp);

    Renderer& rndr_update_viewports(const WindowPartition *wp);
    void rndr_update_offsets_by_id(const int32_t id);
    Renderer& rndr_set_viewport(const SDL_Rect *vp_rect);

    Renderer& rndr_draw_buffer(const RndrItem *const item, const Buffer *const b);
    void rndr_put_cursor(const RndrItem *const item, const int &row, const int &col);
    void rndr_draw_tokens(int& col, ColIt &line, const RndrItem *const item, const int& y);
    void rndr_put_char(const int x, const int y, const int w, const int h, SDL_Texture *t);
    void rndr_buf_offsets(RndrItem &item, const Buffer *const b);
    void rndr_cmd_offsets(void);

    const int &rndr_vpad(void) const { return vertical_padding; }
    const int &rndr_hpad(void) const { return horizontal_padding; }
    Renderer &rndr_draw_cmd(void);
    void rndr_cmd_cursor(void);
    void rndr_update_viewport(RndrItem& item, const WindowPartition *wp, const uint8_t i);

    Renderer& rndr_update_offsets(void);
    const RndrItem *rndr_get_buffer_item(const int32_t id);

    const RndrItem *const rndr_get_rcmd(void) { return &rcmd; }
    const RndrItem *const rndr_get_filename(void) { return &filename; }

    void rndr_draw_filename(const std::string& fn);

  private:
    int error;
    SDL_Renderer *rend;
    std::unordered_set<int32_t> used;
    std::unordered_map<int32_t, RndrItem> rndrbuffers;
    RndrItem rcmd;
    RndrItem filename;
    std::vector<int32_t> commited_ids;
    VectorFont vf;
    const Editor *const ed;
    int vertical_padding;
    int horizontal_padding;
};

#endif
