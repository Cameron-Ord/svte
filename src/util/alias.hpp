#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#define NEW_LINE '\n'

typedef uint32_t u32;
typedef int32_t i32;
typedef float f32;

using char_mat = std::vector<u32>;
using char_mat_ptr = std::shared_ptr<char_mat>;
using const_char_mat_ptr = std::shared_ptr<const char_mat>;

