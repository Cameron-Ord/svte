#ifndef SDL2_WINDOW_HPP
#define SDL2_WINDOW_HPP

#include <SDL2/SDL_video.h>
#include "../vector.hpp"
#include <array>

#define CMD_BOX 0
#define BUF_BOX 1
#define FN_BOX 2

struct WindowPartition
{
  Vec4i fnbox;
  Vec4i bufbox;
  Vec4i cmdbox;
  Vec4i statusbox;

  std::array<Vec4i, 3> boxes = {cmdbox, bufbox, fnbox};

  void set(const int i, const int x, const int y, const int w, const int h){
    boxes.at(i).x = x, boxes.at(i).y = y, boxes.at(i).w = w, boxes.at(i).h = h;
  }

  const Vec4i& get(const int i) const {
    return boxes.at(i);
  }
};

class Window
{
  public:
    Window(void);
    ~Window(void);
    void win_set_err(const int errval) { error = errval; }
    int win_get_err(void) { return error; }
    int win_create_window(const int flags);
    Window &win_update_window_values(void);
    void win_show_window(void);
    void win_hide_window(void);
    void win_set_window(SDL_Window *w) { win = w; }
    SDL_Window *win_get_window(void) { return win; }
    void win_set_resizeable(const SDL_bool opt);
    const int win_width(void) { return width; }
    const int win_height(void) { return height; }
    void win_dft_partition(const int &rblock, const int &vpadding);
    const WindowPartition *const _wp(void) const { return &partitions; }

  private:
    int error;
    SDL_Window *win;
    int width, height;
    WindowPartition partitions;
};

#endif
