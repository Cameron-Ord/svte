#ifndef SDL2_EVENTS_HPP
#define SDL2_EVENTS_HPP

#include <SDL2/SDL_events.h>
#include <functional>
#include <unordered_map>

typedef struct {
    // Set as a SDL2 keysym
    int LEFT;
    int RIGHT;
    int UP;
    int DOWN;

    int ACTION_CMD;
    int ACTION_MOD;
} Controls;

typedef struct {
    int keysym;
    int keymod;
} KeyC;

class KeyEvent{
    public:
        KeyEvent(void);
        int ev_mainloop_poll_event_type(SDL_Event* const e);
        void ev_mainloop_keydown(const int keysym, const int keymod);
        int ev_mainloop_window_event(void);
        int ev_mainloop_text_input(void);
        int ev_mainloop_die(void);

        void ev_left(const int& keymod);
        void ev_right(const int& keymod);
        void ev_up(const int& keymod);
        void ev_down(const int& keymod);
        Controls ev_init_controls(void);
        void ev_init_keybinds(void);

    private:
        Controls controls;
        std::unordered_map<int, std::function<void(int)>> binds;
};

#endif