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

bool buf_cursor::within_bounds(int pos, int max){
    return pos >= 0 && pos < max;
}

void buffer::save_buffer_file(void) { text_io::write_text_file(filepath, contents); }


int buf_cursor::line_x_offset(const_char_mat_ptr contents){
    const int line_start = current_line_start(contents);
    return cursor - line_start;
}


int buf_cursor::current_line_end(const_char_mat_ptr contents){
    int traverse = cursor;
    for(auto it = contents->begin() + cursor; it != contents->end(); it++){
        if(*it == NEW_LINE){
            //return the position before the new line
            return traverse - 1;
        } else if (it == std::prev(contents->end())){
            return traverse;
        }
        traverse++;
    }
    return -1;
}

int buf_cursor::current_line_start(const_char_mat_ptr contents){
    int traverse = cursor;
    for(auto it = contents->begin() + cursor; it != contents->end(); it--){
        if(*it == NEW_LINE && traverse > 0){
            //return the position after the new line
            return traverse + 1;
        } else if(it == contents->begin()){
            //start of the buffer
            return traverse;
        }
        traverse--;
    }
    return -1;
}

int buf_cursor::rewind_by_new_line(int start_pos, const_char_mat_ptr contents){
    int traverse = start_pos;
    for(auto it = contents->begin() + start_pos; it != contents->end(); it--){
        if(*it == NEW_LINE){
            //return the position after the new line
            return traverse + 1;
        }
        traverse--;
    }
    return -1;
}

int buf_cursor::advance_by_new_line(int start_pos, const_char_mat_ptr contents){
    int traverse = start_pos;
    for(auto it = contents->begin() + start_pos; it != contents->end(); it++){
        if(*it == NEW_LINE){
            //return the position after the new line
            return traverse + 1;
        }
        traverse++;
    }
    return -1;
}

int buf_cursor::line_skip_backwards(int skip_count, const_char_mat_ptr contents) {
    int traversal_cursor = cursor;
    for(int i = 0; i < skip_count; i++){
        const int tmp = rewind_by_new_line(traversal_cursor, contents);
        if(tmp >= 0){
            traversal_cursor = tmp;
        }
    }
    return traversal_cursor;
}

int buf_cursor::line_skip_forwards(int skip_count, const_char_mat_ptr contents) {
    int traversal_cursor = cursor;
    for(int i = 0; i < skip_count; i++){
        const int tmp = advance_by_new_line(traversal_cursor, contents);
        if(tmp >= 0){
            traversal_cursor = tmp;
        }
    }
    return traversal_cursor;
}

bool buf_cursor::x_move_left(int amount, const_char_mat_ptr contents){
    const int bufmax = contents->size();
    if(within_bounds(cursor - 1, bufmax) || amount < 1){
        cursor -= amount;
        return true;
    }
    return false;
}

bool buf_cursor::x_move_right(int amount, const_char_mat_ptr contents){
    const int bufmax = contents->size();
    if(within_bounds(cursor + amount, bufmax) || amount < 1){
        cursor += amount;
        return true;
    }
    return false;
}

bool buf_cursor::y_move_up(int amount, const_char_mat_ptr contents){
    return false;
}

bool buf_cursor::y_move_down(int amount, const_char_mat_ptr contents){
    return false;
}

char_mat_ptr buf_mutator::create_newline(const_char_mat_ptr contents){
    char_mat_ptr mutated = std::make_shared<char_mat>(*contents);
    return mutated;
}

char_mat_ptr buf_mutator::char_insert(uint32_t character, const_char_mat_ptr contents) {
    char_mat_ptr mutated = std::make_shared<char_mat>(*contents);
    return mutated;
}

char_mat_ptr buf_mutator::char_replace(uint32_t character, const_char_mat_ptr contents) {
    char_mat_ptr mutated = std::make_shared<char_mat>(*contents);
    return mutated;
}

// not impl
char_mat_ptr buf_mutator::char_remove(const_char_mat_ptr contents) {
    char_mat_ptr mutated = std::make_shared<char_mat>(*contents);
    return mutated;
}

char_mat_ptr buf_mutator::char_delete(const_char_mat_ptr contents) {
    char_mat_ptr mutated = std::make_shared<char_mat>(*contents);
    return mutated;
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
