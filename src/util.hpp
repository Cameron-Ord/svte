#pragma once

#include "alias.hpp"
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

#define UTF8_MAX_BYTES 4

struct utf_handler {
  static uint32_t decode_utf8(std::vector<unsigned char>);
  static char utf8_byte_count(char c);
  static std::vector<unsigned char> encode_utf8(uint32_t cp);
};

struct line_searcher {
  template <typename T>
  std::enable_if_t<std::is_arithmetic_v<T>, void> static swap(T *a, T *b) {
    T *temp = a;
    a = b;
    b = temp;
  }

  template <typename T>
  std::enable_if_t<std::is_arithmetic_v<T>, int> static partition(
      std::vector<T> &arr, int low, int high) {
    const T p = arr[low];
    int i = low;
    int j = high;

    while (i < j) {
      while (arr[i] <= p && i <= high - 1) {
        i++;
      }

      while (arr[j] > p && j >= low + 1) {
        j--;
      }

      if (i < j) {
        swap(&arr[i], &arr[j]);
      }
    }
    swap(&arr[low], &arr[j]);
    return j;
  }

  template <typename T>
  std::enable_if_t<std::is_arithmetic_v<T>, void> static quicksort(
      std::vector<T> &arr, int low, int high) {
    if (low < high) {
      const int pi = partition(arr, low, high);
      quicksort(arr, low, pi - 1);
      quicksort(arr, pi + 1, high);
    }
  }

  template <typename T>
  std::enable_if_t<std::is_arithmetic_v<T>, int> static binary_search(
      std::vector<T> &sorted, int left, int right, T key) {
    while (left <= right) {
      const int mid = left + (right - left) / 2;
      if (sorted[mid] == key) {
        return mid;
      }

      if (sorted[mid] < key) {
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }

    return -1;
  }
};

struct text_io {
  static vec_2d_ptr read_text_file(const std::string &file_path);
  static size_t write_text_file(const std::string &file_path,
                                const vec_2d_ptr buf);
};

struct logger {
  static bool log(std::string msg);

  template <typename T> static bool log_var(std::string msg, T var) {
    std::ofstream file("svte_log.txt", std::ios::app);
    if (!file.is_open()) {
      return false;
    }
    file << msg << var << std::endl;
    return true;
  }
};
