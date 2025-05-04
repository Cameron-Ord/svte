#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>

class Window;

struct SDL_Texture;
typedef struct SDL_Texture SDL_Texture;

struct SDL_Renderer;
typedef struct SDL_Renderer SDL_Renderer;

struct SDL_Window;
typedef struct SDL_Window SDL_Window;

class Chars;
class Buffer;

#include <SDL2/SDL_rect.h>

struct Buffer_Viewport {
  //ID of buffer, each buffer gets assigned its own viewport.
  //This can be done later as I have yet to actually implement multiple buffers.. but the groundwork is laid out.
  uint32_t id;
  SDL_Rect viewport;
};

class Renderer
{
  public:
    Renderer();
    ~Renderer(void);
    SDL_Renderer *get_renderer(void);
    const void *create_renderer(SDL_Window *w);
    void renderer_fill_bg(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void renderer_clear(void);
    void renderer_present(void);
    int renderer_draw_char(const int x, const int y, const int w, const int h, SDL_Texture *tex);
    int renderer_draw_cursor(const int row, const int col, const int max_width, const int max_height);
    int renderer_draw_file(class Buffer *buf, class Chars *ch);

  private:
    SDL_Renderer *rend;
};

#endif
