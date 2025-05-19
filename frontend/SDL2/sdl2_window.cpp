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

void Window::win_dft_partition(const int &rblock, const int &vpadding)
{
    const int reserve_cmd_block = rblock + vpadding;
    const int cmd_box_y = height - reserve_cmd_block;

    partitions.cmd_box_x = 0, partitions.cmd_box_y = cmd_box_y;
    partitions.cmd_box_w = width, partitions.cmd_box_h = reserve_cmd_block;

    const int buf_box_h = height - reserve_cmd_block;

    partitions.buf_box_x = 0, partitions.buf_box_y = 0;
    partitions.buf_box_w = width, partitions.buf_box_h = buf_box_h;
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

Window &Window::win_update_window_values(void)
{
    SDL_GetWindowSize(win, &width, &height);
    return *this;
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