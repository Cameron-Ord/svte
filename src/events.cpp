#include "buffer.hpp"
#include "input.hpp"
#include "util.hpp"


key_mapping::key_mapping(unsigned int setmod, unsigned int ckey,
                         unsigned int skey, unsigned int rkey)
    : quit_key(SET_QUIT), cancel_key(ckey), search_key(skey), replace_key(rkey),
      modkey(setmod) {}

input_tree::input_tree(void)
    : maps(0, 0, 0, 0) {}

input_return input_tree::parse_input(uint32_t character, unsigned int modmask,
                                     std::shared_ptr<buffer> buf) {

  logger::log_var("MOD: ", modmask);
  if (is_control_code(character)) {
    return control_exec(character, buf);
  } else if (has_modifier(modmask)) {
    return modifier_exec(character, buf);
  } else if (is_movement_code(character)) {
    return movement_exec(character, buf);
  } else {
    return default_exec(character, buf);
  }
}

bool input_tree::has_modifier(unsigned int modmask) {
  return (modmask & maps.get_mod()) != 0;
}

bool input_tree::is_control_code(uint32_t character) {
   //return character == NCKEY_RETURN || character == NCKEY_DEL ||
          //character == NCKEY_BACKSPACE;
}

bool input_tree::is_movement_code(uint32_t character) {
   //return character == NCKEY_UP || character == NCKEY_DOWN ||
          //character == NCKEY_LEFT || character == NCKEY_RIGHT;
}

input_return input_tree::default_exec(uint32_t character,
                                      std::shared_ptr<buffer> buf) {
  switch (buf->get_mutator().get_insertion_mode()) {
  default:
    return input_return(false, false);
  case INSERT_MODE:
    return input_return(false, buf->buf_insert(character));
  case REPLACE_MODE:
    return input_return(false, buf->buf_replace(character));
  }
}

input_return input_tree::control_exec(uint32_t character,
                                      std::shared_ptr<buffer> buf) {
  switch (character) {
  default:
    return input_return(false, false);
  }
}

input_return input_tree::modifier_exec(uint32_t character,
                                       std::shared_ptr<buffer> buf) {
  logger::log_var("CHAR: ", (char)character);
  switch (character) {
  case SET_QUIT:
    return input_return(true, true);
  default:
    return input_return(false, false);
  }
}

input_return input_tree::movement_exec(uint32_t character,
                                       std::shared_ptr<buffer> buf) {
  switch (character) {
  default:
    return input_return(false, false);
  }
}
