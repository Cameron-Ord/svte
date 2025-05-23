#include "../../include/SDL2/sdl2_window.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

Window::Window(void)
    : error(SDL2_NIL), win(nullptr), width(BASE_WIDTH), height(BASE_HEIGHT), partitions()
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
    const int mono_line = rblock + vpadding;
    partitions.set(PFX_BOX, 
        vpadding, height - mono_line, (rblock * 4) + vpadding, mono_line
    );

    partitions.set(ARG_BOX,
        (rblock * 4) + vpadding, 
        height - mono_line, 
        width - (rblock * 4) + vpadding, 
        mono_line
    );

    partitions.set(FN_BOX,
        vpadding, 0, width, mono_line
    );

    const int buf_block_height = height - (mono_line * 2);
    partitions.set(BUF_BOX, 
        vpadding, mono_line, width, buf_block_height
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
