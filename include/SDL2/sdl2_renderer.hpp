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
        const CSprite& vec_index_texture(const unsigned char c);
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
        int rndr_commit_buffer(const int32_t id, const class Buffer *cbuf);

    private:
        int error;
        SDL_Renderer *rend;
        std::unordered_set<int32_t> used;
        std::unordered_map<int32_t, class BufRenderer> renderers;
};

class Buffer;

class BufRenderer {
    public:
        BufRenderer(const class Buffer *cbuf);
        int brndr_get_err(void);
        void brndr_set_err(const int errval);
        int brndr_set_buf(const class Buffer *cbuf);
    private:
        int error;
        const class Buffer *constbuf;

};
#endif
