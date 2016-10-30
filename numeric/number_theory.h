#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "numeric.h"

namespace lib {
namespace numeric {

/**
 * Returns positive modulo.
 *
 * Remember, in C++ ingative % positive gives negative result.
 */
uint64 Modulo(int64 value, uint64 modulo) {
  return ((int64(value) % int64(modulo)) + int64(modulo)) % int64(modulo);
}

/**
 * Returns greatest common divisor of two numbers.
 *
 * Note that arguments are unsigned.
 */
uint64 GCD(uint64 a, uint64 b) {
  while (a != 0) {
    uint64 t = a;
    a = b % a;
    b = t;
  }
  return b;
}

/**
 * Returns such x,y that a * x + b * y = GCD(a, b)
 *
 * Note that arguments are signed.
 */
int64_pair ExtendedGCD(int64 a, int64 b) {
  if(a == 0) {
    return {0, 1};
  }
  else {
    int64 x,y;
    std::tie(x, y) = ExtendedGCD(b % a, a);
    return {y - (b/a) * x, x};
  }
}

/**
 * Merges two congruences using Chinese remainder theorem.
 * Congruence x = a (mod p) is denoted as pair (a, p).
 *
 * If those congruences are contradictary throws std::runtime_error.
 *
 * Suppose we have two congruences:
 * x = a (mod p)
 * x = b (mod q)
 * Where a, p, b, q are given.
 *
 * We want to merge those congruences, ie
 * find c, r such that congruence
 * x = c (mod r) is equivalent to two above
 */
uint64_pair MergeCongruences(const uint64_pair first, const uint64_pair second) {
  int64 a, b, p, q;
  std::tie(a, p) = first;
  std::tie(b, q) = second;

  int64 gcd = GCD(p,q);
  if((a - b) % gcd != 0)
    throw std::runtime_error("MergeCongruences - contradiction!");

  const int64 x = ExtendedGCD(p, q).first;

  const int64 c = a + p * (b - a) / gcd * x;
  const int64 m = p * q / gcd;

  return {Modulo(c, m), m};
}

template <typename Iterator>
uint64_pair MergeCongruences(Iterator begin, Iterator end) {
  uint64_pair result{0, 1};
  for (const auto& elem: make_range(begin, end)) {
    result = MergeCongruences(result, elem);
  }
  return result;
}

/**
 * Returns (a + b) (mod modulo)
 */
uint32 Add32(uint32 a, uint32 b, uint32 modulo) {
  return (uint64(a) + uint64(b)) % uint64(modulo);
}

/**
 * Returns (a - b) (mod modulo)
 */
uint32 Subtract32(uint32 a, uint32 b, uint32 modulo) {
  return (uint64(modulo) + uint64(a) - uint64(b)) % uint64(modulo);
}

/**
 * Returns (a * b) (mod modulo)
 */
uint32 Multiply32(uint32 a, uint32 b, uint32 modulo) {
  return (uint64(a) * uint64(b)) % uint64(modulo);
}

/**
 * Returns a^n (mod modulo)
 */
uint32 PowerModulo32(uint32 a, uint64 n, uint32 modulo) {
  uint32 result = 1;
  while (n > 0) {
    if (n % 2 != 0)
      result = Multiply32(result , a, modulo);
    n /= 2;
    a = Multiply32(a , a, modulo);
  }
  return result;
}

#ifdef HAVE_INT128_TYPES

/**
 * Returns (a + b) (mod modulo)
 *
 * Note that this version uses uint128, which can be slow.
 */
uint64 Add64(uint64 a, uint64 b, uint64 modulo) {
  return (uint128(a) + uint128(b)) % uint128(modulo);
}

/**
 * Returns (a - b) (mod modulo)
 *
 * Note that this version uses uint128, which can be slow.
 */
uint64 Subtract64(uint64 a, uint64 b, uint64 modulo) {
  return (uint128(modulo) + uint128(a) - uint128(b)) % uint128(modulo);
}

/**
 * Returns (a * b) (mod modulo)
 *
 * Note that this version uses uint128, which can be slow.
 */
uint64 Multiply64(uint64 a, uint64 b, uint64 modulo) {
  return (uint128(a) * uint128(b)) % uint128(modulo);
}

/**
 * Returns a^n (mod modulo)
 *
 * Note that this version uses uint128, which can be slow.
 */
uint64 PowerModulo64(uint64 a, uint64 n, uint64 modulo) {
  uint64 result = 1;
  while (n > 0) {
    if (n % 2 != 0)
      result = Multiply64(result , a, modulo);
    n /= 2;
    a = Multiply64(a , a, modulo);
  }
  return result;
}

#endif

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
 * Returns vector of divisors of n in increasing order.
 *
 * Computational complexity is O(sqrt(n)).
 */
std::vector<uint32> Divisors(uint32 n) {
  std::vector<uint32> result;
  for (uint64 i = 1; i * i <= n; ++i) {
    if (divides<uint32>(i, n)) {
      uint32 d = uint32(n / i);
      result.push_back(uint32(i));
      if (d != i)
        result.push_back(d);
    }
  }
  std::sort(result.begin(), result.end());
  return result;
}

/**
 * Returns bit_vector of size n.
 *
 * ith bit is on when and only when i is prime number.
 *
 * Algorithm used is Eratostenes sieve. For comparison with other sieves
 * see benchmarks.
 */
bit_vector Sieve(uint32 n) {
  bit_vector V(n, true);
  V[0] = V[1] = 0;


  for(uint32 x = 2; x * x < n ; ++x) {
    if(V[x])
      for(uint32 y = x * x ; y < n ; y += x)
        V[y] = 0;
  }

  return V;
}

/**
 * Returns vector of primes less than n.
 */
std::vector<uint32> PrimeNumbers(uint32 n) {
  bit_vector primes = Sieve(n);
  std::vector<uint32> result;
  uint32 count = 0;
  for (uint32 i: range<uint32>(2, n))
    if (primes[i]) count++;
  result.reserve(count);
  for (uint32 i: range<uint32>(2, n))
    if (primes[i]) result.push_back(i);
  return result;
}

constexpr uint64 kPrimesPreprocessedNumber = 1 * 1000 * 1000;
constexpr uint64 kMaxFactorizableNumber = kPrimesPreprocessedNumber * kPrimesPreprocessedNumber;

/**
 * Returns factorization of n. Uses preprocessed primes to speed up factorization.
 *
 * Computational complexity is O(sqrt(n) / log n).
 */
std::vector<uint32> Factorize(uint32 n) {
  static std::vector<uint32> primes = PrimeNumbers(kPrimesPreprocessedNumber);
  std::vector<uint32> result;
  for (const uint64 p: primes) {
    if (n == 1 || p * p > n)
      break;

    while (divides<uint32>(p, n)) {
      result.push_back(uint32(p));
      n /= p;
    }
  }
  if (n > 1)
    result.push_back(n);
  return result;
}

/**
 * Returns prime divisors of n.
 */
std::vector<uint32> PrimeDivisors(uint32 n) {
  auto divisors = Factorize(n);
  divisors.erase(std::unique(divisors.begin(), divisors.end()), divisors.end());
  return divisors;
}

namespace detail {

/**
 * Performs miller rabin test on p with given set of witnesses.
*/
template<uint64... Witnesses>
struct MillerRabinPrimeTest {
  bool test(uint64 p) {
    constexpr uint64 witnesses[] = {Witnesses...};

    for (const auto witness: witnesses)
      if (p == witness)
        return true;

    const uint64 odd_factor = (p - 1uLL) / (1uLL << trailing_zeros(p - 1));

    for (const auto witness: witnesses) {
      uint64 k = odd_factor;
      uint64 x = PowerModulo64(witness, k, p);

      if (x == 1 || x == (p - 1))
        continue;

      while (k < p - 1) {
        x = Multiply64(x, x, p);

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

/**
 * Returns true if given number is prime.
 *
 * Uses deterministic Miller-Rabin test. See
 * https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test#Deterministic_variants
 */
bool IsPrime(uint64 p)
{
  if(p < 2)
    return false;

  constexpr uint64 threshold = 4759123141;
  detail::MillerRabinPrimeTest<2, 7, 61>
      small; // for n smaller than 4,759,123,141

  detail::MillerRabinPrimeTest<2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37>
      big; // for n smaller than 2^64

  if (p < threshold)
    return small.test(p);
  else
    return big.test(p);
}

/**
 * Returns true if g is primitive root modulo p.
 * Note that p must be prime.
 * Also note that g and p are 32 bits unsigned integers.
 */
bool IsPrimitiveRoot(uint32 g, uint32 p) {
  if (divides(p, g))
    return false;

  auto prime_divisors = PrimeDivisors(p - 1);
  for (auto d: prime_divisors) {
    if (PowerModulo32(g, (p - 1) / d, p) == 1)
      return false;
  }
  return true;
}

/**
 * Calculates inverse of n modulo p.
 *
 * Note that p must be a prime number.
 * Also note that n and p are 32 bits unsigned integers.
 */
uint32 Inverse(uint32 n, uint32 p) {
  return PowerModulo32(n, p - 2, p);
}

/**
 * Returns multiplicative order of a (mod p).
 *
 * If a is 0 (mod p) throws an exception of type std::runtime_error.
 * Note that p must be a prime number.
 * Also note that n and p are 32 bits unsigned integers.
 */
uint32 MultiplicativeOrder(uint32 a, uint32 p) {
  if (divides(p, a))
    throw std::runtime_error("MultiplicativeOrder - argument is 0 (mod p)!");

  auto divisors = Divisors(p - 1);
  for (auto d: divisors) {
    if (PowerModulo32(a, d, p) == 1)
      return d;
  }
  assert(0 && "Impossible to reach.");
}

/**
 * Calculates discrete logarithm of x in the base of a modulo p.
 *
 * If x is not in orbit of a throws an exception of type std::runtime_error.
 *
 * Uses baby-step giant-step algoritm, computational complexity is O(sqrt(p))
 *
 * Note that p must be a prime number.
 * Also note that a, x, and p are 32 bits unsigned integers.
 */
uint32 DiscreteLogarithm(const uint32 a, uint32 x, const uint32 p) {
  const uint32 order_of_a = MultiplicativeOrder(a, p);
  const uint32 sqrt = uint64(SquareCeiling(p - 1));
  std::unordered_map<uint32, uint32> small_steps;
  for (auto i: range<uint32>(0, sqrt))
    small_steps.emplace(PowerModulo32(a, i, p), i);

  uint32 giant_step = Inverse(PowerModulo32(a, sqrt, p), p);

  for (auto i: range<uint32>(0, sqrt)) {
    auto it = small_steps.find(x);
    if (it != small_steps.end()) {
      return (uint64(it->second) + uint64(i) * sqrt) % order_of_a;
    }
    x = Multiply32(x, giant_step, p);
  }
  throw std::runtime_error("DiscreteLogarithm - no logarithm found");
}

} // namespace numeric
} // namespace lib
