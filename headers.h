#pragma once
// Jakub Staroń, 2016

#include <iostream>
#include <sstream>
#include <iterator>
#include <type_traits>
#include <bitset>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <cinttypes>
#include <random>

namespace lib {

std::knuth_b random(time(0));

using int8    = std::int8_t;
using uint8   = std::uint8_t;
using int16   = std::int16_t;
using uint16  = std::uint16_t;
using int32   = std::int32_t;
using uint32  = std::uint32_t;
using int64   = std::int64_t;
using uint64  = std::uint64_t;

using bit_vector = std::vector<bool>;

using int32_pair    = std::pair<int32, int32>;
using uint32_pair   = std::pair<uint32, uint32>;
using int64_pair    = std::pair<int64, int64>;
using uint64_pair    = std::pair<uint64, uint64>;

} // namespace lib
