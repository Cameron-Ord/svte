#ifndef WINDOW_HPP
#define WINDOW_HPP

struct SDL_Window;
typedef struct SDL_Window SDL_Window;

class Window
{
  public:
    Window(void);
    ~Window(void);
    SDL_Window *get_window(void);
    const void *create_window(void);
    const int *get_width(void) { return &s_width; }
    const int *get_height(void) { return &s_height; }
    void win_update_dimensions(void);

  private:
    SDL_Window *w;
    int s_height;
    int s_width;
};

#endif
