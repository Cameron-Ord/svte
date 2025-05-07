#include "../inc/window.hpp"

#include <SDL2/SDL.h>

Window::Window(void) { 
    s_width = 600, s_height = 400;
    if(!create_window()){
        STATE = WIN_STATE_BAD;
    } else {
        STATE = WIN_STATE_OK;
    }
}

Window::~Window(void) {}

SDL_Window *Window::get_window(void) { return w; }

const void *Window::create_window(void)
{
    const int flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
    int x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED;
    w = SDL_CreateWindow("SVTE", x, y, s_width, s_height, flags);
    if (!w) {
        fprintf(stderr, "Failed to create window! -> %s\n", SDL_GetError());
        return NULL;
    }
    return w;
}

void Window::win_update_dimensions(void)
{
    SDL_GetWindowSize(w, &s_width, &s_height);
}
