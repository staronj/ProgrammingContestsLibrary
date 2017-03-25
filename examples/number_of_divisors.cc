// Jakub Staroń, 2016
//
// Solution to SPOJ problem, see
// http://www.spoj.com/problems/NUMDIV/

#include "headers.h"
#include "io.h"
#include "numeric/number_theory.h"

using namespace lib;

const uint64 kMaxN = power(10uLL, 18);
const uint64 kPrimesToPreprocess = power(10uLL, 6);
auto primes = numeric::PrimeNumbers(kPrimesToPreprocess);

uint64 NumberOfDivisors(uint64 N) {
  std::vector<uint32> counts;
  for (uint64 p: primes) {
    if (p * p > N)
      break;

    uint32 count = 0;
    while (divides(p, N)) {
      count++;
      N /= p;
    }
    if (count > 0)
      counts.push_back(count);
  }

  // We tests N with primes till 10^6.
  // If N is not prime then it's product of exactly two primes.
  if (N > 1) {
    if (numeric::IsPrime(N)) {
      counts.push_back(1);
    }
    else {
      uint64 sqrt = SquareFloor(N);
      if (sqrt * sqrt == N) {
        counts.push_back(2);
      }
      else {
        counts.push_back(1);
        counts.push_back(1);
      }
    }
  }

  uint64 result = 1;
  for (uint64 count: counts)
    result *= (count + 1u);
  return result;
}

int main() {
  std::ios::sync_with_stdio(false);

  uint32 T;
  read(std::cin, T);
  auto input = ReadSequence<uint32>(std::cin, T);
  iterate(input, [](uint32 N) {
    print("%0", NumberOfDivisors(N));
  });
  print("%0", lib::flush);

  return 0;
}
