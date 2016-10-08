#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "numeric.h"

namespace lib {

template <typename T>
struct hash_traits;

template <>
struct hash_traits<uint32_pair> {
  using hash_type = uint32_pair;
  using work_type = uint64_pair;
  using scalar_type = uint64;

  static constexpr work_type modulo{uint32_prime1, uint32_prime2};
  static constexpr work_type zero{0, 0};
  static constexpr work_type one{1, 1};
  static constexpr work_type multipler{257, 434};

  static constexpr hash_type add(const hash_type& lhs, const hash_type& rhs) {
    return (work_type(lhs) + work_type(rhs)) % modulo;
  }

  static constexpr hash_type multiply(const hash_type& lhs, const hash_type& rhs) {
    return (work_type(lhs) * work_type(rhs)) % modulo;
  }

  static constexpr hash_type substract(const hash_type& lhs, const hash_type& rhs) {
    return (modulo + work_type(lhs) - work_type(rhs)) % modulo;
  }
};

constexpr hash_traits<uint32_pair>::work_type hash_traits<uint32_pair>::modulo;
constexpr hash_traits<uint32_pair>::work_type hash_traits<uint32_pair>::zero;
constexpr hash_traits<uint32_pair>::work_type hash_traits<uint32_pair>::one;
constexpr hash_traits<uint32_pair>::work_type hash_traits<uint32_pair>::multipler;

} // namespace lib
