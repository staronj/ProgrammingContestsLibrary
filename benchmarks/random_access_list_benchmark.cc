// Jakub Staroń, 2016-2017
#include <celero/Celero.h>

#include "iterators.h"
#include "io.h"
#include "data_structures/random_access_list.h"

CELERO_MAIN

using namespace pcl;

constexpr size_t samples = 5;
constexpr size_t iterations = 5;

class InitializationFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {100, 0},
        {10 * 1000, 0},
        {1000 * 1000, 0}
    };
  }

  void setUp(int64 experimentValue) override {
    size = experimentValue;
  }

  uint32 size;
};

BASELINE_F(Initialization, Set, InitializationFixture, samples, iterations)
{
  std::set<int> set(counting_iterator<int>(0), counting_iterator<int>(size));
}

BENCHMARK_F(Initialization, List, InitializationFixture, samples, iterations)
{
  std::list<int> list(counting_iterator<int>(0), counting_iterator<int>(size));
}

BENCHMARK_F(Initialization, RAList, InitializationFixture, samples, iterations)
{
  RandomAccessList<int> list(counting_iterator<int>(0), counting_iterator<int>(size));
}

class SearchFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {100, 0},
        {10 * 1000, 0},
        {1000 * 1000, 0}
    };
  }

  void setUp(int64 experimentValue) override {
    size = experimentValue;
    auto r = range<int>(0, size);
    set.insert(r.begin(), r.end());
    vector.assign(r.begin(), r.end());
    list.assign(r.begin(), r.end());
    randomAccessList.assign(r.begin(), r.end());
  }

  uint32 size;
  std::set<int> set;
  std::vector<int> vector;
  std::list<int> list;
  RandomAccessList<int> randomAccessList;
};

BASELINE_F(BinarySearch, Set, SearchFixture, samples, 100 * iterations)
{
  set.lower_bound(size / 2);
}

BENCHMARK_F(BinarySearch, Vector, SearchFixture, samples, 100 * iterations)
{
  std::lower_bound(vector.begin(), vector.end(), size / 2);
}

BENCHMARK_F(BinarySearch, RAList, SearchFixture, samples, 100 * iterations)
{
  randomAccessList.find([this](int i) {
    return i >= size / 2;
  });
}


BASELINE_F(Iteration, Set, SearchFixture, samples, 2 * iterations)
{
  uint64 sum = 0;
  for (auto i: set) {
    sum += i;
  }
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(Iteration, List, SearchFixture, samples, 2 * iterations)
{
  uint64 sum = 0;
  for (auto i: list) {
    sum += i;
  }
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(Iteration, RAList, SearchFixture, samples, 2 * iterations)
{
  uint64 sum = 0;
  for (auto i: randomAccessList) {
    sum += i;
  }
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(Iteration, RAListHalf, SearchFixture, samples, 2 * iterations)
{
  uint64 sum = 0;
  for (auto it = randomAccessList.begin(); it != randomAccessList.end(); it += 2) {
    sum += *it;
  }
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(Iteration, RAListTenth, SearchFixture, samples, 2 * iterations)
{
  uint64 sum = 0;
  for (auto it = randomAccessList.begin(); it != randomAccessList.end(); it += 10) {
    sum += *it;
  }
  celero::DoNotOptimizeAway(sum);
}
