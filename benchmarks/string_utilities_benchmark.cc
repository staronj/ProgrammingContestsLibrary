// Jakub Staroń, 2016-2017
#include <celero/Celero.h>

#include "text_algorithms/string.h"

CELERO_MAIN

using namespace pcl;

constexpr size_t samples = 1000;
constexpr size_t iterations = 10;

class TextsFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {10, 0},
        {100, 0}
    };
  }

  void setUp(int64_t experimentValue) override
  {
    using pcl::Random32;
    std::string string;
    for (auto i: range<uint32>(0, 10)) {
      for (auto j: range<uint32>(0, experimentValue)) {
        string += char('a' + Random32() % 2);
      }
      texts.push_back(string);
      string.clear();
    }
  }

  std::vector<std::string> texts;
};


BASELINE_F(Split, IsEqual, TextsFixture, samples, iterations)
{
  for (const auto& text: texts) {
    auto v = text::split(text.begin(), text.end(), 'b', true);
    celero::DoNotOptimizeAway(v.back().size());
  }
}

BENCHMARK_F(Split, IsInSet, TextsFixture, samples, iterations)
{
  for (const auto& text: texts) {
    auto v = text::split(text.begin(), text.end(), "ccccccccccccccb", true);
    celero::DoNotOptimizeAway(v.back().size());
  }
}

