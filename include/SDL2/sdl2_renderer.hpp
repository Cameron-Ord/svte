#ifndef SDL2_RENDERER_HPP
#define SDL2_RENDERER_HPP

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#define ASCII_MAX 128
#define ASCII_MIN 0
#define ASCII_START 32
#define ASCII_END 128

typedef struct {
    int w, h;
    SDL_Texture *texture;
    int bad;
}CSprite;

class Renderer {
    public:
        Renderer(SDL_Window *w, const int* const width, const int* const height);
        ~Renderer(void);
        int rndr_open_font(const std::string filename, const int ptsize);
        void rndr_set_font(TTF_Font *font);
        TTF_Font *rndr_get_font(void);
        void rndr_set_err(const int err);
        int rndr_get_err(void);
        SDL_Renderer *rndr_get_renderer(void);
        int rndr_create_renderer(SDL_Window *w, const int flags);
        void rndr_fill_bg(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
        void rndr_clear(void);
        void rndr_present(void);
        void rndr_set_char(void);
        SDL_Surface* rndr_create_char_surface(const char *str);
        void rndr_create_char_texture(CSprite& spr, SDL_Surface *surface);
        int rndr_create_textures(void);
        void rndr_draw_buffer(std::vector<std::string>::const_iterator it, std::vector<std::string>::const_iterator end);
        void rndr_draw_line(std::string::const_iterator it, std::string::const_iterator end, const int& y);
        const CSprite& rndr_index_texture(const unsigned char c);
        void rndr_set_horizontal_padding(const int val);
        void rndr_set_vertical_padding(const int val);
        void rndr_draw_char(const int& x, const int& y, const int& w, const int& h, SDL_Texture *texture);

    private:
        int error;
        int vertical_padding;
        int horizontal_padding;
        int row_block;
        int col_block;
        SDL_Renderer *rend;
        TTF_Font *rndr_font;
        const int* const _width;
        const int* const _height;
        CSprite ch[ASCII_MAX];
};      

#endif
