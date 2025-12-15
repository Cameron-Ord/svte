#pragma once
#include <memory>
class buffer;

enum KEY_CONSTANTS {
  SET_QUIT = 'Q',
  SET_REPLACE = 'R',
  SET_SEARCH = 'Z',
  SET_CANCEL = 'X',
};

struct input_return {
  input_return(bool run_state, bool exec_result)
      : success(exec_result), quit(run_state) {}
  bool success;
  bool quit;
};

class key_mapping {
public:
  key_mapping(unsigned int setmod, unsigned int ckey, unsigned int skey,
              unsigned int rkey);
  unsigned int get_mod(void) { return modkey; }

private:
  unsigned int quit_key;
  unsigned int cancel_key;
  unsigned int search_key;
  unsigned int replace_key;
  unsigned int modkey;
};

class input_tree {
public:
  input_tree(void);
  ~input_tree() = default;
  input_return parse_input(uint32_t character, unsigned int modmask,
                           std::shared_ptr<buffer> buf);
  bool has_modifier(unsigned int modmask);
  bool is_control_code(uint32_t character);
  bool is_movement_code(uint32_t character);

  input_return control_exec(uint32_t character, std::shared_ptr<buffer> buf);
  input_return modifier_exec(uint32_t character, std::shared_ptr<buffer> buf);
  input_return movement_exec(uint32_t character, std::shared_ptr<buffer> buf);
  input_return default_exec(uint32_t character, std::shared_ptr<buffer> buf);

private:
  key_mapping maps;
};
