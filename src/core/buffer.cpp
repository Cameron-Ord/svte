#include "buffer.hpp"
#include "../util/util.hpp"

buf_mutator::buf_mutator(void) : mode(INSERT_MODE), hist(buf_history()), curs(0, 0) {}

buffer::buffer(i32 set_id, std::string relative_path, char_mat_ptr data)
    : id(set_id), filepath(relative_path), mutator(buf_mutator()), contents(data) {}

bool buffer::overwrite_contents(char_mat_ptr cptr) {
  if (!cptr) {
    return false;
  }

  if (cptr) {
    contents = cptr;
  }
  return contents != cptr;
}

bool buf_cursor::within_bounds(i32 pos, i32 max) const { return pos >= 0 && pos < max; }

void buffer::save_buffer_file(void) { text_io::write_text_file(filepath, contents); }

i32 buf_cursor::line_x_offset(const_char_mat_ptr contents) {
  const i32 line_start = current_line_start(contents);
  return cursor - line_start;
}

i32 buf_cursor::current_line_end(const_char_mat_ptr contents) {
  i32 traverse = cursor;
  for (auto it = contents->begin() + cursor; it != contents->end(); it++) {
    if (*it == NEW_LINE) {
      // return the position before the new line
      return traverse - 1;
    } else if (it == std::prev(contents->end())) {
      return traverse;
    }
    traverse++;
  }
  return -1;
}

i32 buf_cursor::current_line_start(const_char_mat_ptr contents) {
  i32 traverse = cursor;
  for (auto it = contents->begin() + cursor; it != contents->end(); it--) {
    if (*it == NEW_LINE && traverse > 0) {
      // return the position after the new line
      return traverse + 1;
    } else if (it == contents->begin()) {
      // start of the buffer
      return traverse;
    }
    traverse--;
  }
  return -1;
}

i32 buf_cursor::rewind_by_new_line(i32 start_pos, const_char_mat_ptr contents) {
  i32 traverse = start_pos;
  for (auto it = contents->begin() + start_pos; it != contents->end(); it--) {
    if (*it == NEW_LINE) {
      // return the position after the new line
      return traverse + 1;
    }
    traverse--;
  }
  return -1;
}

i32 buf_cursor::advance_by_new_line(i32 start_pos, const_char_mat_ptr contents) {
  i32 traverse = start_pos;
  for (auto it = contents->begin() + start_pos; it != contents->end(); it++) {
    if (*it == NEW_LINE) {
      // return the position after the new line
      return traverse + 1;
    }
    traverse++;
  }
  return -1;
}

i32 buf_cursor::line_skip_backwards(i32 skip_count, const_char_mat_ptr contents) {
  i32 traversal_cursor = cursor;
  for (i32 i = 0; i < skip_count; i++) {
    const i32 tmp = rewind_by_new_line(traversal_cursor, contents);
    if (tmp >= 0) {
      traversal_cursor = tmp;
    }
  }
  return traversal_cursor;
}

i32 buf_cursor::line_skip_forwards(i32 skip_count, const_char_mat_ptr contents) {
  i32 traversal_cursor = cursor;
  for (i32 i = 0; i < skip_count; i++) {
    const i32 tmp = advance_by_new_line(traversal_cursor, contents);
    if (tmp >= 0) {
      traversal_cursor = tmp;
    }
  }
  return traversal_cursor;
}

bool buf_cursor::x_move_left(i32 amount, const_char_mat_ptr contents) {
  const i32 bufmax = static_cast<i32>(contents->size());
  if (within_bounds(cursor - 1, bufmax) || amount < 1) {
    cursor -= amount;
    return true;
  }
  return false;
}

bool buf_cursor::x_move_right(i32 amount, const_char_mat_ptr contents) {
  const i32 bufmax = static_cast<i32>(contents->size());
  if (within_bounds(cursor + amount, bufmax) || amount < 1) {
    cursor += amount;
    return true;
  }
  return false;
}

bool buf_cursor::y_move_up(i32 amount, const_char_mat_ptr contents) { return false; }

bool buf_cursor::y_move_down(i32 amount, const_char_mat_ptr contents) { return false; }

char_mat_ptr buf_mutator::create_newline(const_char_mat_ptr contents) {
  char_mat_ptr mutated = std::make_shared<char_mat>(*contents);
  return mutated;
}

char_mat_ptr buf_mutator::char_insert(u32 character, const_char_mat_ptr contents) {
  char_mat_ptr mutated = std::make_shared<char_mat>(*contents);
  return mutated;
}

char_mat_ptr buf_mutator::char_replace(u32 character, const_char_mat_ptr contents) {
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

void buf_history::history_push(const_char_mat_ptr contents) {
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
