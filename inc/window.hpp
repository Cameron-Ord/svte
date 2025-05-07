#ifndef WINDOW_HPP
#define WINDOW_HPP

struct SDL_Window;
typedef struct SDL_Window SDL_Window;

typedef enum { WIN_STATE_BAD = 0, WIN_STATE_OK = 1} WINDOW_STATE;


class Window
{
  public:
    Window(void);
    ~Window(void);
    const int get_state(void) { return STATE; }
    SDL_Window *get_window(void);
    const void *create_window(void);
    const int *get_width(void) { return &s_width; }
    const int *get_height(void) { return &s_height; }
    void win_update_dimensions(void);

  private:
    int STATE;
    SDL_Window *w;
    int s_height;
    int s_width;
};

#endif
