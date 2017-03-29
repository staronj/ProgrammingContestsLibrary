#pragma once
// Jakub Staroń, 2016-2017

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
#include <forward_list>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <cinttypes>
#include <ctime>
#include <cctype>
#include <cstring>
#include <random>
#include <cassert>

namespace pcl {

using byte    = unsigned char;
using int8    = std::int8_t;
using uint8   = std::uint8_t;
using int16   = std::int16_t;
using uint16  = std::uint16_t;
using int32   = std::int32_t;
using uint32  = std::uint32_t;
using int64   = std::int64_t;
using uint64  = std::uint64_t;

/**
 * Random number generator. Returns 32 bits numbers.
 *
 * You should prefer this random number generator over
 * C's rand(), because rand() max range is not strictly
 * defined and can be supringsly small (like 10000).
 */
std::knuth_b Random(time(0));

/**
 * 32 bits random generator.
 */
uint32 Random32() {
  return Random();
}

/**
 * 64 bits random generator.
 */
uint64 Random64() {
  return uint64(Random32()) << 32 | uint64(Random32());
}

/**
 * Returns random number from range [0, n).
 */
uint32 Random32(uint32 n) {
  return Random32() % n;
}

/**
 * Returns random number from range [a, b).
 *
 * a must be strictly less than b.
 */
uint32 Random32(uint32 a, uint32 b) {
  assert(a < b);
  return (Random32() % (b - a)) + a;
}

/**
 * Returns random number from range [0, n).
 */
uint64 Random64(uint64 n) {
  return Random64() % n;
}

/**
 * Returns random number from range [a, b).
 *
 * a must be strictly less than b.
 */
uint64 Random64(uint64 a, uint64 b) {
  assert(a < b);
  return (Random64() % (b - a)) + a;
}


#ifdef __SIZEOF_INT128__
#define HAVE_INT128_TYPES
#endif

#define USE_INT128_TYPES_IF_AVAILABLE 0

#if defined(HAVE_INT128_TYPES) && USE_INT128_TYPES_IF_AVAILABLE
#define USE_INT128_TYPES
#endif

#ifdef USE_INT128_TYPES
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

} // namespace pcl
