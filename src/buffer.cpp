#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>
#define PER_FILE_LIMIT (250 * 1024 * 1024) // 250 MiB

#ifndef NULLCHAR
#define NULLCHAR '\0'
#endif

#ifndef SPACECHAR
#define SPACECHAR ' '
#endif

#ifndef NEWLINE
#define NEWLINE '\n'
#endif

#define DEFAULT_SIZE 1

static size_t len_add(const size_t *lengths, const size_t size) {
  size_t accumulator = 0;
  for (size_t i = 0; i < size; i++) {
    accumulator += lengths[i];
  }
  return accumulator;
}

static File_Info fsize(FILE *fd) {
  if (!fd) {
    File_Info f = {0, NULL};
    return f;
  }
  fseek(fd, 0, SEEK_END);
  File_Info f = {ftell(fd), fd};
  return f;
}

static FILE *file_open(const char *path) {
  FILE *f = NULL;
  if (!(f = fopen(path, "rb"))) {
    std::cerr << "Failed to open path: " << path << std::endl;
    return NULL;
  }
  return f;
}

// needs some checks
void Buffers::buf_replace_at(const int i, const unsigned char c) {
  if (buf_bounds(i) && buffers[i].pos < (int)buffers[i].size)
    buffers[i].buf[buffers[i].pos] = c;
}

int Buffers::pos_bounds(const int i, const int pos) {
  const size_t size = buffers[i].size;
  if (pos > (int)size) {
    return 0;
  }

  if (pos < 0) {
    return 0;
  }

  return 1;
}

int Buffers::buf_bounds(const int i) {
  if (i < 0) {
    return 0;
  }

  if (i >= (int)buffers.size()) {
    return 0;
  }

  return 1;
}

int Buffers::find_word(const int i, const int direction) {
  if (buf_bounds(i) && pos_bounds(i, buffers[i].pos)) {
    Buf *b = &buffers[i];
    int pos = b->pos;

    switch (direction) {
    default:
      return pos;
    case 1: {
      unsigned char lastchar = 0;
      for (int j = pos; j <= (int)b->size; j++) {
        const unsigned char currchar = b->buf[j];
        const int cond = lastchar == SPACECHAR || lastchar == NEWLINE;

        if (cond && currchar != lastchar) {
          return j;
        }

        lastchar = currchar;
      }
    } break;
    case -1: {
      unsigned char lastchar = 0;
      for (int j = pos; j >= 0; j--) {
        const unsigned char currchar = b->buf[j];
        const int cond = lastchar == SPACECHAR || lastchar == NEWLINE;

        if (cond && currchar != lastchar) {
          return j;
        }
        lastchar = currchar;
      }
    } break;
    }
    return pos;
  }

  return 0;
}

int Buffers::find_line(const int i, const int direction) {
  if (buf_bounds(i) && pos_bounds(i, buffers[i].pos)) {
    Buf *b = &buffers[i];
    int pos = b->pos;

    switch (direction) {
    default:
      return 0;
    case 1: {
      const int cond = b->buf[pos] == NEWLINE; //|| b->buf[pos] == NULLCHAR;
      if (cond && pos_bounds(i, pos + 1)) {
        pos++;
      }

      for (int j = pos; j <= (int)b->size; j++) {
        const char c = b->buf[j];
        if (c == NEWLINE || c == NULLCHAR) {
          return j;
        }
      }
      return (int)b->size;
    } break;

    case -1: {
      const int cond = b->buf[pos] == NEWLINE || b->buf[pos] == NULLCHAR;
      if (cond && pos_bounds(i, pos - 1)) {
        pos--;
      }

      for (int j = pos; j >= 0; j--) {
        const char c = b->buf[j];
        if (c == NEWLINE || c == NULLCHAR) {
          return j;
        }
      }
      return 0;
    } break;
    }
  }
  return 0;
}

