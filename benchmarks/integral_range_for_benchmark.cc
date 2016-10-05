// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "iterators.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 100;
constexpr size_t iterations = 30;

class SizeFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override {
    return {
        {100, 100},
        {10 * 1000, 100},
        {1000 * 1000, 100}
    };
  }

  void setUp(int64_t experimentValue) override {
    N = experimentValue;
  }

  int N;
};


BASELINE_F(For, For, SizeFixture, samples, iterations)
{
  int64 sum = 0;
  for (int i = 0; i < N; ++i)
    sum += i;
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(For, RangeFor, SizeFixture, samples, iterations)
{
  int64 sum = 0;
  for (int i: range(0, N))
    sum += i;
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(For, RangeForEx, SizeFixture, samples, iterations)
{
  int64 sum = 0;
  for (auto i: range(0, N))
    sum += i;
  celero::DoNotOptimizeAway(sum);
}
