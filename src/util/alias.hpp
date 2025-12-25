#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#define SPACE_CHAR ' '
#define NEW_LINE '\n'

typedef uint32_t u32;
typedef int32_t i32;
typedef uint8_t u8;
typedef int8_t i8;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;

using char_mat = std::vector<u32>;
using char_mat_ptr = std::shared_ptr<char_mat>;
using const_char_mat_ptr = std::shared_ptr<const char_mat>;
