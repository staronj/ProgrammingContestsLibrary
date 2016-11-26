#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"

namespace lib {

constexpr uint64 Hundred = 100uLL;
constexpr uint64 Thousand = 1000uLL;
constexpr uint64 Million = Thousand * Thousand;
constexpr uint64 Billion = Thousand * Thousand * Thousand;

constexpr uint32 uint32_infinity = Billion;
constexpr int32 int32_infinity = uint32_infinity;

constexpr uint64 uint64_infinity = Billion * Billion;
constexpr int64 int64_infinity = uint64_infinity;

constexpr uint32 uint32_prime1 = 0xFFFFFFFB;
constexpr uint32 uint32_prime2 = 0xFFFFFFBF;
constexpr uint64 uint64_prime1 = 0xFFFFFFFFFFFFFFC5;

/**
 * Returns true if d divides n.
 */
template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
divides(T d, T n) {
  return (n % d) == 0;
}

/**
 * Returns ceiling of a/b;
 */
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
ceiling_divide(T a, T b) {
  return (a + b - 1) / b;
}

/**
 * Returns abs(value) independently of value type.
 */
template <typename T>
T abs(const T& value) {
  constexpr T zero(0);
  return (value < zero)? (-value) : (value);
}

/**
 * Returns 1, 0 or -1 depending on signum of value.
 * Return type is int.
 */
template <typename T>
inline constexpr
typename std::enable_if<std::is_integral<T>::value, int>::type
signum(const T& value) {
  return (value > T(0))? 1 : ((value < T(0))? -1 : 0);
}

int64 power(int64 v, uint32 n) {
  int64 result = 1;
  while (n > 0) {
    if (n % 2 != 0)
      result *= v;
    n /= 2;

    if (n > 0) // to protect against integer overflow (Undefined Behaviour)
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

/**
 * Returns number of turned on bits.
 */
inline constexpr uint32 pop_count(uint64 n) {
  return __builtin_popcountl(n);
}

/**
 * Returns index of least significant one in n.
 * Alternatively returns max k such that 2^k | n.
 *
 * If n is equal to 0 result is undefined.
 */
inline constexpr uint32 least_significant_one(uint64 n) {
  return __builtin_ffsll(n) - 1;
}

/**
 * Returns floor of binary logaritm of number, ie
 * max k such that 2^k <= n.
 *
 * If n is equal to 0 result is undefined.
 */
inline constexpr uint32 most_significant_one(uint64 n) {
  return 63 - __builtin_clzll(n);
}

/*

Alternative implementations:

inline uint32 least_significant_one(uint64 n) {
  uint64 powOfTwo = ( (-n) & n ) - 1uLL;
  return pop_count(powOfTwo);
}

inline uint32 most_significant_one(uint64 x) {
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >>16);
  x = x | (x >>32);
  return 63 - pop_count(~x);
}

 */

/**
 * Returns floor(sqrt(n)).
 */
uint64 SquareFloor(uint64 n) {
  uint64 result = static_cast<uint64>(sqrt(static_cast<double>(n)));
  do { ++result; } while(result * result <= n);
  do { --result; } while(result * result > n);
  return result;
}

/**
 * Returns ceil(sqrt(n)).
 */
uint64 SquareCeiling(uint64 n) {
  uint64 result = SquareFloor(n);
  if (result * result < n) result++;
  return result;
}

/**
 * Returns vector of prefix sums, ie for sequence a_0, a_1, ..., a_n
 * returns sequence 0, a_0, a_0 + a_1, ..., a_0 + a_1 + ... + a_n
 *
 * Note that returned vector have size n + 1.
 * Also note, that function need a template parameter to specify
 * value type.
 *
 * Example:
 * <pre>
 * std::vector<uint32> v = {1, 2, 3}
 * PrefixSums<uint64>(v.begin(), v.end()); // returns vector<uint64>{0, 1, 3, 6}
 * </pre>
 */
template <typename ValueType, typename Iterator>
std::vector<ValueType> PrefixSums(Iterator begin, Iterator end) {
  auto length = std::distance(begin, end);
  std::vector<ValueType> result;
  result.reserve(length + 1);
  result.push_back(ValueType(0));
  for (const auto& elem: make_range(begin, end)) {
    result.push_back(result.back() + elem);
  }
  return result;
}

/**
 * Returns position of element with maximum value and value.
 *
 * Takes range of iterators, function,
 * default value and comparator for function results.
 *
 * If range is empty or no value was greater than
 * default value, returns pair (default_value, end)
 *
 * Returns position of first element for which value
 * of function is the biggest.
 */
template <typename Iterator, typename Function, typename Comparator>
auto MaximumOrDefault(Iterator begin, Iterator end,
                      Function function,
                      decltype(function(*begin)) defaultValue,
                      Comparator comparator) ->
std::pair<Iterator, decltype(function(*begin))> {
  auto biggestValue = defaultValue;
  auto biggestPosition = end;
  for (auto it = begin; it != end; ++it) {
    auto value = function(*it);
    if (comparator(biggestValue, value)) {
      biggestValue = value;
      biggestPosition = it;
    }
  }
  return std::make_pair(biggestPosition, biggestValue);
}

/**
 * Returns position of element with maximum value and value.
 *
 * Takes range of iterators, function and default value.
 * Function result must be comparable.
 *
 * If range is empty or no value was greater than
 * default value, returns pair (default_value, end)
 *
 * Returns position of first element for which value
 * of function is the biggest.
 */
template <typename Iterator, typename Function>
auto MaximumOrDefault(Iterator begin, Iterator end,
                      Function function,
                      decltype(function(*begin)) defaultValue) ->
std::pair<Iterator, decltype(function(*begin))> {
  return MaximumOrDefault(std::move(begin),
                          std::move(end),
                          std::move(function),
                          defaultValue,
                          std::less<decltype(defaultValue)>());
}

/**
 * Returns position of element with maximum value and value.
 *
 * Takes range of iterators, function and
 * comparator for function results.
 *
 * Returns position of first element for which value
 * of function is the biggest.
 */
template <typename Iterator, typename Function, typename Comparator>
auto Maximum(Iterator begin, Iterator end, Function function, Comparator comparator) ->
std::pair<Iterator, decltype(function(*begin))> {
  auto value = function(*begin);
  return MaximumOrDefault(std::next(begin), end, function, value, comparator);
}

/**
 * Returns position of element with maximum value.
 *
 * Takes range of iterators and function.
 * Function result must be comparable.
 *
 * Returns position of first element for which value
 * of function is the biggest.
 */
template <typename Iterator, typename Function>
auto Maximum(Iterator begin, Iterator end, Function function) ->
std::pair<Iterator, decltype(function(*begin))> {
  return Maximum(std::move(begin),
                 std::move(end),
                 std::move(function),
                 std::less<decltype(function(*begin))>());
}

} // namespace lib
