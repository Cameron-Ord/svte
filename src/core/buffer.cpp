#include "../buffer.hpp"
#include "../util.hpp"



buf_mutator::buf_mutator(void) : mode(INSERT_MODE), hist(buf_history()), curs(0, 0) {}

buffer::buffer(int set_id, std::string relative_path, vec_2d_ptr data)
    : id(set_id), filepath(relative_path), mutator(buf_mutator()), contents(data) {}


bool buffer::overwrite_contents(vec_2d_ptr cptr) {
  if(!cptr){
    return false;
  }

  if(cptr){
    contents = cptr;
  }
  return contents != cptr;
}
void buffer::save_buffer_file(void) { text_io::write_text_file(filepath, contents); }

bool buf_cursor::x_move_left(int amount, const_vec_2d_ptr contents){
  const int bufmax = contents->size();
  if(within_y(c.y, bufmax)){
    const int linemax = (*contents)[c.y].size();
    if(within_x(c.x - amount, linemax)){
      c.x -= amount;
      return true;
    } else {
      c.x = 0;
      return true;
    }
  }
  return false;
}

bool buf_cursor::y_move_up(int amount, const_vec_2d_ptr contents){
  const int bufmax = contents->size();
  if(within_y(c.y - amount, bufmax)){
    const int y = c.y - amount;
    const int linemax = (*contents)[y].size();
    if(!within_x(c.x, linemax)){
      c.x = linemax;
    }
    c.y = y;
    return true;
  } else {
    const int y = 0;
    const int linemax = (*contents)[y].size();
    if(!within_x(c.x, linemax)){
      c.x = linemax;
    }
    c.y = y;
    return true;
  }
  return false;
}

bool buf_cursor::y_move_down(int amount, const_vec_2d_ptr contents){
  const int bufmax = contents->size();
  if(within_y(c.y + amount, bufmax)){
    const int y = c.y + amount;
    const int linemax = (*contents)[y].size();
    if(!within_x(c.x, linemax)){
      c.x = linemax;
    }
    c.y = y;
    return true;
  } else {
    const int y = bufmax - 1;
    const int linemax = (*contents)[y].size();
    if(within_y(y, bufmax) && !within_x(c.x, linemax)){
      c.x = linemax;
    }
    c.y = y;
    return true;
  }
  return false;
}

bool buf_cursor::x_move_right(int amount, const_vec_2d_ptr contents){

  const int bufmax = contents->size();
  if(within_y(c.y, bufmax)){
    const_bufline_ptr line = &(*contents)[c.y];
    const int linemax = line->size();
    if(within_x(c.x + amount, linemax)){
      c.x += amount;
      return true;
    } else {
      c.x = linemax;
      return true;
    }
  }
  return false;
}

bool buf_cursor::within_x(int x, int max){
  return x >= 0 && x <= max;
}

bool buf_cursor::within_y(int y, int max){
  return y >= 0 && y < max;
}

vec_2d_ptr buf_mutator::create_newline(const_vec_2d_ptr contents){
  vec_2d_ptr overwrite = std::make_shared<vec_2d>(*contents);
  hist.history_push(contents);

  const coordinates& pos = curs.get_positions();
  const int bufmax = overwrite->size();

  if(curs.within_y(pos.y, bufmax)){
    overwrite->insert(overwrite->begin() + pos.y, bufline());
  }

  return overwrite;
}

vec_2d_ptr buf_mutator::char_insert(uint32_t character, const_vec_2d_ptr contents) {
  vec_2d_ptr overwrite = std::make_shared<vec_2d>(*contents);
  hist.history_push(contents);

  const coordinates& pos = curs.get_positions(); 
  const int bufmax = overwrite->size();
  
  if(curs.within_y(pos.y, bufmax)){
    bufline_ptr line = &(*overwrite)[pos.y];
    const int linemax = line->size();
    if(curs.within_x(pos.x, linemax)){
      line->insert(line->begin() + pos.x, character);
      curs.update_x(pos.x + 1);
    }
  }
  return overwrite;
}

vec_2d_ptr buf_mutator::char_replace(uint32_t character, const_vec_2d_ptr contents) {
  vec_2d_ptr overwrite = std::make_shared<vec_2d>(*contents);
  hist.history_push(contents);

  const coordinates& pos = curs.get_positions();
  const int bufmax = overwrite->size();

  if(curs.within_y(pos.y, bufmax)){
    bufline_ptr line = &(*overwrite)[pos.y];
    const int linemax = line->size();
    if(curs.within_x(pos.x, linemax)){
      *(line->begin() + pos.x) = character;
    }
  }

  return overwrite;
}

// not impl
vec_2d_ptr buf_mutator::char_remove(const_vec_2d_ptr contents) {
  vec_2d_ptr overwrite = std::make_shared<vec_2d>(*contents);
  hist.history_push(contents);
  return overwrite;
}

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
