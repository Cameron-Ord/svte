#include "../../include/SDL2/sdl2_window.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

Window::Window(void)
    : error(SDL2_NIL), win(nullptr), width(BASE_WIDTH), height(BASE_HEIGHT)
{
    win_set_err(
        win_create_window(
            SDL_WINDOW_HIDDEN |
            SDL_WINDOW_RESIZABLE));
}

Window::~Window(void)
{
}

int Window::win_get_err(void)
{
    return error;
}

void Window::win_set_err(const int err)
{
    error = err;
}

void Window::win_set_window(SDL_Window *w)
{
    win = w;
}

SDL_Window *Window::win_get_window(void)
{
    return win;
}

const int Window::win_height(void)
{
    return height;
}

const int Window::win_width(void)
{
    return width;
}

int Window::win_create_window(const int flags)
{
    win_set_window(
        SDL_CreateWindow(
            "svte",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            flags));
    if (!win_get_window()) {
        return SDL2_ERR;
    }
    return SDL2_NIL;
}

void Window::win_update_window_values(void)
{
    SDL_GetWindowSize(win, &width, &height);
}

void Window::win_show_window(void)
{
    SDL_ShowWindow(win);
}

void Window::win_hide_window(void)
{
    SDL_HideWindow(win);
}

void Window::win_set_resizeable(const SDL_bool opt)
{
    SDL_SetWindowResizable(win, opt);
}