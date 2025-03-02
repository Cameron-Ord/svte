#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "grid.hpp"
#include "strbuffer.hpp"

#include <SDL2/SDL_render.h>
#include <cstdint>

class Window;

struct SDL_Rect;
typedef struct SDL_Rect SDL_Rect;

struct Vec2i;
typedef struct Vec2i Vec2i;

struct SDL_Window;
typedef struct SDL_Window SDL_Window;

class Chars;

struct Render_Rect {
  SDL_Texture *t;
  SDL_Rect rect;
};

class FontRenderer {
public:
  FontRenderer(SDL_Renderer *renderer, const Vec2i *dimensions);
  ~FontRenderer(void);
  void frender_set_renderer(SDL_Renderer *r);

private:
  SDL_Renderer *rend;
  const Vec2i *dim;
  Grid editor_grid;
};

class Renderer {
public:
  Renderer(Window *w);
  ~Renderer(void);
  SDL_Renderer *get_renderer(void);
  const void *create_renderer(SDL_Window *w);
  void bg_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  void clear_render(void);
  void render_present(void);
  FontRenderer create_font_renderer(void);
  FontRenderer *_frender(void);

private:
  SDL_Renderer *rend;
  const Vec2i *dim;
  FontRenderer frender;
};

#endif
