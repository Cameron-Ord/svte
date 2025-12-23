#include "buffer.hpp"
#include "../util/util.hpp"



buf_mutator::buf_mutator(void) : mode(INSERT_MODE), hist(buf_history()), curs(0, 0) {}

buffer::buffer(int set_id, std::string relative_path, char_mat_ptr data)
    : id(set_id), filepath(relative_path), mutator(buf_mutator()), contents(data) {}


bool buffer::overwrite_contents(char_mat_ptr cptr) {
  if(!cptr){
    return false;
  }

  if(cptr){
    contents = cptr;
  }
  return contents != cptr;
}
void buffer::save_buffer_file(void) { text_io::write_text_file(filepath, contents); }

bool buf_cursor::x_move_left(int amount, const_char_mat_ptr contents){
}

bool buf_cursor::y_move_up(int amount, const_char_mat_ptr contents){
}

bool buf_cursor::y_move_down(int amount, const_char_mat_ptr contents){
}

bool buf_cursor::x_move_right(int amount, const_char_mat_ptr contents){
}

char_mat_ptr buf_mutator::create_newline(const_char_mat_ptr contents){
}

char_mat_ptr buf_mutator::char_insert(uint32_t character, const_char_mat_ptr contents) {
}

char_mat_ptr buf_mutator::char_replace(uint32_t character, const_char_mat_ptr contents) {
}

// not impl
char_mat_ptr buf_mutator::char_remove(const_char_mat_ptr contents) {
}

char_mat_ptr buf_mutator::char_delete(const_char_mat_ptr contents) {
}

void buf_history::history_push(const_char_mat_ptr contents)  {
  while (contents_history.size() > MAX_HISTORY) {
    history_prune();
  }

  if (contents_history.size() < MAX_HISTORY) {
    contents_history.push_back(contents);
  } else {
    history_prune().push_back(contents);
  }
}

std::vector<const_char_mat_ptr> &buf_history::history_prune(void) {
  if (!contents_history.empty()) {
    contents_history.erase(contents_history.begin());
  }
  return contents_history;
}
