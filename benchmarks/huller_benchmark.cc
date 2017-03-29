// Jakub Staroń, 2016-2017
#include <celero/Celero.h>

#include "data_structures/huller.h"
#include "iterators.h"

CELERO_MAIN

using namespace pcl;

constexpr size_t samples = 10;
constexpr size_t iterations = 1;

class InsertFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {1000 * 1000, 0}
    };
  }

  void setUp(int64_t experimentValue) override
  {
    using pcl::Random32;
    for (auto i: range<uint32>(0, experimentValue))
      queries.emplace_back(Random32(), Random32());
  }

  std::vector<std::pair<int64, int64>> queries;
};

BASELINE(SequentInsert, Huller, samples, iterations)
{
  Huller<int64> huller;
  for (auto i: range<int64>(0, 1000 * 1000))
    huller.insert(i, -i * (i + 1));
  celero::DoNotOptimizeAway(huller.evaluate(0));
}

BENCHMARK(SequentInsert, Set, samples, iterations)
{
  std::set<std::pair<int64, int64>> set;
  for (auto i: range<int64>(0, 1000 * 1000))
    set.emplace(i, -i * (i + 1));
  celero::DoNotOptimizeAway(set.size());
}

BASELINE_F(RandomInsert, Huller, InsertFixture, samples, iterations)
{
  Huller<int64> huller;
  for (auto elem: queries)
    huller.insert(elem);
  celero::DoNotOptimizeAway(huller.evaluate(0));
}

BENCHMARK_F(RandomInsert, Set, InsertFixture, samples, iterations)
{
  std::set<std::pair<int64, int64>> set;
  for (auto elem: queries)
    set.insert(elem);
  celero::DoNotOptimizeAway(set.size());
}

class QueryFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {1000 * 1000, 0}
    };
  }

  void setUp(int64_t experimentValue) override
  {
    using pcl::Random32;
    for (auto i: range<uint32>(0, experimentValue))
      huller.insert(i, -i * (i + 1));

    for (auto i: range<uint32>(0, experimentValue))
      queries.push_back(Random32());
  }

  std::vector<int64> queries;
  Huller<int64> huller;
};

BASELINE_F(Query, Huller, QueryFixture, samples, iterations)
{
  int64 sum = 0;
  for (auto query: queries)
    sum += huller.evaluate(query);
  celero::DoNotOptimizeAway(sum);
}
