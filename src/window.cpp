#include "../inc/window.hpp"

#include <SDL2/SDL_video.h>
#include <cstdio>
#include <cstdlib>

#include <SDL2/SDL.h>

Window::Window(void) { dimensions.x = 600, dimensions.y = 400; }

Window::~Window(void) {}

SDL_Window *Window::get_window(void) { return w; }

const void *Window::create_window(void) {
  const int flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
  int x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED;
  w = SDL_CreateWindow("SVTE", x, y, dimensions.x, dimensions.y, flags);
  if (!w) {
    fprintf(stderr, "Failed to create window! -> %s\n", SDL_GetError());
    return NULL;
  }
  return w;
}

Vec2i *Window::get_dimensions(void) { return &dimensions; }
void Window::win_update_dimensions(void) {
  int *wth = &dimensions.x, *hgt = &dimensions.y;
  SDL_GetWindowSize(w, wth, hgt);
}
