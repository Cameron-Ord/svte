#include "../buffer.hpp"
#include "../util.hpp"

bool buffer::yfits(size_t bufsize) const {
  int sbufsize = static_cast<int>(bufsize);
  return cursor_y < sbufsize && cursor_y >= 0;
}

bool buffer::xfits(size_t linesize) const {
  int slinesize = static_cast<int>(linesize);
  return cursor_x <= slinesize && cursor_x >= 0;
}


bool buffer::yfits_arg(int y, size_t bufsize){
  int sbufsize = static_cast<int>(bufsize);
  return y < sbufsize && y >= 0;
}

bool buffer::xfits_arg(int x, size_t linesize){
  int slinesize = static_cast<int>(linesize);
  return x <= slinesize && x >= 0;
}

buf_mutator::buf_mutator(void) : mode(INSERT_MODE) {}

buffer::buffer(int set_id, std::string relative_path, vec_2d_ptr data)
    : id(set_id), filepath(relative_path), cursor_x(0), cursor_y(0), mutator(buf_mutator()), contents(data) {}

void buffer::save_buffer_file(void) { text_io::write_text_file(filepath, contents); }

const uint32_t *buffer::char_at_cursor(void) const {
  if(yfits(contents->size())){
    const std::vector<uint32_t> *line = &(*contents)[cursor_y];
    if(xfits(line->size())){
      return &(*line)[cursor_x];
    }
  }
  return nullptr;
}

bool buffer::buf_insert(uint32_t character) {
  if(yfits(contents->size())){
    const std::vector<uint32_t> *line = &(*contents)[cursor_y];
    if (xfits(line->size())) {
      vec_2d_ptr copy = std::make_shared<vec_2d>(*contents);
      history.history_push(contents);
      (*copy)[cursor_y] = mutator.char_insert(cursor_x, character, *line);
      contents = copy;
    }
  }
  return contents != nullptr;
}

bool buffer::buf_replace(uint32_t character) {
  if(yfits(contents->size())){
    const std::vector<uint32_t> *line = &(*contents)[cursor_y];
    if (xfits(line->size())) {
      vec_2d_ptr copy = std::make_shared<vec_2d>(*contents);
      history.history_push(contents);
      (*copy)[cursor_y] = mutator.char_replace(cursor_x, character, *line);
      contents = copy;
    }
  }
  return contents != nullptr;
}

bool buffer::buf_delete(void) {
  if(yfits(contents->size())){
    const std::vector<uint32_t> *line = &(*contents)[cursor_y];
    if (xfits(line->size())) {
      vec_2d_ptr copy = std::make_shared<vec_2d>(*contents);
      history.history_push(contents);
      (*copy)[cursor_y] = mutator.char_delete(cursor_x, *line);
      contents = copy;
    }
  }
  return contents != nullptr;
}

bool buffer::buf_remove(void) {
  if(yfits(contents->size())){
    const std::vector<uint32_t> *line = &(*contents)[cursor_y];
    if (xfits_arg(cursor_x - 1, line->size())) {
      vec_2d_ptr copy = std::make_shared<vec_2d>(*contents);
      history.history_push(contents);
      (*copy)[cursor_y] = mutator.char_remove(cursor_x, *line);
      contents = copy;
    }
  }
  return contents != nullptr;
}

std::vector<uint32_t> buf_mutator::char_insert(int &x, uint32_t character, std::vector<uint32_t> line) {
  line.insert(line.begin() + x, character);
  x++;
  return line;
}

std::vector<uint32_t> buf_mutator::char_replace(const int &x, uint32_t character, std::vector<uint32_t> line) {
  *(line.begin() + x) = character;
  return line;
}

std::vector<uint32_t> buf_mutator::char_remove(int &x, std::vector<uint32_t> line) {
 line.erase(line.begin() + (x - 1));
 x--;
 return line;
}

// not impl
std::vector<uint32_t> buf_mutator::char_delete(const int &x, std::vector<uint32_t> line) {
  line.erase(line.begin() + x);
  return line;
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
