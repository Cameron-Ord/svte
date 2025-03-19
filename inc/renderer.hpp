#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "grid.hpp"
#include <SDL2/SDL_render.h>
#include <string>
#include <vector>

class Window;

struct SDL_Rect;
typedef struct SDL_Rect SDL_Rect;

struct Vec2i;
typedef struct Vec2i Vec2i;

struct SDL_Window;
typedef struct SDL_Window SDL_Window;

struct Buf;
typedef struct Buf Buf;

class Chars;

class Renderer
{
  public:
    Renderer();
    ~Renderer(void);
    SDL_Renderer *get_renderer(void);
    const void *create_renderer(SDL_Window *w);
    void bg_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void clear_render(void);
    void render_present(void);
    int render_buffer(const Buf *buf, Chars *ch);
    int render_curs(const Buf *b, const Vec2i *dims);
  private:
    SDL_Renderer *rend;
};

#endif
