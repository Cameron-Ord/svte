#ifndef SDL2_EVENTS_HPP
#define SDL2_EVENTS_HPP

#include <SDL2/SDL_events.h>
#include <functional>
#include <unordered_map>

class Editor;

typedef struct {
    // Set as a SDL2 keysym
    int LEFT;
    int RIGHT;
    int UP;
    int DOWN;

    int INSERT_MODE;
    int APPEND_MODE;
    int VISUAL_MODE;
    int ESCAPE;

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
        int ev_mainloop_keydown(const int keysym, const int keymod, class Editor *e);
        int ev_mainloop_window_event(void);
        void ev_mainloop_text_input(const char *text, class Editor *e);
        int ev_mainloop_die(void);

        int ev_left(const int& keymod, class Editor *e);
        int ev_right(const int& keymod, class Editor *e);
        int ev_up(const int& keymod, class Editor *e);
        int ev_down(const int& keymod, class Editor *e);

        int ev_visual(const int& keymod, class Editor *e);
        int ev_append(const int& keymod, class Editor *e);
        int ev_insert(const int& keymod, class Editor *e);
        int ev_escape(const int& keymod, class Editor *e);

        Controls ev_init_controls(void);
        void ev_init_keybinds(void);

    private:
        Controls controls;
        std::unordered_map<int, std::function<int(int, class Editor *e)>> binds;
};

#endif