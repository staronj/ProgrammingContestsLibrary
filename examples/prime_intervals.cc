// Jakub Staroń, 2016
//
// Solution to SPOJ problem, see
// http://www.spoj.com/problems/PRINT/

#include "headers.h"
#include "numeric/number_theory.h"
#include "io.h"

using namespace lib;

constexpr uint32 kMaxN = 2147483647u;
const uint32 kMaxPrimeNeeded = SquareFloor(kMaxN);

const auto primes = numeric::PrimeNumbers(kMaxPrimeNeeded + 1);

/**
 * Returns all primes from range [begin, end)
 *
 * Computational complexity is O(end - begin)
 */
std::vector<uint32> PrimesInRange(uint32 begin, uint32 end) {
  const uint32 length = end - begin;
  bit_vector primes_in_range(length, true);
  for (uint32 p: primes) {
    const uint32 first_divisible = (begin + (p - (begin % p)) % p);
    for (uint32 q = first_divisible; q < end; q += p) {
      if (q != p)
        primes_in_range[q - begin] = false;
    }
  }
  std::vector<uint32> result;
  for (auto i: range<uint32>(0, length)) {
    if (primes_in_range[i])
      result.push_back(begin + i);
  }
  return result;
}

int main(int argc, const char* argv[]) {
  std::ios::sync_with_stdio(false);

  uint32 T;
  read(std::cin, T);
  auto input = ReadSequence<uint32_pair>(std::cin, T);
  iterate(input, [](uint32_pair pair) {
    const auto primes = PrimesInRange(pair.first, pair.second + 1);
    for (auto p: primes)
      print("%0", p);
  });

  return 0;
}
