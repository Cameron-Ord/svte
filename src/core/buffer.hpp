
#pragma once
#include "../util/alias.hpp"
#include <cassert>
#include <string>

enum EDITOR_CONSTANTS {
  INSERT_MODE = 0,
  REPLACE_MODE = 1,
  MAX_HISTORY = 12,
};

class buf_history {
public:
  ~buf_history() = default;
  void history_push(const_char_mat_ptr contents);
  // history_prune can and will only be called inside history_push when
  // required.
private:
  std::vector<const_char_mat_ptr> contents_history;
  std::vector<const_char_mat_ptr> &history_prune(void);
};

class buf_cursor {
  public:
    buf_cursor(int pos, int off) : cursor(pos), offset(off)  {}
    bool within_bounds(int pos, int max);

    int rewind_by_new_line(int start_pos, const_char_mat_ptr contents);
    int advance_by_new_line(int start_pos, const_char_mat_ptr contents);

    int line_skip_backwards(int skip_count, const_char_mat_ptr contents);
    int line_skip_forwards(int skip_count, const_char_mat_ptr contents);

    int line_x_offset(const_char_mat_ptr contents);

    int current_line_start(const_char_mat_ptr contents);
    int current_line_end(const_char_mat_ptr contents);

    bool x_move_left(int amount, const_char_mat_ptr contents);
    bool x_move_right(int amount, const_char_mat_ptr contents);
    bool y_move_down(int amount, const_char_mat_ptr contents);
    bool y_move_up(int amount, const_char_mat_ptr contents);

    int get_offset(void) const { return offset; }
    int get_cursor(void) const { return cursor; }

  private:
    int cursor;
    int offset;
};

class buf_mutator {
public:
  buf_mutator(void);
  ~buf_mutator() = default;
  // operates using copy on write, makes tracking history easy
  char_mat_ptr char_insert(uint32_t character, const_char_mat_ptr contents);
  char_mat_ptr char_replace(uint32_t character, const_char_mat_ptr contents);
  char_mat_ptr char_delete(const_char_mat_ptr contents);
  char_mat_ptr char_remove(const_char_mat_ptr contents);
  char_mat_ptr create_newline(const_char_mat_ptr contents);

  const char &get_insertion_mode(void) const { return mode; }
  void set_insertion_mode(char set) { mode = set; }

  buf_cursor &mutable_cursor(void) { return curs; }
  const buf_cursor &const_cursor(void) const { return curs; }
  const buf_history &const_history(void) const { return hist; }

private:
  char mode;
  buf_history hist;
  buf_cursor curs;
};


class buffer {
public:
  buffer(int set_id, std::string relative_path, char_mat_ptr data);
  void save_buffer_file(void);
  bool overwrite_contents(char_mat_ptr new_content);

  buf_mutator &mutable_mutator(void) { return mutator; }
  const buf_mutator &const_mutator(void) const { return mutator; }
  const_char_mat_ptr const_buf(void) const { return contents; }
  unsigned int get_id(void) const { return id; }
private:
  const unsigned int id;
  const std::string filepath;
  buf_mutator mutator;
  char_mat_ptr contents;
};
