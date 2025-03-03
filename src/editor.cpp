#include "../inc/editor.hpp"
#include <cstdlib>
#include <iostream>

#define BASE_BUFFER_SIZE 2

static FILE *file_open(std::string str) {
  std::cout << "Opening: " << str.c_str() << std::endl;
  FILE *f = fopen(str.c_str(), "rb");
  if (f) {
    return f;
  }

  std::cout << "File not read or doesn't exist" << std::endl;
  return NULL;
}

Editor::Editor(std::string path, std::string file)
    : f(NULL), buffer(NULL), current_path(path), file_str(file),
      abs_path(path + "/" + file) {
  std::cout << "Working path: " << abs_path << std::endl;

  buffer = (char *)malloc(BASE_BUFFER_SIZE);
  if (!buffer) {
    std::cerr << "Failed to allocate file buffer!" << std::endl;
  }

  if (buffer) {
    buffer[BASE_BUFFER_SIZE - 1] = '\0';
  }

  if (file.size() != 0) {
    f = file_open(abs_path);

    if (f && buffer) {
      fseek(f, 0, SEEK_END);
      long fsize = ftell(f);
      rewind(f);

      std::cout << "Allocating buffer for size: " << fsize << std::endl;
      buffer = (char *)realloc(buffer, fsize + 1);
      if (!buffer) {
        std::cerr << "Failed to allocate file buffer!" << std::endl;
      }

      if (buffer) {
        const size_t read = fread(buffer, 1, fsize, f);
        buffer[read] = '\0';
      }

      fclose(f);
      printf("%s\n", buffer);
    }
  }

  grid.row.resize(1);
  grid.col.resize(1);
  pos.x = 0, pos.y = 0;
}
