#pragma once

#include <cstdint>
#include <memory>
#include <vector>


using char_mat = std::vector<uint32_t>;
using char_mat_ptr = std::shared_ptr<char_mat>;
using const_char_mat_ptr = std::shared_ptr<const char_mat>;

