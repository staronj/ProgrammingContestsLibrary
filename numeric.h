#pragma once
// Jakub Staroń, 2016

#include "headers.h"

namespace lib {

constexpr uint32 uint32_prime1 = 0xFFFFFFFB;
constexpr uint32 dupa = 0xFFFFFFFB;
constexpr uint32 uint32_prime2 = 0xFFFFFFBF;
constexpr uint64 uint64_prime1 = 0xFFFFFFFFFFFFFFC5;

template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
divides(T d, T n) {
  return (n % d) == 0;
}

int64 GCD(int64 a, int64 b) {
  while (a != 0) {
    int t = a;
    a = b % a;
    b = t;
  }
  return b;
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

#ifdef HAVE_INT128_TYPES

uint64 multiply_modulo(uint64 a, uint64 b, uint64 modulo) {
  return ((uint128(a) * uint128(b)) % uint128(modulo));
}

#else

uint64 multiply_modulo(uint64 a, uint64 b, uint64 modulo) {
  uint64 result = 0;
  while (b > 0) {
    if(b % 2 == 0)
      result = (result + a) % modulo;
    a = (a << 1) % modulo;
    b /= 2;
  }
  return result;
}

#endif

uint64 pow_modulo(uint64 v, uint64 n, uint64 modulo) {
  uint64 result = 1;
  while (n > 0) {
    if (n % 2 != 0)
      result = multiply_modulo(result , v, modulo);
    n /= 2;
    v = multiply_modulo(v , v, modulo);
  }
  return result;
}

template <typename T>
typename std::enable_if<!std::is_integral<T>::value, T>::type
pow_modulo(T v, uint64 n, const T &modulo, T one) {
  while (n > 0) {
    if (n % 2 != 0)
      one = (one * v) % modulo;
    n /= 2;
    v = (v * v) % modulo;
  }
  return one;
}

int32 signum(int64 n) {
  return (n == 0)? 0 : ((n < 0)? -1 : 1);
}

uint64 square(uint64 n) {
  uint64 result = static_cast<uint64>(sqrt(static_cast<double>(n)));
  do { ++result; } while(result * result <= n);
  do { --result; } while(result * result > n);
  return result;
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

bit_vector sieve_of_eratostenes(uint32 n) {
  bit_vector V(n, true);
  V[0] = V[1] = 0;


  for(uint32 x = 2; x * x < n ; ++x) {
    if(V[x])
      for(uint32 y = x * x ; y < n ; y += x)
        V[y] = 0;
  }

  return V;
}

bit_vector linear_sieve(uint32 n) {
  bit_vector V(n, true);
  V[0] = V[1] = false;
  uint64 p = 2;

  while(p * p < n) {
    uint64 q = p;
    while(p * q < n) {
      uint64 x = p * q;
      while(x < n) {
        V[x] = false;
        x *= p;
      }

      while(!V[++q]);
    }

    while(!V[++p]);
  }
  return V;
}

inline bit_vector sieve(uint32 n) {
  return sieve_of_eratostenes(n);
}

namespace detail {

template<uint64... Tests>
struct miller_rabin_prime_test {
  bool test(uint64 p) {

    constexpr uint64 tests[] = {Tests...};

    for (const auto q: tests)
      if (p == q)
        return true;

    const uint64 odd_factor = (p - 1uLL) / (1uLL << trailing_zeros(p - 1));

    for (const auto q: tests) {
      uint64 k = odd_factor;
      uint64 x = pow_modulo(q, k, p);

      if (x == 1 || x == (p - 1))
        continue;

      while (k < p - 1) {
        x = multiply_modulo(x, x, p);

        if (x == (p - 1))
          break;
        else if (x == 1)
          return false;

        k *= 2;
      }

      if (x != p - 1)
        return false;
    }
    return true;
  }
};

} // namespace detail

bool miller_rabin_prime_test(uint64 p)
{
  if(p < 2)
    return false;

  constexpr uint64 threshold = 4759123141;
  detail::miller_rabin_prime_test<2, 7, 61>
      small; // for n smaller than 4,759,123,141

  detail::miller_rabin_prime_test<2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37>
      big; // for n smaller than 2^64

  if (p < threshold)
    return small.test(p);
  else
    return big.test(p);
}

inline bool is_prime(uint64 p) {
  return miller_rabin_prime_test(p);
}

} // namespace lib
