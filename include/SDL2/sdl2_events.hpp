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

typedef struct {
    // Id the event belongs to.
    int id;
    int input_opt;
    int reported_cursor_loc;
    int reported_row_loc;
} EventRet;

class KeyEvent{
    public:
        KeyEvent(void);
        int ev_mainloop_poll_event_type(SDL_Event* const e);
        EventRet ev_mainloop_keydown(const int keysym, const int keymod, class Editor *e, const int id);
        int ev_mainloop_window_event_type(const int windowevent);
        void ev_mainloop_text_input(const char *text, class Editor *e, const int id);
        int ev_mainloop_die(void);

        EventRet ev_left(const int& keymod, class Editor *e, const int id);
        EventRet ev_right(const int& keymod, class Editor *e, const int id);
        EventRet ev_up(const int& keymod, class Editor *e, const int id);
        EventRet ev_down(const int& keymod, class Editor *e, const int id);

        EventRet ev_visual(const int& keymod, class Editor *e, const int id);
        EventRet ev_append(const int& keymod, class Editor *e, const int id);
        EventRet ev_insert(const int& keymod, class Editor *e, const int id);
        EventRet ev_escape(const int& keymod, class Editor *e, const int id);

        Controls ev_init_controls(void);
        void ev_init_keybinds(void);
    private:
        Controls controls;
        std::unordered_map<int, std::function<EventRet(int, class Editor *e, int)>> binds;
};

#endif