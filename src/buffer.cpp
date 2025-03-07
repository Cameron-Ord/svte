#include "../inc/editor.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#define DEFAULT_SIZE 2

static File_Info fsize(FILE *fd) {
  if (!fd) {
    return (File_Info){0, NULL};
  }
  fseek(fd, 0, SEEK_END);
  return (File_Info){ftell(fd), fd};
}

static FILE *file_open(std::string path) { return fopen(path.c_str(), "rb"); }

// Append the initial filename. If it is empty, it will just append as empty and
// the file names vector will be empty, so this will need to be handled later if
// no initial file name was provided
Buffers::Buffers(std::string wpath, std::string str_arg) : working_path(wpath) {
  if (str_arg.size() <= 0) {
    str_arg = "new_file";
  }
  append_buffer(str_arg);
  const int i = match_buffer(str_arg);
  assert(i >= 0);
  buffers[i].buf = (char *)malloc(DEFAULT_SIZE);
  if (!buffers[i].buf) {
    std::cerr << "Failed to allocate buffer!" << std::endl;
  }
  buffers[i].buf[DEFAULT_SIZE - 1] = '\0';
  buf_realloc(i, fsize(file_open(strjoin(working_path, str_arg))));
}

size_t Buffers::buf_realloc(const size_t i, File_Info fi) {
  size_t read = 0;
  if (fi.f && fi.fs > 0) {
    rewind(fi.f);

    buffers[i].buf = (char *)realloc(buffers[i].buf, fi.fs + 1);
    if (!buffers[i].buf) {
      std::cerr << "Failed to allocate buffer!" << std::endl;
      fclose(fi.f);
      return 0;
    }

    read = fread(buffers[i].buf, 1, fi.fs, fi.f);
    if (read != fi.fs) {
      std::cerr << "Reading from file failed!" << std::endl;
      fclose(fi.f);
      return 0;
    }
    buffers[i].buf[read] = '\0';

    std::cout << "Read: " << read << " bytes" << std::endl;
    fclose(fi.f);
  }

  return read;
}

void Buffers::delete_buffer(std::string file_name) {
  const int i = match_buffer(file_name);
  assert(i >= 0);
  if (buffers[i].buf) {
    free(buffers[i].buf);
  }
  buffers.erase(buffers.begin() + i);
}

void Buffers::append_buffer(std::string file_name) {
  Buf b = {file_name, NULL, 0};
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
