#include "../buffer.hpp"
#include "../util.hpp"

static bool yfits(int y, size_t bufsize) {
  int sbufsize = static_cast<int>(bufsize);
  return y < sbufsize && y >= 0;
}

static bool xfits(int x, size_t linesize) {
  int slinesize = static_cast<int>(linesize);
  return x <= slinesize && x >= 0;
}

buf_mutator::buf_mutator(void) : mode(INSERT_MODE) {}

buffer::buffer(int set_id, std::string relative_path, vec_2d_ptr data)
    : id(set_id), filepath(relative_path), cursor_x(0), cursor_y(0),
      mutator(buf_mutator()), contents(data) {}

void buffer::save_buffer_file(void) {
  text_io::write_text_file(filepath, contents);
}

bool buffer::buf_insert(uint32_t character) {
  history.history_push(contents);
  contents = mutator.char_insert(cursor_x, cursor_y, character, contents);
  return contents != nullptr;
}

bool buffer::buf_replace(uint32_t character) {
  history.history_push(contents);
  contents = mutator.char_replace(cursor_x, cursor_y, character, contents);
  return contents != nullptr;
}

bool buffer::buf_delete(void) {
  history.history_push(contents);
  contents = mutator.char_delete(cursor_x, cursor_y, contents);
  return contents != nullptr;
}

bool buffer::buf_remove(void) {
  history.history_push(contents);
  contents = mutator.char_remove(cursor_x, cursor_y, contents);
  return contents != nullptr;
}

vec_2d_ptr buf_mutator::char_insert(int &x, int &y, uint32_t character,
                                    vec_2d_ptr contents) {
  vec_2d_ptr copy = std::make_shared<vec_2d>(*contents);
  if (yfits(y, copy->size())) {
    std::vector<uint32_t> *line = &(*copy)[y];
    if (xfits(x, line->size())) {
      line->insert(line->begin() + x, character);
      x++;
    }
  }
  return copy;
}

vec_2d_ptr buf_mutator::char_replace(int &x, int &y, uint32_t character,
                                     vec_2d_ptr contents) {
  vec_2d_ptr copy = std::make_shared<vec_2d>(*contents);
  if (yfits(y, copy->size())) {
    std::vector<uint32_t> *line = &(*copy)[y];
    if (xfits(x, line->size())) {
      line->erase(line->begin() + x);
      line->insert(line->begin() + x, character);
    }
  }
  return copy;
}

vec_2d_ptr buf_mutator::char_remove(int &x, int &y, vec_2d_ptr contents) {
  vec_2d_ptr copy = std::make_shared<vec_2d>(*contents);
  if (yfits(y, copy->size())) {
    std::vector<uint32_t> *line = &(*copy)[y];
    if (xfits(x - 1, line->size())) {
      line->erase(line->begin() + (x - 1));
      x--;
    }
  }
  return copy;
}

// not impl
vec_2d_ptr buf_mutator::char_delete(int &x, int &y, vec_2d_ptr contents) {
  vec_2d_ptr copy = std::make_shared<vec_2d>(*contents);
  return copy;
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