void Buffers::buf_mv_pos(const int i, const int operation) {
  switch (operation) {
  default:
    return;

  case MV_RIGHT: {
    if (buf_bounds(i) && pos_bounds(i, buffers[i].pos + 1)) {
      buffers[i].pos += 1;
    } else if (buf_bounds(i) && !pos_bounds(i, buffers[i].pos + 1)) {
      buffers[i].pos = buffers[i].size;
    }
  } break;

  case MV_LEFT: {
    if (buf_bounds(i) && pos_bounds(i, buffers[i].pos - 1)) {
      buffers[i].pos -= 1;
    } else if (buf_bounds(i) && !pos_bounds(i, buffers[i].pos - 1)) {
      buffers[i].pos = 0;
    }
  } break;

  // Words are seperated by spaces, so just find the first char after a space,
  // can add skip to last position later
  case NEXT_WORD: {
    if (buf_bounds(i)) {
      buffers[i].pos = find_word(i, 1);
    }
  } break;
  case PREV_WORD: {
    if (buf_bounds(i)) {
      buffers[i].pos = find_word(i, -1);
    }
  } break;
  case PREV_LINE: {
    if (buf_bounds(i)) {
      buffers[i].pos = find_line(i, -1);
    }
  } break;
  case NEXT_LINE: {
    if (buf_bounds(i)) {
      buffers[i].pos = find_line(i, 1);
    }
  } break;
  }
}

void Buffers::buf_pos_bw(const int i) {
  if (buf_bounds(i) && pos_bounds(i, buffers[i].pos - 1)) {
    buffers[i].pos--;
  } else if (buf_bounds(i) && !pos_bounds(i, buffers[i].pos - 1)) {
    buffers[i].pos = 0;
  }
}

void Buffers::buf_insert(const int i, const char *c) {
  if (buf_bounds(i) && (size_t)buffers[i].pos < buffers[i].size) {
    Buf *buf = &buffers[i];
    memcpy(&buf->buf[buf->pos], c, 1);
    buf->pos++;
  }
}

void Buffers::shift_buffer(const int direction, const int i) {
  if (!buf_bounds(i)) {
    return;
  }
  Buf *buf = &buffers[i];

  switch (direction) {
  case INS: { // insert
    memmove(&buf->buf[buf->pos + 1], &buf->buf[buf->pos],
            buf->size - buf->pos - 1);
  } break;

  case DEL: { // delete
    memmove(&buf->buf[buf->pos], &buf->buf[buf->pos + 1],
            buf->size - buf->pos - 1);
  } break;

  case RMV: { // backspace/remove
    memmove(&buf->buf[buf->pos - 1], &buf->buf[buf->pos], buf->size - buf->pos);
  } break;
  }
}

// Get buffer by idx
const Buf *Buffers::get_buf(const int i) {
  if (!buf_bounds(i)) {
    return NULL;
  }

  return &buffers[i];
}

void Buffers::print_file(const int i) {
  if (!buffers[i].buf) {
    return;
  }
  printf("%s\n", buffers[i].buf);
}

// Append the initial filename. If it is empty, it will just append as empty
// and the file names vector will be empty, so this will need to be handled
// later if no initial file name was provided
Buffers::Buffers(char *pathstr, char *str_arg) : working_path(pathstr) {
  const int gate = (str_arg && strlen(str_arg) > 0) ? 1 : 0;

  switch (gate) {
  case 0: {
    append_buffer(random_fn(), 1);
    buf_malloc(0, 0);
  } break;

  case 1: {
    append_buffer(str_arg, 0);
    read_file(str_arg);
  } break;
  }
}

char *Buffers::random_fn(void) {
  srand(time(NULL));

  const size_t str_length = 24;
  const size_t ASCII_RAND_MAX = 128 - 32;

  char *str = (char *)malloc(str_length + 1);
  for (size_t i = 0; i < str_length; i++) {
    const unsigned char r = rand() % ASCII_RAND_MAX;
    str[i] = r;
  }
  str[str_length] = '\0';
  return str;
}

