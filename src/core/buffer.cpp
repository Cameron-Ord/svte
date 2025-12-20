#include "../buffer.hpp"
#include "../util.hpp"



buf_mutator::buf_mutator(void) : mode(INSERT_MODE), hist(buf_history()), curs(0, 0) {}

buffer::buffer(int set_id, std::string relative_path, vec_2d_ptr data)
    : id(set_id), filepath(relative_path), mutator(buf_mutator()), contents(data) {}

void buffer::save_buffer_file(void) { text_io::write_text_file(filepath, contents); }

void buf_cursor::x_move_left(int amount, const_vec_2d_ptr contents){
  const int bufmax = contents->size();
  if(within_y(c.y, bufmax)){
    const_bufline line = &(*contents)[c.y];
    const int linemax = line->size();
    if(within_x(c.x - amount, linemax)){
      c.x -= amount;
    } else {
      c.x = 0;
    }
  }
}

void buf_cursor::y_move_up(int amount, const_vec_2d_ptr contents){
}

void buf_cursor::y_move_down(int amount, const_vec_2d_ptr contents){
}

void buf_cursor::x_move_right(int amount, const_vec_2d_ptr contents){
  const int bufmax = contents->size();
  if(within_y(c.y, bufmax)){
    const_bufline line = &(*contents)[c.y];
    const int linemax = line->size();
    if(within_x(c.x + amount, linemax)){
      c.x += amount;
    } else {
      c.x = linemax;
    }
  }
}

bool buf_cursor::within_x(int x, int max){
  return x >= 0 && x <= max;
}

bool buf_cursor::within_y(int y, int max){
  return y >= 0 && y < max;
}

// These functions do mutate the buffer, but since -
// I want to pass down a constant snapshot pointer of -
// the buffer as history before returning a newly allocated pointer, this is how it is.
vec_2d_ptr buf_mutator::char_insert(uint32_t character, const_vec_2d_ptr contents) {
  vec_2d_ptr overwrite = std::make_shared<vec_2d>(*contents);
  hist.history_push(contents);
  return overwrite;
}

vec_2d_ptr buf_mutator::char_replace(uint32_t character, const_vec_2d_ptr contents) {
  vec_2d_ptr overwrite = std::make_shared<vec_2d>(*contents);
  hist.history_push(contents);
  return overwrite;
}

vec_2d_ptr buf_mutator::char_remove(const_vec_2d_ptr contents) {
  vec_2d_ptr overwrite = std::make_shared<vec_2d>(*contents);
  hist.history_push(contents);
  return overwrite;
}

// not impl
vec_2d_ptr buf_mutator::char_delete(const_vec_2d_ptr contents) {
  vec_2d_ptr overwrite = std::make_shared<vec_2d>(*contents);
  hist.history_push(contents);
  return overwrite;
}

void buf_history::history_push(const_vec_2d_ptr contents)  {
  while (contents_history.size() > MAX_HISTORY) {
    history_prune();
  }

  if (contents_history.size() < MAX_HISTORY) {
    contents_history.push_back(contents);
  } else {
    history_prune().push_back(contents);
  }
}

std::vector<const_vec_2d_ptr> &buf_history::history_prune(void) {
  if (!contents_history.empty()) {
    contents_history.erase(contents_history.begin());
  }
  return contents_history;
}
