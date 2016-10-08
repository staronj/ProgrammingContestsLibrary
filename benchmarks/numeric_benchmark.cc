// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "iterators.h"
#include "numeric.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 50;
constexpr size_t iterations = 5;

class SizeFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override {
    return {
        {10 * 1000, 100},
        {1000 * 1000, 0},
        {100 * 1000 * 1000, 1}
    };
  }

  void setUp(int64_t experimentValue) override {
    N = experimentValue;
  }

  uint32 N;
};


BASELINE_F(Sieve, Eratostenes, SizeFixture, samples, iterations)
{
  auto primes = sieve_of_eratostenes(N);
  celero::DoNotOptimizeAway(primes[N - 1]);
}

BENCHMARK_F(Sieve, Linear, SizeFixture, samples, iterations)
{
  auto primes = linear_sieve(N);
  celero::DoNotOptimizeAway(primes[N - 1]);
}

