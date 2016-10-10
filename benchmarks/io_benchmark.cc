// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "data_structures/indexer.h"
#include "iterators.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 10;
constexpr size_t iterations = 10;

class NumbersLoadFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {1000, 0},
        {1000 * 1000, 0},
        {10 * 1000 * 1000, 0}
    };
  }

  void setUp(int64_t experimentValue) override
  {
    N = experimentValue;
    constexpr uint32 kMaxNumber = 1000 * 1000 * 1000;
    std::string string;
    for (auto i: range(int64_t(0), experimentValue)) {
      string += std::to_string(lib::random() % kMaxNumber);
      string += '\n';
    }
    stream.str(string);
  }

  std::istringstream stream;
  uint32 N;
};


BASELINE_F(Load, Istringstream, NumbersLoadFixture, samples, iterations)
{
  uint64 sum = 0;
  for (auto i: range(0u, N)) {
    uint32 k;
    stream >> k;
    sum += k;
  }
  celero::DoNotOptimizeAway(sum);
}

