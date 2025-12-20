#pragma once
#include "alias.hpp"
#include <cassert>
#include <string>

enum EDITOR_CONSTANTS {
  INSERT_MODE = 0,
  REPLACE_MODE = 1,
  MAX_HISTORY = 12,
};

struct coordinates {
  coordinates(int cx, int cy) : x(cx), y(cy) {}
  int x, y;
  int xoffset, yoffset;
};



class buf_history {
public:
  ~buf_history() = default;
  void history_push(const_vec_2d_ptr contents);
  // history_prune can and will only be called inside history_push when
  // required.
private:
  std::vector<const_vec_2d_ptr> contents_history;
  std::vector<const_vec_2d_ptr> &history_prune(void);
};

class buf_cursor {
  public:
    buf_cursor(int cx, int cy) : c(cx, cy) {}
  
    bool within_x(int x, int max);
    bool within_y(int y, int max);

    void x_move_left(int amount, const_vec_2d_ptr contents);
    void x_move_right(int amount, const_vec_2d_ptr contents);
    void y_move_down(int amount, const_vec_2d_ptr contents);
    void y_move_up(int amount, const_vec_2d_ptr contents);

    const coordinates& get_positions(void) {return c;}
  private:
    coordinates c;
};

class buf_mutator {
public:
  buf_mutator(void);
  ~buf_mutator() = default;
  // operates using copy on write, makes tracking history easy
  vec_2d_ptr char_insert(uint32_t character, const_vec_2d_ptr contents);
  vec_2d_ptr char_replace(uint32_t character, const_vec_2d_ptr contents);
  vec_2d_ptr char_delete(const_vec_2d_ptr contents);
  vec_2d_ptr char_remove(const_vec_2d_ptr contents);

  const char &get_insertion_mode(void) const { return mode; }
  void set_insertion_mode(char set) { mode = set; }

  const buf_cursor &get_cursor(void) { return curs; }
  const buf_history &get_hist(void) { return hist; }

private:
  char mode;
  buf_history hist;
  buf_cursor curs;
};


class buffer {
public:
  buffer(int set_id, std::string relative_path, vec_2d_ptr data);
  void save_buffer_file(void);
  const uint32_t *char_at_cursor(void) const;
  const_vec_2d_ptr const_buf(void) const { return contents; }
  void overwrite_contents(vec_2d_ptr new_content);
  bool inserter(uint32_t c);
  const buf_mutator &get_mutator(void) const { return mutator; }
  unsigned int get_id(void) { return id; }
private:
  const unsigned int id;
  const std::string filepath;
  buf_mutator mutator;
  vec_2d_ptr contents;
};
