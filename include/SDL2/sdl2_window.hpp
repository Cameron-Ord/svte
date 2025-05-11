#ifndef SDL2_WINDOW_HPP
#define SDL2_WINDOW_HPP

#define BASE_WIDTH 400
#define BASE_HEIGHT 300

typedef struct SDL_Window SDL_Window;

class Window {
    public:
        Window(void);
        ~Window(void);
        int win_create_window(const int flags);
        int win_update_window_values(void);
        void win_show_window(void);
        void win_hide_window(void);
        void win_set_resizeable(const int opt);
    private:
        SDL_Window *win;
        int width, height;
};

#endif
