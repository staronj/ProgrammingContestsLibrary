// Jakub Staroń, 2016-2017
#include <celero/Celero.h>

#include "data_structures/find_and_union.h"

CELERO_MAIN

using namespace pcl;

constexpr size_t samples = 20;
constexpr size_t iterations = 10;

class QueriesFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {1000, 0},
        {1000 * 1000, 0}
    };
  }

  void setUp(int64_t experimentValue) override
  {
    N = experimentValue;
    using pcl::Random32;
    for (auto i: range<uint32>(0, experimentValue)) {
      queries.emplace_back(Random32() % experimentValue, Random32() % experimentValue);
    }
  }

  uint32 N;
  std::vector<std::pair<FindAndUnion::id_type, FindAndUnion::id_type>> queries;
};


BASELINE_F(FindAndUnion, FindAndUnion, QueriesFixture, samples, iterations)
{
  FindAndUnion findAndUnion(N);
  for (const auto& query: queries) {
    findAndUnion.union_sets(query.first, query.second);
  }
  celero::DoNotOptimizeAway(findAndUnion.find_root(N - 1));
  celero::DoNotOptimizeAway(findAndUnion.find_root(0));
}

