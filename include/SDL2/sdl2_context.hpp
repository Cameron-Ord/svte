#ifndef SDL2_CONTEXT_HPP
#define SDL2_CONTEXT_HPP

#include "sdl2_window.hpp"
#include "sdl2_events.hpp"
#include "sdl2_renderer.hpp"

class SDL2_Initializer{
    public:
        SDL2_Initializer(void);
        int init_sdl2_instance(void);
        int init_get_err(void);
        void init_set_err(const int errval);
        int init_open_ttf(void);
    private:
        int error;
};

class SDL2_Context{
    public:
        SDL2_Context(void);
        ~SDL2_Context(void);
        int sdl2_get_fps(void);
        int sdl2_get_err(void);
        void sdl2_set_err(const int err);
        void sdl2_set_run_state(int running);
        int sdl2_get_run_state(void);
        class Window* sdl2_get_win(void) { return &win; }
        class Renderer* sdl2_get_rend(void) { return &rend; }
        class KeyEvent* sdl2_get_keyevent(void) { return &events; }
        void sdl2_mainloop_event_branch(const class EventResult& er);
        void sdl2_window_size_update(const class EventResult& er);
        void sdl2_rndr_cursor_update(const class EventResult& er);
        void sdl2_input_chmode(const class EventResult& er);
        void sdl2_init_proxy_fncs(void);

    private:
        int fps_target;
        int error;
        int running;
        class Window win;
        class Renderer rend;
        class KeyEvent events;
        std::unordered_map<std::string, std::function<void(const class EventResult&)>> branches;
};

#endif