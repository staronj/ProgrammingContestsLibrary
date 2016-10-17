#pragma once
// Jakub Staroń, 2016

#include <iostream>
#include <sstream>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <memory>
#include <utility>
#include <functional>
#include <bitset>
#include <vector>
#include <array>
#include <list>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <cinttypes>
#include <ctime>
#include <cctype>
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

#ifdef __SIZEOF_INT128__
#define HAVE_INT128_TYPES
constexpr bool have_int128_types = true;
#else
constexpr bool have_int128_types = false;
#endif

#ifdef HAVE_INT128_TYPES
using int128 = __int128;
using uint128 = unsigned __int128;
#endif

using bit_vector = std::vector<bool>;

using char_pair     = std::pair<char, char>;
using bool_pair     = std::pair<bool, bool>;
using int32_pair    = std::pair<int32, int32>;
using uint32_pair   = std::pair<uint32, uint32>;
using int64_pair    = std::pair<int64, int64>;
using uint64_pair   = std::pair<uint64, uint64>;

} // namespace lib
