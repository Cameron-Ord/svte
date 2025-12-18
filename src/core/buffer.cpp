#include "../buffer.hpp"
#include "../util.hpp"

bool buffer::yfits(size_t bufsize) const {
  int sbufsize = static_cast<int>(bufsize);
  return cursor_y < sbufsize && cursor_y >= 0 && sbufsize > 0;
}

bool buffer::xfits(size_t linesize) const {
  int slinesize = static_cast<int>(linesize);
  return cursor_x <= slinesize && cursor_x >= 0;
}

bool buffer::fits(int val, size_t max) const {
  int smax = static_cast<int>(max);
  return val < smax && val >= 0;
}


bool buffer::mv_left(unsigned int amount){
  unsigned int x = cursor_x, y = cursor_y;
  if(yfits(contents->size()) && xfits((*contents)[y].size())){
    for(unsigned int i = 0; i < amount && x >= 0; i++){
      x--;
    }
    cursor_x = x;
    return true;
  }
  return false;
}

bool buffer::mv_right(unsigned int amount){
  unsigned int x = cursor_y, y = cursor_y;
  if(yfits(contents->size()) && xfits((*contents)[y].size())){
    for(size_t i = 0; i < amount && x < (*contents)[y].size(); i++){
      x++;
    }
    cursor_x = x;
    return true;
  }
  return false;
}

bool buffer::mv_up(unsigned int amount){
  return true;
}
bool buffer::mv_down(unsigned int amount){
  return true;
}


buf_mutator::buf_mutator(void) : mode(INSERT_MODE) {}

buffer::buffer(int set_id, std::string relative_path, vec_2d_ptr data)
    : id(set_id), filepath(relative_path), cursor_x(0), cursor_y(0), mutator(buf_mutator()), contents(data) {}

void buffer::save_buffer_file(void) { text_io::write_text_file(filepath, contents); }

const uint32_t *buffer::char_at_cursor(void) const {
  if (yfits(contents->size())) {
    const std::vector<uint32_t> *line = &(*contents)[cursor_y];
    if (fits(cursor_x, line->size())) {
      return &(*line)[cursor_x];
    } else {
      return nullptr;
    }
  } else {
    return nullptr;
  }
}

bool buffer::buf_insert(uint32_t character) {
  if (yfits(contents->size())) {
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
  if (yfits(contents->size())) {
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
  if (yfits(contents->size())) {
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
  if (yfits(contents->size())) {
    const std::vector<uint32_t> *line = &(*contents)[cursor_y];
    if (fits(cursor_x - 1, line->size())) {
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
