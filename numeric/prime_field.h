#pragma once
// Jakub Staroń, 2016-2017

#include "headers.h"

namespace pcl {
namespace numeric {

// Predeclarations

template <uint32 prime>
class prime_field;

/**
 * Returns a^n in Z_prime.
 */
template<uint32 prime>
prime_field<prime> power(prime_field<prime> a, uint64 n);

/**
 * Returns 1/lhs in Z_prime.
 *
 * Throws an std::runtime_error if lhs is not inversible (ie is 0).
 */
template<uint32 prime>
prime_field<prime> inverse(const prime_field<prime>& lhs);

template<uint32 prime>
std::ostream& operator<<(std::ostream& stream, const prime_field<prime>& lhs);

template<uint32 prime>
std::istream& operator>>(std::istream& stream, prime_field<prime>& lhs);

// Predeclarations End

namespace detail {

template <typename Integral>
constexpr
typename std::enable_if<std::is_unsigned<Integral>::value, uint32>::type
modulo(Integral value, uint32 prime) {
  return uint64(value) % uint64(prime);
}

template <typename Integral>
constexpr
typename std::enable_if<std::is_signed<Integral>::value, uint32>::type
modulo(Integral value, uint32 prime) {
  return ((int64(value) % int64(prime)) + int64(prime)) % int64(prime);
}

} // namespace detail

/**
 * Integers modulo compiled-time prime. Type safe.
 */
template <uint32 prime>
class prime_field {
public:
  constexpr prime_field():
      value_(0) { }

  template <typename Integral, typename = typename std::enable_if<std::is_integral<Integral>::value>::type>
  constexpr prime_field(Integral value):
      value_(detail::modulo(value, prime)) { }

  friend constexpr prime_field operator+(const prime_field& lhs, const prime_field& rhs) {
    return (uint64(lhs.value_) + uint64(rhs.value_)) % prime;
  }

  friend constexpr prime_field operator-(const prime_field& lhs, const prime_field& rhs) {
    return (uint64(prime) + uint64(lhs.value_) - uint64(rhs.value_)) % prime;
  }

  friend constexpr prime_field operator*(const prime_field& lhs, const prime_field& rhs) {
    return (uint64(lhs.value_) * uint64(rhs.value_)) % prime;
  }

  friend prime_field power <>(prime_field a, uint64 n);
  friend prime_field inverse <>(const prime_field& lhs);

  /**
   * Returns lhs/rhs in Z_prime.
   */
  friend prime_field operator/(const prime_field& lhs, const prime_field& rhs) {
    return lhs * inverse(rhs);
  }

  void operator+=(const prime_field& rhs);
  void operator-=(const prime_field& rhs);
  void operator*=(const prime_field& rhs);
  void operator/=(const prime_field& rhs);

  friend constexpr bool operator==(const prime_field& lhs, const prime_field& rhs) {
    return lhs.value_ == rhs.value_;
  }

  friend constexpr bool operator!=(const prime_field& lhs, const prime_field& rhs) {
    return !(lhs == rhs);
  }

  /**
   * Returns conversion of value to uint32.
   */
  constexpr uint32 value() const {
    return value_;
  }

  friend std::ostream& operator<< <>(std::ostream& stream, const prime_field& lhs);
  friend std::istream& operator>> <>(std::istream& stream, prime_field& lhs);

private:
  uint32 value_;
};

template<uint32 prime>
prime_field<prime> power(prime_field<prime> a, uint64 n) {
  if (n == 0)
    return 1;
  else if (a == 0)
    return 0;
  n %= (prime - 1); // Fermat little theorem
  prime_field<prime> result = 1;
  while (n > 0) {
    if (n % 2 == 1)
      result *= a;
    a *= a;
    n /= 2;
  }
  return result;
}

template<uint32 prime>
prime_field<prime> inverse(const prime_field<prime>& lhs) {
  if (lhs == 0)
    throw std::runtime_error("prime_field - inverse of zero");
  return power(lhs, prime - 2);
}

template<uint32 prime>
void prime_field<prime>::operator+=(const prime_field<prime>& rhs) {
  *this = *this + rhs;
}

template<uint32 prime>
void prime_field<prime>::operator-=(const prime_field<prime>& rhs) {
  *this = *this - rhs;
}

template<uint32 prime>
void prime_field<prime>::operator*=(const prime_field<prime>& rhs) {
  *this = *this * rhs;
}

template<uint32 prime>
void prime_field<prime>::operator/=(const prime_field<prime>& rhs) {
  *this = *this / rhs;
}

template<uint32 prime>
std::ostream& operator<<(std::ostream& stream, const prime_field<prime>& lhs) {
  return stream << lhs.value_;
}

template<uint32 prime>
std::istream& operator>>(std::istream& stream, prime_field<prime>& lhs) {
  int64 value;
  stream >> value;
  lhs = prime_field<prime>(value);
  return stream;
}

} // namespace numeric
} // namespace pcl
