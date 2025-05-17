#ifndef SDL2_WINDOW_HPP
#define SDL2_WINDOW_HPP

#include <SDL2/SDL_video.h>

class Window {
    public:
        Window(void);
        ~Window(void);
        void win_set_err(const int errval);
        int win_get_err(void);
        int win_create_window(const int flags);
        void win_update_window_values(void);
        void win_show_window(void);
        void win_hide_window(void);
        void win_set_window(SDL_Window *w);
        SDL_Window *win_get_window(void);
        void win_set_resizeable(const SDL_bool opt);
        const int win_width(void);
        const int win_height(void);
    private:
        int error;
        SDL_Window *win;
        int width, height;
};

#endif
