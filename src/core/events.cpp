#include "../buffer.hpp"
#include "../input.hpp"

#include <SDL2/SDL_keycode.h>

key_mapping::key_mapping(unsigned int qkey, unsigned int ckey, unsigned int rkey, unsigned int skey , unsigned int mod)
    : quit_key(qkey), cancel_key(ckey), replace_key(rkey), search_key(skey), mod_key(mod) {}

input_tree::input_tree(unsigned int qkey, unsigned int ckey, unsigned int rkey, unsigned int skey, unsigned int mod)
    : maps(qkey, ckey, skey, rkey, mod) {}

input_return input_tree::parse_input(uint32_t character, unsigned int modmask, std::shared_ptr<buffer> buf) {
  // Just doing this to break up the switch statements,
  // and keep the input loop in main() as generic as possible.
  if (is_control_code(character)) {
    return control_exec(character, buf);
  } else if (has_modifier(modmask)) {
    return modifier_exec(character, buf);
  } else if (is_movement_code(character)) {
    return movement_exec(character, buf);
  }
  return input_return(false, false);
}

bool input_tree::has_modifier(unsigned int modmask) { return (modmask & maps.get_mod()) != 0; }

bool input_tree::is_control_code(uint32_t character) {
  return character == SDLK_RETURN || character == SDLK_DELETE || character == SDLK_BACKSPACE;
}

bool input_tree::is_movement_code(uint32_t character) {
  return character == SDLK_DOWN || character == SDLK_UP || character == SDLK_LEFT || character == SDLK_RIGHT;
}

input_return input_tree::control_exec(uint32_t character, std::shared_ptr<buffer> buf) {
  switch (character) {
  default:
    return input_return(false, false);
  }
}

input_return input_tree::modifier_exec(uint32_t character, std::shared_ptr<buffer> buf) {
  switch (character) {
  default:
    return input_return(false, false);
  }
}

input_return input_tree::movement_exec(uint32_t character, std::shared_ptr<buffer> buf) {
  switch (character) {
  default:
    return input_return(false, false);
  }
}
