#include "../buffer.hpp"
#include "../input.hpp"

#include <SDL3/SDL_keycode.h>

key_mapping::key_mapping(unsigned int qkey, unsigned int ckey, unsigned int rkey, unsigned int skey, unsigned int mod)
    : quit_key(qkey), cancel_key(ckey), replace_key(rkey), search_key(skey), mod_key(mod) {}

input_tree::input_tree(unsigned int qkey, unsigned int ckey, unsigned int rkey, unsigned int skey, unsigned int mod)
    : maps(qkey, ckey, skey, rkey, mod) {}


void input_tree::send_insert_request(uint32_t ch, std::shared_ptr<buffer> buf){
  switch(buf->get_mutator().get_insertion_mode()){
    default: 
      break;
    case REPLACE_MODE:
      break;
    case INSERT_MODE:
      
      break;
  }
}
input_return input_tree::parse_input(unsigned int keysym, unsigned int modmask, std::shared_ptr<buffer> buf) {
  // Just doing this to break up the switch statements,
  // and keep the input loop in main() as generic as possible.
  if (is_control_code(keysym)) {
    return control_exec(keysym, buf);
  } else if (has_modifier(modmask)) {
    return modifier_exec(keysym, buf);
  } else if (is_movement_code(keysym)) {
    return movement_exec(keysym, buf);
  }
  return input_return(false, false);
}

bool input_tree::has_modifier(unsigned int modmask) { return (modmask & maps.get_mod()) != 0; }

bool input_tree::is_control_code(unsigned int keysym) {
  return keysym == SDLK_RETURN || keysym == SDLK_DELETE || keysym == SDLK_BACKSPACE ||
         keysym == SDLK_ESCAPE;
}

bool input_tree::is_movement_code(unsigned int keysym) {
  return keysym == SDLK_DOWN || keysym == SDLK_UP || keysym == SDLK_LEFT || keysym == SDLK_RIGHT;
}

input_return input_tree::control_exec(unsigned int keysym, std::shared_ptr<buffer> buf) {
  switch (keysym) {
  default:
    return input_return(false, false);
  }
}

input_return input_tree::modifier_exec(unsigned int keysym, std::shared_ptr<buffer> buf) {
  switch (keysym) {
  default:
    return input_return(false, false);
  }
}

input_return input_tree::movement_exec(unsigned int keysym, std::shared_ptr<buffer> buf) {
  switch (keysym) {
  case SDLK_RIGHT:
 //   return input_return(false, buf->mv_right(1));
  case SDLK_LEFT:
 //   return input_return(false, buf->mv_left(1));
  case SDLK_UP:
 //   return input_return(false, buf->mv_up(1));
  case SDLK_DOWN:
 //   return input_return(false, buf->mv_down(1));
  default:
    return input_return(false, false);
  }
}
