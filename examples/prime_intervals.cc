// Jakub Staroń, 2016
//
// Solution to SPOJ problem, see
// http://www.spoj.com/problems/PRINT/

#include "headers.h"
#include "numeric/number_theory.h"
#include "io.h"

using namespace lib;

constexpr uint32 kMaxN = 2147483647u;
const uint32 kMaxPrimeNeeded = numeric::SquareFloor(kMaxN);

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


class Application {
public:
  Application() { }

  void Run() {
    for(auto i: range<uint32>(0, T)) {
      uint32 L, U;
      read(std::cin, L, U);
      const auto primes = PrimesInRange(L, U + 1);
      for (auto p: primes)
        print("%0", p);
    }
  }

  void LoadData() {
    read(std::cin, T);
  }

private:
  uint32 T;
};

int main(int argc, const char* argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Application application;
  application.LoadData();
  application.Run();
  return 0;
}
