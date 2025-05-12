#ifndef SDL2_RENDERER_HPP
#define SDL2_RENDERER_HPP

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#define ASCII_MAX 128
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
        SDL_Texture* rndr_create_char_texture(SDL_Surface *surface);
        int rndr_create_textures(void);

    private:
        int error;
        SDL_Renderer *rend;
        TTF_Font *rndr_font;
        const int* const _width;
        const int* const _height;
        SDL_Texture* ch[ASCII_MAX];
};

#endif
