#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

using vec_2d = std::vector<std::vector<uint32_t>>;
using bufline = std::vector<uint32_t>*;
using const_bufline = const std::vector<uint32_t>*;
using vec_2d_ptr = std::shared_ptr<vec_2d>;
using const_vec_2d_ptr = std::shared_ptr<const vec_2d>;

