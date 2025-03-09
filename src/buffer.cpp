#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#define DEFAULT_SIZE 2

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

static FILE *file_open(std::string path) { return fopen(path.c_str(), "rb"); }

// Append the initial filename. If it is empty, it will just append as empty and
// the file names vector will be empty, so this will need to be handled later if
// no initial file name was provided
Buffers::Buffers(std::string wpath, std::string str_arg) : working_path(wpath) {
  const int gate = (str_arg.empty()) ? 1 : 0;

  switch (gate) {
  case 1: {
    str_arg = random_fn();
    append_buffer(str_arg, 1);
    buf_malloc(match_buffer(str_arg), DEFAULT_SIZE);
  } break;

  case 0: {
    append_buffer(str_arg, 0);
    read_file(str_arg);
    print_file(match_buffer(str_arg));
  } break;
  }
}

std::string Buffers::random_fn(void) {
  srand(time(NULL));

  const size_t str_length = 24;
  const size_t ASCII_RAND_MAX = 128 - 32;

  std::string str = "";
  for (size_t i = 0; i < str_length; i++) {
    const unsigned char r = rand() % ASCII_RAND_MAX;
    str += (r + 32);
  }
  return str;
}

size_t Buffers::read_file(std::string fn) {
  const std::string file_path = strjoin(working_path, fn);
  File_Info fi = fsize(file_open(file_path));
  size_t read = 0;

  if (fi.f && fi.fs > 0) {
    const int i = match_buffer(fn);

    rewind(fi.f);
    const size_t size = (size_t)fi.fs;

    if (buf_malloc(i, size + 1)) {
      read = fread(buffers[i].buf, 1, size, fi.f);
    }
  }

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

void Buffers::delete_buffer(std::string file_name) {
  const int i = match_buffer(file_name);
  assert(i >= 0);
  if (buffers[i].buf) {
    free(buffers[i].buf);
  }
  buffers.erase(buffers.begin() + i);
}

void Buffers::append_buffer(std::string file_name, const int fn_needs_change) {
  Buf b = {file_name, fn_needs_change, NULL, 0};
  buffers.push_back(b);
}

int Buffers::match_buffer(std::string key) {
  for (size_t i = 0; i < buffers.size(); i++) {
    if (key == buffers[i].fn) {
      return i;
    }
  }
  return -1;
}

// This will need to be gated before it's called to make sure the file name
// isn't empty. Can use the current buffer index to fetch the buffer and
// prompt to overwrite the fn if it's empty before calling this.
int Buffers::write_buffer(std::string file_name) {
  const int i = match_buffer(file_name);
  assert(i >= 0);

  FILE *f = fopen(strjoin(working_path, file_name).c_str(), "wa");
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
  return written;
}
