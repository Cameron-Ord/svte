#ifndef SDL2_EVENTS_HPP
#define SDL2_EVENTS_HPP

#include <SDL2/SDL_events.h>

class KeyEvent{
    public:
        int ev_mainloop_poll_event_type(SDL_Event* const e);
        int ev_mainloop_keydown(const int keysym, const int keymod);
        int ev_mainloop_window_event(void);
        int ev_mainloop_text_input(void);
        int ev_mainloop_die(void);

    private:
};

#endif