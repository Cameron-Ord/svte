#pragma once
#include "../util/alias.hpp"

class buffer;

struct input_return {
  input_return(bool run_state, bool exec_result) : success(exec_result), quit(run_state) {}
  bool success;
  bool quit;
};

class key_mapping {
public:
  key_mapping(u32 qkey, u32 ckey, u32 rkey, u32 skey, u32 mod);

  u32 get_quit(void) { return quit_key; }
  u32 get_cancel(void) { return cancel_key; }
  u32 get_replace(void) { return replace_key; }
  u32 get_search(void) { return search_key; }
  u32 get_mod(void) { return mod_key; }

private:
  u32 quit_key;
  u32 cancel_key;
  u32 replace_key;
  u32 search_key;
  u32 mod_key;
};

class input_tree {
public:
  input_tree(u32 qkey, u32 ckey, u32 rkey, u32 skey, u32 mod);
  ~input_tree() = default;
  input_return parse_input(u32 keysym, u32 modmask, std::shared_ptr<buffer> buf);
  bool has_modifier(u32 modmask);
  bool is_control_code(u32 keysym);
  bool is_movement_code(u32 keysym);

  input_return control_exec(u32 keysym, std::shared_ptr<buffer> buf);
  input_return modifier_exec(u32 keysym, std::shared_ptr<buffer> buf);
  input_return movement_exec(u32 keysym, std::shared_ptr<buffer> buf);
  input_return default_exec(u32 keysym, std::shared_ptr<buffer> buf);
  void send_insert_request(u32 ch, std::shared_ptr<buffer> buf);

private:
  key_mapping maps;
};
