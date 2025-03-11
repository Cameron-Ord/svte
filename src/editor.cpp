#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>

Editor::Editor(char *pathstr, char *arg_str) : bufs(Buffers(pathstr, arg_str)) {
  buf_i = 0;
}

void Editor::switch_buffer(const int direction) {
  const int buf_count = (int)bufs.buffer_count();
  int signed_i = (int)buf_i;

  switch (direction) {
  default:
    return;

  case NEXT_BUFFER: {
    if (signed_i + 1 < buf_count) {
      buf_i += 1;
    } else {
      buf_i = buf_count - 1;
    }
  } break;

  case PREV_BUFFER: {
    if (signed_i - 1 >= 0) {
      buf_i -= 1;
    } else {
      buf_i = 0;
    }
  } break;
  }
}

int Editor::buffer_del_char(void) {
  const Buf *b = bufs.get_buf(buf_i);
  if (b->size < 1)
    return 0;

  if (b->size == 1 && b->pos == 0) {
    bufs.buf_replace_at(buf_i, ' ');
    return 1;
  }

  if (b->pos < b->size) {
    // Shift it over before deleting so the placeholder remains
    if (b->pos == b->size - 1) {
      bufs.buf_rm(buf_i);
    }

    bufs.shift_buffer(DEL, buf_i);
    bufs.buf_realloc(buf_i, b->size - 1);
    if (b->pos == b->size) {
      bufs.buf_rm(buf_i);
    }
  }

  return 1;
}

int Editor::buffer_rm_char(void) {
  const Buf *b = bufs.get_buf(buf_i);
  if (b->size < 1)
    return 0;

  if (b->pos > 0 && b->pos < b->size) {
    bufs.shift_buffer(RMV, buf_i);
    bufs.buf_rm(buf_i);
    bufs.buf_realloc(buf_i, b->size - 1);
  }

  return 1;
}

int Editor::buffer_insert_char(const unsigned char c) {
  const Buf *b = bufs.get_buf(buf_i);
  if (b->pos < b->size) {
    bufs.buf_realloc(buf_i, b->size + 1);
    bufs.shift_buffer(INS, buf_i);
    bufs.buf_insert(buf_i, c);
  }
  return 1;
}

void Editor::buffer_mv_op(const int operation) {
  bufs.buf_mv_pos(buf_i, operation);
}
