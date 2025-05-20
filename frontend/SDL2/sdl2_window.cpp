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
    const int cmd_block_height = rblock + vpadding;
    partitions.set(CMD_BOX, 
        vpadding, height - cmd_block_height, width * 0.40, cmd_block_height
    );

    const int buf_block_height = height - cmd_block_height;
    partitions.set(BUF_BOX, 
        vpadding, 0, width, buf_block_height
    );

    partitions.set(STATUS_NOX,
        vpadding + partitions.get(0).w, 
        height - cmd_block_height, width * 0.25, cmd_block_height
    );
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