// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "iterators.h"
#include "numeric/number_theory.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 50;
constexpr size_t iterations = 5;

class SizeFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override {
    return {
        {100 * 1000, 10},
        {1000 * 1000, 5},
        {10 * 1000 * 1000, 1},
        {100 * 1000 * 1000, 1}
    };
  }

  void setUp(int64_t experimentValue) override {
    N = experimentValue;
  }

  uint32 N;
};


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

class NumbersFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override {
    return {
        {1000, 10},
        {1000 * 1000, 10},
        {1000 * 1000 * 1000, 10},
        {1000uLL * 1000 * 1000 * 1000, 10},
        {1000uLL * 1000 * 1000 * 1000 * 100, 10},
    };
  }

  void setUp(int64_t experimentValue) override {
    number = (lib::random() % (experimentValue - 2)) + 2;
  }

  uint64 number;
};

constexpr size_t factorization_samples = 10000;

BASELINE_F(Factorization, Factorize, NumbersFixture, samples, iterations)
{
  auto v = numeric::Factorize(number);
  celero::DoNotOptimizeAway(v.front() + v.back());
}
