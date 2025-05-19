#ifndef SDL2_WINDOW_HPP
#define SDL2_WINDOW_HPP

#include <SDL2/SDL_video.h>

struct WindowPartition{
    int cmd_box_x, cmd_box_y;
    int cmd_box_w, cmd_box_h;

    int buf_box_x, buf_box_y;
    int buf_box_w, buf_box_h;
};

class Window {
    public:
        Window(void);
        ~Window(void);
        void win_set_err(const int errval) { error = errval; }
        int win_get_err(void) { return error; }
        int win_create_window(const int flags);
        Window& win_update_window_values(void);
        void win_show_window(void);
        void win_hide_window(void);
        void win_set_window(SDL_Window *w) { win = w; }
        SDL_Window *win_get_window(void) { return win; }
        void win_set_resizeable(const SDL_bool opt);
        const int win_width(void) { return width; }
        const int win_height(void) { return height; }
        void win_dft_partition(const int& rblock, const int& vpadding);
        const WindowPartition* const _wp(void) const { return &partitions; }

    private:
        int error;
        SDL_Window *win;
        int width, height;
        WindowPartition partitions;
};

#endif
