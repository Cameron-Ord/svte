#ifndef SDL2_EVENTS_HPP
#define SDL2_EVENTS_HPP

#include <functional>
#include <unordered_map>

class Editor;

typedef struct
{
    // Set as a SDL2 keysym
    int LEFT;
    int RIGHT;
    int UP;
    int DOWN;
    int RETURN;
    int BACKSPACE;
    int DELETE;

    int INSERT_MODE;
    int APPEND_MODE;
    int SELECTION_MODE;
    int NAV_MODE;
    int ACTION_CMD;

    int ACTION_MOD;
} Controls;

typedef struct
{
    int keysym;
    int keymod;
} KeyC;

class KeyEvent
{
  public:
    KeyEvent(void);
    int ev_mainloop_poll_event_type(SDL_Event *const e);
    int ev_mainloop_die(void);
    class EventResult ev_mainloop_window_event_type(const int windowevent);
    class EventResult ev_mainloop_text_input(const char *text, class Editor *e, const int32_t id);
    class EventResult ev_mainloop_keydown(const int keysym, const int keymod, class Editor *e);
    class EventResult ev_left(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_right(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_up(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_down(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_visual(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_append(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_insert(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_escape(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_backspace(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_return(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_delete(const int &keymod, class Editor *e, const int32_t id);
    class EventResult ev_cmd(const int &keymod, class Editor *e, const int32_t id);
    Controls ev_init_controls(void);
    void ev_init_keybinds(void);

  private:
    Controls controls;
    std::unordered_map<int, std::function<class EventResult(int, class Editor *e, int32_t)>> binds;
    // Stores meta data from the last input event.
    // IE. the buffer ID of the related input, values/returns of functions, etc
    
};

#endif
