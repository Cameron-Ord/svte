#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>

struct SDL_Renderer;
typedef struct SDL_Renderer SDL_Renderer;

struct SDL_Window;
typedef struct SDL_Window SDL_Window;

struct Ascii_Char;
typedef struct Ascii_Char Ascii_Char;

#include <cstddef>

class Renderer {
public:
  ~Renderer(void);
  SDL_Renderer *get_renderer(void);
  const void *create_renderer(SDL_Window *w);
  void bg_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  void clear_render(void);
  void render_present(void);
  void render_logo(const char *str, const size_t len);

private:
  SDL_Renderer *rend;
};

Renderer *get_renderer_inst(void);

#endif
