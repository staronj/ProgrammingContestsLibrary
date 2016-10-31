#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "numeric.h"
#include "numeric/prime_field.h"
#include "operators.h"

namespace lib {
namespace hash {

/**
 * Type for storing hash.
 *
 * It's guaranteed that have at least 63 bit space of values.
 */
using hash_type = std::pair<numeric::prime_field<uint32_prime1>, numeric::prime_field<uint32_prime2>>;
using scalar_type = uint32;

constexpr hash_type zero{0, 0};
constexpr hash_type one{1, 1};
constexpr hash_type multipler{259, 440}; // primitive roots!!

/**
 * Return sum of two hashes.
 */
inline constexpr hash_type add(const hash_type& lhs, const hash_type& rhs) {
  return lhs + rhs;
}

/**
 * Returns multiplication of two hashes.
 */
inline constexpr hash_type multiply(const hash_type& lhs, const hash_type& rhs) {
  return lhs * rhs;
}

/**
 * Returns multiplication of hash and scalar.
 */
inline constexpr hash_type multiply(const hash_type& lhs, const scalar_type & rhs) {
  return lhs * rhs;
}

/**
 * Returns substraction of two hashes.
 */
inline constexpr hash_type subtract(const hash_type& lhs, const hash_type& rhs) {
  return lhs - rhs;
}

/**
 * Computes hash of sequence.
 *
 * It's guaranteed that hash of empty sequence is equal to hash::zero.
 */
template <typename Iterator>
hash_type hash(Iterator begin, Iterator end) {
  hash_type result = zero;
  hash_type power = one;
  for (auto c: make_range(begin, end)) {
    hash_type m = multiply(power, scalar_type(c));
    result = add(result, m);
    power = multiply(power, multipler);
  }
  return result;
}

/**
 * Computes hash of string.
 */
hash_type hash(const std::string& text) {
  return hash(text.begin(), text.end());
}

/**
 * Computes hash of C-string.
 */
hash_type hash(const char* text) {
  return hash(text, text + std::strlen(text));
}

} // namespace hash
} // namespace lib
