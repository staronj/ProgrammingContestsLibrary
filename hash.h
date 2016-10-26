#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "numeric.h"
#include "numeric/prime_field.h"
#include "operators.h"

namespace lib {
namespace hash {

using hash_type = std::pair<numeric::prime_field<uint32_prime1>, numeric::prime_field<uint32_prime2>>;
using scalar_type = uint32;

constexpr hash_type zero{0, 0};
constexpr hash_type one{1, 1};
constexpr hash_type multipler{259, 440}; // primitive roots!!

inline constexpr hash_type add(const hash_type& lhs, const hash_type& rhs) {
  return lhs + rhs;
}

inline constexpr hash_type multiply(const hash_type& lhs, const hash_type& rhs) {
  return lhs * rhs;
}

inline constexpr hash_type multiply(const hash_type& lhs, const scalar_type & rhs) {
  return lhs * rhs;
}

inline constexpr hash_type subtract(const hash_type& lhs, const hash_type& rhs) {
  return lhs - rhs;
}

} // namespace hash
} // namespace lib
