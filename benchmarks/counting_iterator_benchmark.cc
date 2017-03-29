// Jakub Staroń, 2016-2017
#include <celero/Celero.h>

#include "iterators.h"

CELERO_MAIN

using namespace pcl;

constexpr size_t samples = 50;
constexpr size_t iterations = 30;

class SizeFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {100, 30},
        {10 * 1000, 30},
        {1000 * 1000, 30}
    };
  }

  void setUp(int64_t experimentValue) override
  {
    N = experimentValue;
  }

  int N;
};


BASELINE_F(BuildVector, For, SizeFixture, samples, iterations)
{
  std::vector<int> V;
  for (int i = 0; i < N; ++i)
    V.push_back(i);
  celero::DoNotOptimizeAway(V.size());
  celero::DoNotOptimizeAway(V[V[N - 1]]);
}

BENCHMARK_F(BuildVector, ReserveFor, SizeFixture, samples, iterations)
{
  std::vector<int> V;
  V.reserve(N);
  for (int i = 0; i < N; ++i)
    V.push_back(i);
  celero::DoNotOptimizeAway(V.size());
  celero::DoNotOptimizeAway(V[V[N - 1]]);
}

BENCHMARK_F(BuildVector, CountingIterator, SizeFixture, samples, iterations)
{
  std::vector<int> V(make_counting_iterator(0), make_counting_iterator(N));
  celero::DoNotOptimizeAway(V.size());
  celero::DoNotOptimizeAway(V[V[N - 1]]);
}
