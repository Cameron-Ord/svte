#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_error_codes.hpp"

KeyEvent::KeyEvent(void) : controls(ev_init_controls())
{
    ev_init_keybinds();
    std::cout << "KeyEvents initialized" << std::endl;
}
Controls KeyEvent::ev_init_controls(void)
{
    Controls c = {
        controls.LEFT = SDLK_h,
        controls.RIGHT = SDLK_l,
        controls.UP = SDLK_k,
        controls.DOWN = SDLK_j,
        controls.RETURN = SDLK_RETURN,
        controls.BACKSPACE = SDLK_BACKSPACE,
        controls.DELETE = SDLK_DELETE,
        
        controls.INSERT_MODE = SDLK_i,
        controls.APPEND_MODE = SDLK_a,
        controls.VISUAL_MODE = SDLK_v,
        controls.ESCAPE = SDLK_ESCAPE,

        controls.ACTION_CMD = SDLK_c,
        controls.ACTION_MOD = KMOD_LCTRL,
    };
    return c;
}

//https://en.cppreference.com/w/cpp/language/lambda
void KeyEvent::ev_init_keybinds(void)
{
    binds.insert({controls.LEFT, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_left(mod, e, id); }});
    binds.insert({controls.RIGHT, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_right(mod, e, id); }});
    binds.insert({controls.UP, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_up(mod, e, id); }});
    binds.insert({controls.DOWN, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_down(mod, e, id); }});
    
    binds.insert({controls.RETURN, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_return(mod, e, id); }});
    binds.insert({controls.BACKSPACE, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_backspace(mod, e, id); }});
    binds.insert({controls.DELETE, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_delete(mod, e, id); }});
    
    binds.insert({controls.INSERT_MODE, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_insert(mod, e, id); }});
    binds.insert({controls.APPEND_MODE, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_append(mod, e, id); }});
    binds.insert({controls.ESCAPE, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_escape(mod, e, id); }});
    binds.insert({controls.VISUAL_MODE, [this](int mod, class Editor *e, const int32_t id) -> class EventResult { return ev_visual(mod, e, id); }});
}

class EventResult KeyEvent::ev_backspace(const int& keymod, class Editor *e, const int32_t id){
    class Buffer *buf = e->ed_fetch_buffer(id);
    if(!buf){
        return EventResult("nobuffer", "noopt", id);
    }
    
    if(e->ed_get_mode() == INSERT && SDL_IsTextInputActive()){
        buf->buf_rmv_before();
        return EventResult("textinsert", "noopt", id);

    } else if (e->ed_get_mode() == NVISUAL && !SDL_IsTextInputActive()){
        buf->buf_mv_col(-1);
        return EventResult("move", "noopt", id);
    }

    return EventResult("notbound", "noopt", id);

}

class EventResult KeyEvent::ev_return(const int& keymod, class Editor *e, const int32_t id){
    class Buffer *buf = e->ed_fetch_buffer(id);
    if(!buf){
        return EventResult("nobuffer", "noopt", id);
    }
    
    if(e->ed_get_mode() == INSERT && SDL_IsTextInputActive()){
        buf->buf_new_line();
        return EventResult("textinsert", "noopt", id);

    } else if (e->ed_get_mode() == NVISUAL && !SDL_IsTextInputActive()){
        buf->buf_mv_row(1);
        return EventResult("move", "noopt", id);
    }

    return EventResult("notbound", "noopt", id);
}

class EventResult KeyEvent::ev_delete(const int& keymod, class Editor *e, const int32_t id){
    class Buffer *buf = e->ed_fetch_buffer(id);
    if(!buf){
        return EventResult("nobuffer", "noopt", id);
    }

    if(e->ed_get_mode() == INSERT || e->ed_get_mode() == NVISUAL){
        buf->buf_rmv_at();
        return EventResult("textinsert", "noopt", id);
    }

    return EventResult("notbound", "noopt", id);
}

class EventResult KeyEvent::ev_left(const int &keymod, class Editor *e, const int32_t id)
{
    class Buffer *buf = e->ed_fetch_buffer(id);
    if(!buf){
        return EventResult("nobuffer", "noopt", id);
    }

    if (e->ed_get_mode() == NVISUAL && !SDL_IsTextInputActive()) {
        buf->buf_mv_col(-1);
    }
    return EventResult("move", "noopt", id);
}

