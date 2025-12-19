#include "../buffer.hpp"
#include "../util.hpp"



buf_mutator::buf_mutator(void) : mode(INSERT_MODE), hist(buf_history()), curs(0, 0) {}

buffer::buffer(int set_id, std::string relative_path, vec_2d_ptr data)
    : id(set_id), filepath(relative_path), mutator(buf_mutator()), contents(data) {}

void buffer::save_buffer_file(void) { text_io::write_text_file(filepath, contents); }

std::vector<uint32_t> buf_mutator::char_insert(uint32_t character, vec_2d_ptr contents) {
}

std::vector<uint32_t> buf_mutator::char_replace(uint32_t character, vec_2d_ptr contents) {
}

std::vector<uint32_t> buf_mutator::char_remove(vec_2d_ptr contents) {
}

// not impl
std::vector<uint32_t> buf_mutator::char_delete(vec_2d_ptr contents) {
}

void buf_history::history_push(vec_2d_ptr contents) {
  while (contents_history.size() > MAX_HISTORY) {
    history_prune();
  }

  if (contents_history.size() < MAX_HISTORY) {
    contents_history.push_back(contents);
  } else {
    history_prune().push_back(contents);
  }
}

std::vector<vec_2d_ptr> &buf_history::history_prune(void) {
  if (!contents_history.empty()) {
    contents_history.erase(contents_history.begin());
  }
  return contents_history;
}
