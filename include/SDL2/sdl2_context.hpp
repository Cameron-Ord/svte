#ifndef SDL2_CONTEXT_HPP
#define SDL2_CONTEXT_HPP

class SDL2_Context{
    public:
        SDL2_Context(void);
        ~SDL2_Context(void);
        void sdl2_set_text_input(const int opt);
        int sdl2_get_fps(void);
        int sdl2_get_err(void);
        int sdl2_open_instance(void);
        void sdl2_set_err(const int err);
        void sdl2_set_run_state(int running);
        int sdl2_get_run_state(void);
        void sdl2_quit_instance(void);

    private:
        int fps_target;
        int error;
        int running;
};

#endif