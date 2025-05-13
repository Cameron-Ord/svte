#ifndef SDL2_WINDOW_HPP
#define SDL2_WINDOW_HPP

#include <SDL2/SDL_video.h>

class Window {
    public:
        Window(void);
        ~Window(void);
        void win_set_err(const int err);
        int win_get_err(void);
        int win_create_window(const int flags);
        void win_update_window_values(void);
        void win_show_window(void);
        void win_hide_window(void);
        SDL_Window *win_get_window(void);
        void win_set_resizeable(const SDL_bool opt);
        const int* const win_width_ptr(void);
        const int* const win_height_ptr(void);
    private:
        int error;
        SDL_Window *win;
        int width, height;
};

#endif
