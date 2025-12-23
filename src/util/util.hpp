
#pragma once

#include "alias.hpp"
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

#define UTF8_MAX_BYTES 4

struct utf_handler {
  static uint32_t decode_utf8(std::vector<char>);
  static char utf8_byte_count(char c);
  static std::vector<char> encode_utf8(uint32_t cp);
};

struct line_searcher {
  static void swap(uint32_t *a, uint32_t *b);
  static int partition(char_mat &arr, int low, int high);
  static void quicksort(char_mat &arr, int low, int high);
  static int search(char_mat &sorted, int left, int right, uint32_t key);
};

struct text_io {
  static char_mat_ptr read_text_file(const std::string &file_path, char_mat_ptr buf);
  static size_t write_text_file(const std::string &file_path, const char_mat_ptr buf);
};

struct logger {
  static bool log(std::string msg);
  static bool log_int(std::string msg, int64_t val);
  static bool log_str(std::string msg, std::string val);
};
