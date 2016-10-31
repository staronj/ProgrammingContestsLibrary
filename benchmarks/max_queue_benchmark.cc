// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "data_structures/max_queue.h"
#include "iterators.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 10;
constexpr size_t iterations = 10;

class QueriesFixture : public celero::TestFixture
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
    using lib::Random32;
    std::queue<int32> queue;
    for (auto i: range<uint32>(0, experimentValue)) {
      if (!queue.empty() && Random32() % 2 == 0) {
        queries.push_back(-queue.front());
        queue.pop();
      }
      else {
        int32 value = 1 + Random32() % 1000000uLL;
        queries.push_back(value);
        queue.push(value);
      }
    }
  }

  std::vector<int32> queries;
};


BASELINE_F(MaxQueue, Set, QueriesFixture, samples, iterations)
{
  uint64 sum = 0;
  std::set<int, std::greater<int>> set;
  for (auto i: queries) {
    if (i < 0) {
      sum += *set.begin();
      set.erase(-i);
    }
    else {
      set.insert(i);
      sum += *set.begin();
    }
  }
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(MaxQueue, MaxQueue, QueriesFixture, samples, iterations)
{
  uint64 sum = 0;
  MaxQueue<int> queue;
  for (auto i: queries) {
    if (i < 0) {
      sum += queue.max();
      queue.pop();
    }
    else {
      queue.push(i);
      sum += queue.max();
    }
  }
  celero::DoNotOptimizeAway(sum);
}

