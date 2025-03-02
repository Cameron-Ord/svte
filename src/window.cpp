#include "../inc/window.hpp"

#include <SDL2/SDL_video.h>
#include <cstdio>
#include <cstdlib>

#include <SDL2/SDL.h>

Window window_instance;

Window::Window(void) { dimensions.x = 600, dimensions.y = 400; }

Window::~Window(void) {}

SDL_Window *Window::get_window(void) { return window_instance.w; }

const void *Window::create_window(void) {
  int x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED;
  w = SDL_CreateWindow("SVTE", x, y, dimensions.x, dimensions.y,
                       SDL_WINDOW_HIDDEN);
  if (!w) {
    fprintf(stderr, "Failed to create window! -> %s\n", SDL_GetError());
    return NULL;
  }
  return w;
}

Vec2i Window::get_dimensions(void) { return dimensions; }

Window *get_window_inst(void) { return &window_instance; }
