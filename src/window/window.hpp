#pragma once
#include "../util/alias.hpp"
#include <string>

typedef struct SDL_Window SDL_Window;

class window_container {
public:
  window_container(std::string title, i32 bwidth, i32 bheight);
  bool init_window(unsigned long flags);
  void update_window_dim(void);
  SDL_Window *get_window(void) { return w; }
  void free_window(void);

private:
  std::string title;
  SDL_Window *w;
  i32 width, height;
};
