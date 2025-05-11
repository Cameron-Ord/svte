#ifndef SDL2_RENDERER_HPP
#define SDL2_RENDERER_HPP

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;

class Renderer {
    public:
        Renderer(SDL_Window *w, const int* const width, const int* const height);
        ~Renderer(void);
        int rndr_create_renderer(SDL_Window *w, const int flags);

    private:
        SDL_Renderer *rend;
        const int* const width, height;
};

#endif
