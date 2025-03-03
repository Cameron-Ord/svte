#include "../inc/editor.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#define DEFAULT_SIZE 2

// Append the initial filename. If it is empty, it will just append as empty and
// the file names vector will be empty, so this will need to be handled later if
// no initial file name was provided
Buffers::Buffers(std::string wpath, std::string str_arg) : working_path(wpath) {
  append_buffer(str_arg);
  buf_alloc(str_arg);
}

int Buffers::buf_alloc(std::string file_name) {
  const int i = match_buffer(file_name);
  assert(i >= 0);

  std::cout << "Alloc buffer for: " << strjoin(working_path, file_name)
            << std::endl;

  buffers[i].buf = (char *)malloc(DEFAULT_SIZE);
  if (!buffers[i].buf) {
    std::cerr << "Failed to allocate buffer!" << std::endl;
    return 0;
  }
  buffers[i].buf[DEFAULT_SIZE - 1] = '\0';

  if (file_name.size() > 0) {
    FILE *f = fopen(strjoin(working_path, file_name).c_str(), "rb");
    if (f) {
      fseek(f, 0, SEEK_END);
      long fs = ftell(f);
      rewind(f);

      buffers[i].buf = (char *)realloc(buffers[i].buf, fs + 1);
      if (!buffers[i].buf) {
        std::cerr << "Failed to allocate buffer!" << std::endl;
        fclose(f);
        return 0;
      }

      const size_t read = fread(buffers[i].buf, 1, fs, f);
      if (read != fs) {
        std::cerr << "Reading from file failed!" << std::endl;
        fclose(f);
        return 0;
      }
      buffers[i].buf[read] = '\0';

      std::cout << "Read: " << read << " bytes" << std::endl;
      fclose(f);
    }
  }
  return 1;
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
// isn't empty. Can use the current buffer index to fetch the buffer and prompt
// to overwrite the fn if it's empty before calling this.
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
  return 1;
}
