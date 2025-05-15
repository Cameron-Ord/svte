#ifndef SDL2_RENDERER_HPP
#define SDL2_RENDERER_HPP

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

typedef struct {
    int w, h;
    SDL_Texture *texture;
    int bad;
}CSprite;

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
        void rndr_set_err(const int errval);
        int rndr_get_err(void);
        int rndr_create_renderer(SDL_Window *w, const int flags);
        SDL_Renderer *rndr_get_renderer(void);
        void rndr_set_renderer(SDL_Renderer *r);
        void rndr_clear(void);
        void rndr_set_colour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
        void rndr_present(void);
        int rndr_commit_buffer(const int32_t id, const class Buffer *cbuf, const int width, const int height);
        void rndr_draw_id(const int32_t id, const class VectorFont* vfont);
        class BufRenderer *rndr_grab_bufrenderer(const int32_t id);
        void rndr_update_viewports(const std::vector<int32_t>& open, const int width, const int height);
        void rndr_id_update_offsets(const int32_t id);

    private:
        int error;
        SDL_Renderer *rend;
        std::unordered_set<int32_t> used;
        std::unordered_map<int32_t, class BufRenderer> renderers;
};

class Buffer;

class BufRenderer {
    public:
        BufRenderer(const class Buffer *cbuf, const int width, const int height);
        void br_set_viewport_dims(const int width, const int height);
        void br_set_viewport_pos(const int x, const int y);
        int br_get_err(void);
        void br_set_err(const int errval);
        int br_set_buf(const class Buffer *cbuf);
        int br_valid_ptr(void);
        void br_draw_buffer(SDL_Renderer *rend, const class VectorFont* vfont, std::vector<std::string>::const_iterator it, std::vector<std::string>::const_iterator end);
        void br_draw_line(SDL_Renderer *rend, const class VectorFont* vfont, std::string::const_iterator it, std::string::const_iterator end, const int y);
        void br_put_char(SDL_Renderer *rend, const int x, const int y, const int w, const int h, SDL_Texture *t);
        void br_put_cursor(SDL_Renderer *rend);
        std::vector<std::string>::const_iterator br_get_row_start(void);
        std::vector<std::string>::const_iterator br_get_row_end(void);


    private:
        int error;
        int vertical_padding;
        int horizontal_padding;
        int col_offset, row_offset;
        const class Buffer *constbuf;
        SDL_Rect viewport;
};
#endif
