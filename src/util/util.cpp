
#include "util.hpp"

enum SHIFTS {
  S18 = 18,
  S12 = 12,
  S6 = 6,
};

enum LIMITS {
  UTF8_1B_MAX = 0x7F,
  UTF8_2B_MAX = 0x7FF,
  UTF8_3B_MAX = 0xFFFF,
  UTF8_4B_MAX = 0x10FFFF,
};

enum SIGNATURES {
  UTF8_1B_SIG = 0x00,
  UTF8_2B_SIG = 0xC0,
  UTF8_3B_SIG = 0xE0,
  UTF8_4B_SIG = 0xF0,
  CONT_SIG = 0x80,
};

enum MASKS {
  UTF8_1B_MASK = 0x80,
  UTF8_2B_MASK = 0xE0,
  UTF8_3B_MASK = 0xF0,
  UTF8_4B_MASK = 0xF8,

  CONT_MASK = 0x3F,
  UTF8_2B_VMASK = 0x1F,
  UTF8_3B_VMASK = 0x0F,
  UTF8_4B_VMASK = 0x07,
};

const char NULLCHAR = '\0';
static const u8 MASKS[] = {UTF8_1B_MASK, UTF8_2B_MASK, UTF8_3B_MASK, UTF8_4B_MASK};

static const u8 SIGS[] = {UTF8_1B_SIG, UTF8_2B_SIG, UTF8_3B_SIG, UTF8_4B_SIG};

size_t ucast(i32 sint) { return static_cast<size_t>(sint); }

i32 line_searcher::search(char_mat &sorted, i32 left, i32 right, u32 key) {
  while (left <= right) {
    const i32 mid = left + (right - left) / 2;
    if (sorted[ucast(mid)] == key) {
      return mid;
    }

    if (sorted[ucast(mid)] < key) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return -1;
}

void line_searcher::quicksort(char_mat &arr, i32 low, i32 high) {
  if (low < high) {
    const i32 pi = partition(arr, low, high);
    quicksort(arr, low, pi - 1);
    quicksort(arr, pi + 1, high);
  }
}

i32 line_searcher::partition(char_mat &arr, i32 low, i32 high) {
  const u32 p = arr[static_cast<size_t>(low)];
  i32 i = low;
  i32 j = high;

  while (i < j) {
    while (arr[ucast(i)] <= p && i <= high - 1) {
      i++;
    }

    while (arr[ucast(j)] > p && j >= low + 1) {
      j--;
    }

    if (i < j) {
      swap(&arr[ucast(i)], &arr[ucast(j)]);
    }
  }
  swap(&arr[ucast(low)], &arr[ucast(j)]);
  return j;
}

void line_searcher::swap(u32 *a, u32 *b) {
  u32 *tmp = a;
  a = b;
  b = tmp;
}

u8 utf_handler::utf8_byte_count(const char ch) {
  for (u8 i = 0; i < UTF8_MAX_BYTES; i++) {
    if ((ch & MASKS[i]) == SIGS[i]) {
      return i + 1;
    }
  }
  return 0;
}

std::vector<char> utf_handler::encode_utf8(u32 cp) {
  auto cont_append = [](u32 val, i8 bytecount) {
    std::vector<char> bytes(ucast(bytecount));
    for (i8 i = bytecount - 1; i >= 0; i--) {
      bytes[ucast(i)] = static_cast<char>(CONT_SIG | (val & CONT_MASK));
      val >>= 6;
    }
    bytes.push_back(NULLCHAR);
    return bytes;
  };

  if (cp <= UTF8_1B_MAX) {
    return std::vector<char>{static_cast<char>(cp), NULLCHAR};
  } else if (cp <= UTF8_2B_MAX) {
    char front = static_cast<char>(UTF8_2B_SIG | ((cp >> S6) & UTF8_2B_VMASK));
    std::vector<char> b = cont_append(cp, 1);
    b.insert(b.begin(), front);
    return b;
  } else if (cp <= UTF8_3B_MAX) {
    char front = static_cast<char>(UTF8_3B_SIG | ((cp >> S12) & UTF8_3B_VMASK));
    std::vector<char> b = cont_append(cp, 2);
    b.insert(b.begin(), front);
    return b;
  } else if (cp <= UTF8_4B_MAX) {
    char front = static_cast<char>(UTF8_4B_SIG | ((cp >> S18) & UTF8_4B_VMASK));
    std::vector<char> b = cont_append(cp, 3);
    b.insert(b.begin(), front);
    return b;
  } else {
    return std::vector<char>{NULLCHAR};
  }
}

u32 utf_handler::decode_utf8(std::vector<char> bytes) {
  switch (bytes.size()) {
  default:
    return 0;
  case 4:
    return ((u32)(bytes[0] & UTF8_4B_VMASK) << S18) | ((u32)(bytes[1] & CONT_MASK) << S12) |
           ((u32)(bytes[2] & CONT_MASK) << S6) | (u32)(bytes[3] & CONT_MASK);
  case 3:
    return ((u32)(bytes[0] & UTF8_3B_VMASK) << S12) | ((u32)(bytes[1] & CONT_MASK) << S6) | (u32)(bytes[2] & CONT_MASK);
  case 2:
    return ((u32)(bytes[0] & UTF8_2B_VMASK) << S6) | (u32)(bytes[1] & CONT_MASK);
  case 1:
    return (u32)bytes[0];
  }
}

// these are defined here, but need to go in a tpp
bool logger::log(std::string msg) {
  std::ofstream file("svte_log.txt", std::ios::app);
  if (!file.is_open()) {
    return false;
  }
  file << msg << std::endl;
  return true;
}

bool logger::log_str(std::string msg, std::string val) {
  std::ofstream file("svte_log.txt", std::ios::app);
  if (!file.is_open()) {
    return false;
  }
  file << msg << val << std::endl;
  return true;
}

bool logger::log_float(std::string msg, f32 val) {
  std::ofstream file("svte_log.txt", std::ios::app);
  if (!file.is_open()) {
    return false;
  }
  file << msg << val << std::endl;
  return true;
}

bool logger::log_int_unsigned(std::string msg, u64 val) {
  std::ofstream file("svte_log.txt", std::ios::app);
  if (!file.is_open()) {
    return false;
  }
  file << msg << val << std::endl;
  return true;
}

bool logger::log_int_signed(std::string msg, i64 val) {
  std::ofstream file("svte_log.txt", std::ios::app);
  if (!file.is_open()) {
    return false;
  }
  file << msg << val << std::endl;
  return true;
}
