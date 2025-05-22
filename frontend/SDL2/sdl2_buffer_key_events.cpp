#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_events.hpp"

#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_event_mgr.hpp"

EventResult KeyEvent::prev_buffer(const int &keymod, Editor *e, const int32_t id){
    if(keymod & controls.META_MOD){
        e->ed_set_current_id(e->ed_prev_id());
        return EventResult(BUFFER_CHANGED, NO_OPTION, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::next_buffer(const int &keymod, Editor *e, const int32_t id){
    if(keymod & controls.META_MOD){
        e->ed_set_current_id(e->ed_next_id());
        return EventResult(BUFFER_CHANGED, NO_OPTION, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}
