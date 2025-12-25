
#include "window.hpp"
#include "../util/util.hpp"
#include <SDL3/SDL.h>

window_container::window_container(std::string btitle, int bwidth, int bheight)
    : title(btitle), w(nullptr), width(bwidth), height(bheight) {
  logger::log_str("Title: ", title);
  logger::log_int("Initial width: ", width);
  logger::log_int("Initial height: ", height);
}

bool window_container::init_window(int flags) {
  SDL_Window *tmp = SDL_CreateWindow(title.c_str(), width, height, flags);

  if (!tmp) {
    logger::log_str("SDL failed to create window: ", SDL_GetError());
    return false;
  }

  w = tmp;
  return true;
}

void window_container::update_window_dim(void) {
  int twidth, theight;
  SDL_GetWindowSize(w, &twidth, &theight);
  width = twidth, height = theight;
}
