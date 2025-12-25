#pragma once
#include <string>

typedef struct SDL_Window SDL_Window;

class window_container {
public:
  window_container(std::string title, int bwidth, int bheight);
  bool init_window(int flags);
  void update_window_dim(void);
  SDL_Window *get_window(void) { return w; }

private:
  std::string title;
  SDL_Window *w;
  int width, height;
};
