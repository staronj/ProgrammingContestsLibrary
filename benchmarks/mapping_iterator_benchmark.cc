// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "iterators.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 20;
constexpr size_t iterations = 5;

class SizeFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {1000 * 1000, 0}
    };
  }

  void setUp(int64_t experimentValue) override {
    data.assign(make_counting_iterator<uint32>(0), make_counting_iterator<uint32>(experimentValue));
  }

  std::vector<uint32> data;
};

BASELINE_F(RangeTransform, Lambda, SizeFixture, samples, iterations)
{
  uint64 sum = 0;
  using iterator = mapping_iterator<std::vector<uint32>::iterator, std::function<uint32(uint32)>>;
  auto begin = iterator(data.begin(), [](uint32 n) { return 2 * n; });
  auto end = iterator(data.end());
  for (const auto i: make_range(begin, end))
    sum += i;
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(RangeTransform, Struct, SizeFixture, samples, iterations)
{
  uint64 sum = 0;
  struct Mapper {
    uint32 operator()(uint32 n) const {
      return 2 * n;
    }
  };
  auto begin = mapping_iterator<std::vector<uint32>::iterator, Mapper>(data.begin());
  auto end = mapping_iterator<std::vector<uint32>::iterator, Mapper>(data.end());

  for (const auto i: make_range(begin, end))
    sum += i;
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(RangeTransform, StructOverFunction, SizeFixture, samples, iterations)
{
  uint64 sum = 0;
  struct Mapper {
    uint32 operator()(uint32 n) const {
      return 2 * n;
    }
  };
  Mapper mapper;
  auto begin = mapping_iterator<std::vector<uint32>::iterator, std::function<uint32(uint32)>>(data.begin(), mapper);
  auto end = mapping_iterator<std::vector<uint32>::iterator, std::function<uint32(uint32)>>(data.end());

  for (const auto i: make_range(begin, end))
    sum += i;
  celero::DoNotOptimizeAway(sum);
}
