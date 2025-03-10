#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>

#define DEFAULT_SIZE 2

static size_t len_add(const size_t *lengths, const size_t size) {
  size_t accumulator = 0;
  for (size_t i = 0; i < size; i++) {
    accumulator += lengths[i];
  }
  return accumulator;
}

int Buffers::buf_split_by_nl(const size_t i) {
  if (i >= buffers.size()) {
    return 0;
  }
  return 1;
}

// Get buffer by idx
const Buf *Buffers::get_buf(const size_t i) {
  if (i >= buffers.size()) {
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

// Append the initial filename. If it is empty, it will just append as empty
// and the file names vector will be empty, so this will need to be handled
// later if no initial file name was provided
Buffers::Buffers(char *pathstr, char *str_arg) : working_path(pathstr) {
  const int gate = (str_arg && strlen(str_arg) > 0) ? 1 : 0;

  switch (gate) {
  case 0: {
    std::cout << "New file" << std::endl;
    append_buffer(random_fn(), 1);
    buf_malloc(0, DEFAULT_SIZE);
  } break;

  case 1: {
    std::cout << "Read file" << std::endl;
    append_buffer(str_arg, 0);
    read_file(str_arg);
    print_file(match_buffer(str_arg));
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
  size_t read = 0;

  if (fi.f && fi.fs > 0) {
    const int i = match_buffer(fn);

    rewind(fi.f);
    const size_t size = (size_t)fi.fs;

    if (buf_malloc(i, size + 1)) {
      read = fread(buffers[i].buf, 1, size, fi.f);
    }
  }
  free(path);

  return read;
}

size_t Buffers::buf_malloc(const size_t i, const size_t size) {
  assert(i >= 0 && size >= DEFAULT_SIZE);
  buffers[i].buf = (char *)malloc(size);
  if (!buffers[i].buf) {
    std::cerr << "Failed to allocate buffer!" << std::endl;
    return 0;
  }
  buffers[i].buf[size - 1] = '\0';
  buffers[i].size = size;
  return size;
}

size_t Buffers::buf_realloc(const size_t i, const size_t new_size) {
  assert(i >= 0 && new_size >= DEFAULT_SIZE);
  char *tmp = (char *)realloc(buffers[i].buf, new_size + 1);
  if (!tmp) {
    std::cerr << "Failed to reallocate buffer!" << std::endl;
    return 0;
  }

  buffers[i].buf = tmp;
  buffers[i].buf[new_size] = '\0';
  buffers[i].size = new_size;

  return new_size;
}

void Buffers::delete_buffer(const char *file_name) {
  const int i = match_buffer(file_name);
  assert(i >= 0);
  if (buffers[i].buf) {
    free(buffers[i].buf);
  }
  buffers.erase(buffers.begin() + i);
}

void Buffers::append_buffer(char *file_name, const int fn_needs_change) {
  Buf b = {file_name, fn_needs_change, NULL, NULL, 0};
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
  assert(i >= 0);

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
