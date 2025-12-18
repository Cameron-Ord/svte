#include "../buffer.hpp"
#include "../util.hpp"

bool buffer::yfits(int bufsize) const {
  return cursor_y < bufsize && cursor_y >= 0 && bufsize > 0;
}

bool buffer::xfits(int linesize) const {
  return cursor_x <= linesize && cursor_x >= 0;
}

bool buffer::fits(int val, int max) const {
  return val < max && val >= 0;
}


bool buffer::mv_left(unsigned int amount){
  int x = cursor_x;
  const int y = cursor_y;
  const int sbufsize = static_cast<int>(contents->size());
  if(yfits(sbufsize)){
    const std::vector<uint32_t> *line = &(*contents)[y];
    const int slinesize = static_cast<int>(line->size());
    if(xfits(slinesize)){
        for(unsigned int i = 0; i < amount && x > 0; i++){
          x--;
        }
        cursor_x = x;
        return true;
    }
  }
  return false;
}

bool buffer::mv_right(unsigned int amount){
  int x = cursor_x;
  const int y = cursor_y;
  const int sbufsize = static_cast<int>(contents->size());
  if(yfits(sbufsize)){
    const std::vector<uint32_t> *line = &(*contents)[y];
    const int slinesize = static_cast<int>(line->size());
    if(xfits(slinesize)){
        for(unsigned int i = 0; i < amount && x < slinesize; i++){
          x++;
        }
        cursor_x = x;
        return true;
    }
  }
  return false;
}

bool buffer::mv_up(unsigned int amount){
  int x = cursor_x, y = cursor_y;
  const int sbufsize = static_cast<int>(contents->size());
  if(yfits(sbufsize)){
    for(unsigned int i = 0; i < amount && y > 0; i++){
        y--;
    }
    const std::vector<uint32_t> *line = &(*contents)[y];
    const int slinesize = static_cast<int>(line->size());
    if(!xfits(slinesize)){
        x = slinesize;
    }
    cursor_x = x;
    cursor_y = y;
    return true;
  }
  return true;
}
bool buffer::mv_down(unsigned int amount){
  int x = cursor_x, y = cursor_y;
  const int sbufsize = static_cast<int>(contents->size());
  if(yfits(sbufsize)){
    for(unsigned int i = 0; i < amount && y < sbufsize; i++){
        y++;
    }
    const std::vector<uint32_t> *line = &(*contents)[y];
    const int slinesize = static_cast<int>(line->size());
    if(!xfits(slinesize)){
        x = slinesize;
    }
    cursor_y = y;
    cursor_x = x;
    return true;
  }
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
