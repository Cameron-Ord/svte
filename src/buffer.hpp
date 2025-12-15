#pragma once
#include "alias.hpp"
#include <string>

enum EDITOR_CONSTANTS {
  INSERT_MODE = 0,
  REPLACE_MODE = 1,
  MAX_HISTORY = 12,
};

class buf_mutator {
public:
  buf_mutator(void);
  ~buf_mutator() = default;
  // operates using copy on write, makes tracking history easy
  vec_2d_ptr char_insert(int &x, int &y, uint32_t character,
                         vec_2d_ptr contents);
  vec_2d_ptr char_replace(int &x, int &y, uint32_t character,
                          vec_2d_ptr contents);
  vec_2d_ptr char_delete(int &x, int &y, vec_2d_ptr contents);
  vec_2d_ptr char_remove(int &x, int &y, vec_2d_ptr contents);

  const char &get_insertion_mode(void) const { return mode; }
  void set_insertion_mode(char set) { mode = set; }

private:
  char mode;
};

class buf_history {
public:
  ~buf_history() = default;
  void history_push(vec_2d_ptr contents);
  // history_prune can and will only be called inside history_push when
  // required.
private:
  std::vector<vec_2d_ptr> contents_history;
  std::vector<vec_2d_ptr> &history_prune(void);
};

class buffer {
public:
  buffer(int set_id, std::string relative_path, vec_2d_ptr data);
  ~buffer() = default;
  void save_buffer_file(void);

  bool buf_insert(uint32_t character);
  bool buf_replace(uint32_t character);
  bool buf_delete(void);
  bool buf_remove(void);

  const vec_2d_ptr const_buf(void) const { return contents; }

  const buf_mutator &get_mutator(void) const { return mutator; }
  const buf_history &get_history(void) const { return history; }
  unsigned int get_id(void) { return id; }

private:
  const unsigned int id;
  const std::string filepath;
  int cursor_x, cursor_y;
  buf_mutator mutator;
  buf_history history;
  vec_2d_ptr contents;
};
