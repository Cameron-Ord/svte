#include "../util.hpp"

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
static const uint8_t MASKS[] = {UTF8_1B_MASK, UTF8_2B_MASK, UTF8_3B_MASK,
                                UTF8_4B_MASK};

static const uint8_t SIGS[] = {UTF8_1B_SIG, UTF8_2B_SIG, UTF8_3B_SIG,
                               UTF8_4B_SIG};

char utf_handler::utf8_byte_count(const char ch) {
  for (int i = 0; i < UTF8_MAX_BYTES; i++) {
    if ((ch & MASKS[i]) == SIGS[i]) {
      return i + 1;
    }
  }
  return 0;
}

std::vector<char> utf_handler::encode_utf8(uint32_t cp) {
  auto cont_append = [](uint32_t cp, int bytecount) {
    std::vector<char> bytes(bytecount);
    for (int i = bytecount - 1; i >= 0; i--) {
      bytes[i] = CONT_SIG | (cp & CONT_MASK);
      cp >>= 6;
    }
    bytes.push_back(NULLCHAR);
    return bytes;
  };

  if (cp <= UTF8_1B_MAX) {
    return std::vector<char>{static_cast<char>(cp), NULLCHAR};
  } else if (cp <= UTF8_2B_MAX) {
    char front = UTF8_2B_SIG | ((cp >> S6) & UTF8_2B_VMASK);
    std::vector<char> b = cont_append(cp, 1);
    b.insert(b.begin(), front);
    return b;
  } else if (cp <= UTF8_3B_MAX) {
    char front = UTF8_3B_SIG | ((cp >> S12) & UTF8_3B_VMASK);
    std::vector<char> b = cont_append(cp, 2);
    b.insert(b.begin(), front);
    return b;
  } else if (cp <= UTF8_4B_MAX) {
    char front = UTF8_4B_SIG | ((cp >> S18) & UTF8_4B_VMASK);
    std::vector<char> b = cont_append(cp, 3);
    b.insert(b.begin(), front);
    return b;
  } else {
    return std::vector<char>{NULLCHAR};
  }
}

uint32_t utf_handler::decode_utf8(std::vector<unsigned char> bytes) {
  switch (bytes.size()) {
  default:
    return 0;
  case 4:
    return ((uint32_t)(bytes[0] & UTF8_4B_VMASK) << S18) |
           ((uint32_t)(bytes[1] & CONT_MASK) << S12) |
           ((uint32_t)(bytes[2] & CONT_MASK) << S6) |
           (uint32_t)(bytes[3] & CONT_MASK);
  case 3:
    return ((uint32_t)(bytes[0] & UTF8_3B_VMASK) << S12) |
           ((uint32_t)(bytes[1] & CONT_MASK) << S6) |
           (uint32_t)(bytes[2] & CONT_MASK);
  case 2:
    return ((uint32_t)(bytes[0] & UTF8_2B_VMASK) << S6) |
           (uint32_t)(bytes[1] & CONT_MASK);
  case 1:
    return (uint32_t)bytes[0];
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
