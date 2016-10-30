#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"

namespace lib {

constexpr uint32 uint32_infinity = 1000 * 1000 * 1000;
constexpr int32 int32_infinity = uint32_infinity;

constexpr uint64 uint64_infinity = uint64(uint32_infinity) * uint64(uint32_infinity);
constexpr int64 int64_infinity = uint64_infinity;

constexpr uint32 uint32_prime1 = 0xFFFFFFFB;
constexpr uint32 uint32_prime2 = 0xFFFFFFBF;
constexpr uint64 uint64_prime1 = 0xFFFFFFFFFFFFFFC5;

template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
divides(T d, T n) {
  return (n % d) == 0;
}

template <typename T>
T abs(const T& value) {
  constexpr T zero(0);
  return (value < zero)? (-value) : (value);
}

template <typename T>
int signum(const T& value) {
  constexpr T zero(0);
  return (value == zero)? 0 : ((value < zero)? -1 : 1);
}

int64 power(int64 v, uint32 n) {
  int64 result = 1;
  while (n > 0) {
    if (n % 2 != 0)
      result *= v;
    n /= 2;
    v *= v;
  }
  return result;
}

template <typename T>
typename std::enable_if<!std::is_integral<T>::value, T>::type
power(T v, uint64 n, T one) {
  while (n > 0) {
    if (n % 2 != 0)
      one *= v;
    n /= 2;
    v *= v;
  }
  return one;
}

int32 signum(int64 n) {
  return (n == 0)? 0 : ((n < 0)? -1 : 1);
}

inline constexpr uint32 pop_count(uint64 n) {
  return __builtin_popcountl(n);
}

inline uint32 integer_log2(uint64 n) {
  return (n == 0)? 0u : uint32(63 - __builtin_clzll (n));
}

inline uint32 trailing_zeros(uint64 n) {
  return (n == 0)? 0u : __builtin_ctzll(n);
}

} // namespace lib