size_t Buffers::read_file(const char *fn) {
  std::cout << fn << std::endl;
  std::cout << working_path << std::endl;

  const char *s = "/";
  const size_t lengths[] = {strlen(fn), strlen(working_path), strlen(s)};
  char *path = (char *)malloc(len_add(lengths, 3) + 1);

  strncpy(path, working_path, lengths[1] + 1);
  strncat(path, s, lengths[2] + 1);
  strncat(path, fn, lengths[0] + 1);

  File_Info fi = fsize(file_open(path));
  const int i = match_buffer(fn);
  size_t read = 0;

  if (fi.f && fi.fs > 0) {
    rewind(fi.f);
    const int cond = fi.fs > PER_FILE_LIMIT;

    switch (cond) {
    case 0: {
      if (buf_malloc(i, (size_t)fi.fs)) {
        read = fread(buffers[i].buf, 1, fi.fs, fi.f);
      }
    } break;
    case 1: {
      std::cerr << "File too large!" << std::endl;
    } break;
    }

  } else if (!fi.f) {
    buf_malloc(i, 0);
  }

  free(path);
  return read;
}

size_t Buffers::buf_malloc(const int i, const size_t size) {
  assert(buf_bounds(i) && size + 1 >= DEFAULT_SIZE);
  buffers[i].buf = (char *)malloc(size + 1);
  if (!buffers[i].buf) {
    std::cerr << "Failed to allocate buffer!" << std::endl;
    return 0;
  }

  for (size_t j = 0; j < size + 1; j++)
    buffers[i].buf[j] = NULLCHAR;

  buffers[i].size = size;
  return size;
}

size_t Buffers::buf_realloc(const int i, const size_t new_size) {
  assert(buf_bounds(i) && new_size + 1 >= DEFAULT_SIZE);
  char *tmp = (char *)realloc(buffers[i].buf, new_size + 1);
  if (!tmp) {
    std::cerr << "Failed to reallocate buffer!" << std::endl;
    return 0;
  }
  buffers[i].buf = tmp;

  if (new_size == 0) {
    for (size_t j = 0; j < new_size + 1; j++) {
      buffers[i].buf[j] = NULLCHAR;
    }

    buffers[i].size = new_size;
    return new_size;
  }

  buffers[i].buf[new_size] = NULLCHAR;
  buffers[i].size = new_size;
  return new_size;
}

void Buffers::delete_buffer(const char *file_name) {
  const int i = match_buffer(file_name);
  assert(buf_bounds(i));
  if (buffers[i].buf) {
    free(buffers[i].buf);
  }
  buffers.erase(buffers.begin() + i);
}

void Buffers::append_buffer(char *file_name, const int fn_needs_change) {
  Buf b = {file_name, NULL, fn_needs_change, 0, 0};
  buffers.push_back(b);
}

int Buffers::match_buffer(const char *key) {
  for (size_t i = 0; i < buffers.size(); i++) {
    if (strcmp(key, buffers[i].fn) == 0) {
      return i;
    }
  }
  return -1;
}

// This will need to be gated before it's called to make sure the file name
// isn't empty. Can use the current buffer index to fetch the buffer and
// prompt to overwrite the fn if it's empty before calling this.
int Buffers::write_buffer(const char *file_name) {
  const int i = match_buffer(file_name);
  assert(buf_bounds(i));

  const char *s = "/";
  const size_t lengths[] = {strlen(file_name), strlen(working_path), strlen(s)};
  char *path = (char *)malloc(len_add(lengths, 3) + 1);

  strncpy(path, working_path, lengths[1] + 1);
  strncat(path, s, lengths[2] + 1);
  strncat(path, file_name, lengths[0] + 1);

  FILE *f = fopen(path, "wa");
  if (!f) {
    std::cerr << "Failed to open file!" << std::endl;
    return 0;
  }

  const size_t written = fwrite(buffers[i].buf, 1, buffers[i].size, f);
  if (written != buffers[i].size) {
    std::cerr << "Failed to write to file!" << std::endl;
    return 0;
  }

  fclose(f);
  free(path);
  return written;
}
