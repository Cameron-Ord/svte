#include "../svte.hpp"
#include "../util.hpp"
#include <SDL2/SDL.h>

window_container::window_container(std::string btitle, int bwidth, int bheight)
    : title(btitle), w(nullptr), width(bwidth), height(bheight) {
  logger::log_var("Title: ", title);
  logger::log_var("Initial width: ", width);
  logger::log_var("Initial height: ", height);
}

bool window_container::init_window(int flags) {
  const int centered = SDL_WINDOWPOS_CENTERED;
  SDL_Window *tmp = SDL_CreateWindow(title.c_str(), centered, centered, width, height, flags);

  if (!tmp) {
    logger::log_var("SDL failed to create window: ", SDL_GetError());
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
