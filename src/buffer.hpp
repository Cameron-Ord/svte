#pragma once
#include "alias.hpp"
#include <cassert>
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
  std::vector<uint32_t> char_insert(int &x, uint32_t character, std::vector<uint32_t> line);
  std::vector<uint32_t> char_replace(const int &x, uint32_t character, std::vector<uint32_t> line);
  std::vector<uint32_t> char_delete(const int &x, std::vector<uint32_t> line);
  std::vector<uint32_t> char_remove(int &x, std::vector<uint32_t> line);

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
  void save_buffer_file(void);

  bool buf_insert(uint32_t character);
  bool buf_replace(uint32_t character);
  bool buf_delete(void);
  bool buf_remove(void);

  const uint32_t *char_at_cursor(void) const;

  const vec_2d_ptr const_buf(void) const { return contents; }

  bool fits(int val, int max) const;
  bool yfits(int bufsize) const;
  bool xfits(int linesize) const;

  int get_curs_y(void) { return cursor_y; }
  int get_curs_x(void) { return cursor_x; }

  const buf_mutator &get_mutator(void) const { return mutator; }
  const buf_history &get_history(void) const { return history; }
  unsigned int get_id(void) { return id; }

  bool mv_left(unsigned int amount);
  bool mv_right(unsigned int amount);
  bool mv_up(unsigned int amount);
  bool mv_down(unsigned int amount);

private:
  const unsigned int id;
  const std::string filepath;
  int cursor_x, cursor_y;
  buf_mutator mutator;
  buf_history history;
  vec_2d_ptr contents;
};