class EventResult KeyEvent::ev_up(const int &keymod, class Editor *e, const int32_t id)
{
    class Buffer *buf = e->ed_fetch_buffer(id);
    if(!buf){
        return EventResult("nobuffer", "noopt", id);
    }

    if (e->ed_get_mode() == NVISUAL && !SDL_IsTextInputActive()) {
        buf->buf_update_col(buf->buf_mv_row(-1));
    }
    return EventResult("move", "noopt", id);
}

class EventResult KeyEvent::ev_down(const int &keymod, class Editor *e, const int32_t id)
{
    class Buffer *buf = e->ed_fetch_buffer(id);
    if(!buf){
        return EventResult("nobuffer", "noopt", id);
    }
    
    if (e->ed_get_mode() == NVISUAL && !SDL_IsTextInputActive()) {
        buf->buf_update_col(buf->buf_mv_row(1));
    }
    return EventResult("move", "noopt", id);
}

class EventResult KeyEvent::ev_right(const int &keymod, class Editor *e, const int32_t id)
{    
    class Buffer *buf = e->ed_fetch_buffer(id);
    if(!buf){
        return EventResult("nobuffer", "noopt", id);
    }

    if (e->ed_get_mode() == NVISUAL && !SDL_IsTextInputActive()) {
        buf->buf_mv_col(1);
    }
    return EventResult("move", "noopt", id);
}

class EventResult KeyEvent::ev_visual(const int &keymod, class Editor *e, const int32_t id)
{
    if (e->ed_get_mode() == NVISUAL && !SDL_IsTextInputActive()) {
        e->ed_set_mode(VISUAL);
    }
    return EventResult("notbound", "noopt", id);
}

class EventResult KeyEvent::ev_append(const int &keymod, class Editor *e, const int32_t id)
{
    if (e->ed_get_mode() == NVISUAL && !SDL_IsTextInputActive()) {
        e->ed_set_mode(APPEND);
        return EventResult("chsdl2textinput", "start", id);
    }
    return EventResult("notbound", "noopt", id);
}

class EventResult KeyEvent::ev_insert(const int &keymod, class Editor *e, const int32_t id)
{
    if (e->ed_get_mode() == NVISUAL && !SDL_IsTextInputActive()) {
        e->ed_set_mode(INSERT);
        return EventResult("chsdl2textinput", "start", id);
    }
    return EventResult("notbound", "noopt", id);
}

class EventResult KeyEvent::ev_escape(const int &keymod, class Editor *e, const int32_t id)
{
    e->ed_set_mode(NVISUAL);
    if(SDL_IsTextInputActive()){
        return EventResult("chsdl2textinput", "stop", id);
    }
    return EventResult("notbound", "noopt", id);
}

class EventResult KeyEvent::ev_mainloop_keydown(const int keysym, const int keymod, class Editor *e)
{
    const int32_t id = e->ed_get_current_id();
    std::unordered_map<int, std::function<class EventResult(int, class Editor *e, int)>>::const_iterator it;
    it = binds.find(keysym);
    if (it != binds.end()) {
        return it->second(keymod, e, id);
    }
    return EventResult("notbound", "noopt", id);
}

int KeyEvent::ev_mainloop_poll_event_type(SDL_Event *const e)
{
    while (SDL_PollEvent(e)) {
        return e->type;
    }
    return SDL2_NIL;
}
//This will probably have to have its own return type later possibly, but for now - checking these two events for size changes is fine.
class EventResult KeyEvent::ev_mainloop_window_event_type(const int windowevent)
{
    switch (windowevent) {
    default:
    {
        return EventResult("notbound", "noopt", SDL2_NIL);
    }
    case SDL_WINDOWEVENT_RESIZED:
    {
        return EventResult("resized", "noopt", SDL2_NIL);
    } break;

    case SDL_WINDOWEVENT_SIZE_CHANGED:
    {
        return EventResult("sizechanged", "noopt", SDL2_NIL);
    } break;
    }
}

class EventResult KeyEvent::ev_mainloop_text_input(const char *text, class Editor *e, const int32_t id)
{
    const size_t len = strlen(text);
    class Buffer *buf = e->ed_fetch_buffer(id);
    if(!buf){
        return EventResult("nobuffer", "noopt", id);
    }

    for (size_t i = 0; i < len; i++) {
        buf->buf_ins_char(text[i]);
    }
    return EventResult("textinsert", "noopt", id);
}

int KeyEvent::ev_mainloop_die(void) { return NO_RUN; }
