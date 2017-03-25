// Jakub Staroń, 2016
//
// Solution to SPOJ problem, see
// http://www.spoj.com/problems/DIVFACT/

#include "headers.h"
#include "numeric/number_theory.h"
#include "numeric/prime_field.h"
#include "io.h"

using namespace lib;

constexpr uint32 kPrime = 1000 * 1000 * 1000 + 7;
constexpr uint32 kMaxN = 50 * 1000;

auto primes = numeric::PrimeNumbers(kMaxN + 1);
using result_type = numeric::prime_field<kPrime>;


/**
 * Returns power of given prime in factorization of N!.
 *
 * Note that p must be prime number.
 * Computational complexity is O(log_p(N)).
 */
uint32 PowerInFactorial(uint32 p, uint64 N) {
  uint32 result = 0;
  for (uint64 q = p; q <= N; q *= p)
    result += N / q;
  return result;
}


class Application {
public:
  Application() { }

  uint64 Result(uint32 N) {
    result_type result = 1;
    for (uint32 p: primes) {
      if (p > N)
        break;
      result *= (PowerInFactorial(p, N) + 1);
    }

    return result.value();
  }

  void Run() {
    auto input = ReadSequence<uint32>(std::cin, T);
    iterate(input, [this](uint32 N) {
      print("%0", Result(N));
    });
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
