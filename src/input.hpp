#pragma once
#include <cstdint>
#include <memory>
class buffer;

struct input_return {
  input_return(bool run_state, bool exec_result) : success(exec_result), quit(run_state) {}
  bool success;
  bool quit;
};

class key_mapping {
public:
  key_mapping(unsigned int qkey, unsigned int ckey, unsigned int rkey, unsigned int skey, unsigned int mod);

  unsigned int get_quit(void) { return quit_key; }
  unsigned int get_cancel(void) { return cancel_key; }
  unsigned int get_replace(void) { return replace_key; }
  unsigned int get_search(void) { return search_key; }
  unsigned int get_mod(void) { return mod_key; }

private:
  unsigned int quit_key;
  unsigned int cancel_key;
  unsigned int replace_key;
  unsigned int search_key;
  unsigned int mod_key;
};

class input_tree {
public:
  input_tree(unsigned int qkey, unsigned int ckey, unsigned int rkey, unsigned int skey, unsigned int mod);
  ~input_tree() = default;
  input_return parse_input(unsigned int keysym, unsigned int modmask, std::shared_ptr<buffer> buf);
  bool has_modifier(unsigned int modmask);
  bool is_control_code(unsigned int keysym);
  bool is_movement_code(unsigned int keysym);

  input_return control_exec(unsigned int keysym, std::shared_ptr<buffer> buf);
  input_return modifier_exec(unsigned int keysym, std::shared_ptr<buffer> buf);
  input_return movement_exec(unsigned int keysym, std::shared_ptr<buffer> buf);
  input_return default_exec(unsigned int keysym, std::shared_ptr<buffer> buf);

private:
  key_mapping maps;
};
