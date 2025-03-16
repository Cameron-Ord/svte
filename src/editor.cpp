#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>

#include <iostream>

static int del_char_force_bounds(const size_t bs, const int pos) {
  if (pos >= (int)bs) {
    return 0;
  }

  if (pos < 0) {
    return 0;
  }

  return 1;
}

Editor::Editor(char *pathstr, char *arg_str) : bufs(Buffers(pathstr, arg_str)) {
  buf_i = 0;
}

void Editor::switch_buffer(const int direction) {
  const int buf_count = (int)bufs.buffer_count();

  switch (direction) {
  default:
    return;

  case NEXT_BUFFER: {
    if (buf_i + 1 < buf_count) {
      buf_i += 1;
    } else {
      buf_i = buf_count - 1;
    }
  } break;

  case PREV_BUFFER: {
    if (buf_i - 1 >= 0) {
      buf_i -= 1;
    } else {
      buf_i = 0;
    }
  } break;
  }
}

int Editor::buffer_del_char(void) {
  const Buf *b = bufs.get_buf(buf_i);
  if (b->size < 1) {
    return 0;
  }

  if (bufs.buf_bounds(buf_i)) {
    if (!del_char_force_bounds(b->size, b->pos)) {
      bufs.buf_pos_bw(buf_i);
    }

    bufs.shift_buffer(DEL, buf_i);
    bufs.buf_realloc(buf_i, b->size - 1);
  }

  return 1;
}

int Editor::buffer_rm_char(void) {
  const Buf *b = bufs.get_buf(buf_i);
  if (!(b->pos >= 0) || b->size < 1)
    return 0;

  if (bufs.buf_bounds(buf_i)) {
    bufs.shift_buffer(RMV, buf_i);
    bufs.buf_pos_bw(buf_i);
    bufs.buf_realloc(buf_i, b->size - 1);
  }

  return 1;
}

int Editor::buffer_insert_char(const char c) {
  const Buf *b = bufs.get_buf(buf_i);
  if (bufs.buf_bounds(buf_i)) {
    bufs.buf_realloc(buf_i, b->size + 1);
    bufs.shift_buffer(INS, buf_i);
    bufs.buf_insert(buf_i, &c);
  }
  return 1;
}

void Editor::buffer_mv_op(const int operation) {
  bufs.buf_mv_pos(buf_i, operation);
}
