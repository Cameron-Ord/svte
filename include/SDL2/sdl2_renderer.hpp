#ifndef SDL2_RENDERER_HPP
#define SDL2_RENDERER_HPP

#include <SDL2/SDL_render.h>

class Renderer {
    public:
        Renderer(SDL_Window *w, const int* const width, const int* const height);
        ~Renderer(void);
        void rndr_set_err(const int err);
        int rndr_get_err(void);
        SDL_Renderer *rndr_get_renderer(void);
        int rndr_create_renderer(SDL_Window *w, const int flags);
        void rndr_fill_bg(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
        void rndr_clear(void);
        void rndr_present(void);

    private:
        SDL_Renderer *rend;
        const int* const _width;
        const int* const _height;
        int error;
};

#endif
