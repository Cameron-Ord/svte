#ifndef WINDOW_HPP
#define WINDOW_HPP
#include "vecdef.hpp"

struct SDL_Window;
typedef struct SDL_Window SDL_Window;

class Window
{
  public:
    Window(void);
    ~Window(void);
    SDL_Window *get_window(void);
    const void *create_window(void);
    Vec2i *get_dimensions(void);
    void win_update_dimensions(void);

  private:
    SDL_Window *w;
    Vec2i dimensions;
};

#endif
