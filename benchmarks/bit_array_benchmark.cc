// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "data_structures/bit_array.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 100;
constexpr size_t iterations = 1;

class SizeFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {10 * 1000, 0},
        {1000 * 1000, 0}
    };
  }

  void setUp(int64_t experimentValue) override
  {
    N = experimentValue;
  }

  uint32 N;
};

BASELINE_F(SieveOfEratostenes, Vector, SizeFixture, samples, iterations)
{
  bit_vector V(N, true);
  V[0] = V[1] = 0;


  for(uint32 x = 2; x * x < N ; ++x)
    if(V[x])
      for(uint32 y = x * x ; y < N ; y += x)
        V[y] = 0;

  celero::DoNotOptimizeAway(V[N - 1]);
}

BENCHMARK_F(SieveOfEratostenes, Array, SizeFixture, samples, iterations)
{
  BitArray V(N, true);
  V.set(0, false);
  V.set(1, false);

  for(uint32 x = 2; x * x < N ; ++x)
    if(V.get(x))
      for(uint32 y = x * x ; y < N ; y += x)
        V.set(y, false);

    celero::DoNotOptimizeAway(V.get(N - 1));
}
