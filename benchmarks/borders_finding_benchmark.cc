// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "iterators.h"
#include "io.h"
#include "text_algorithms/hasher.h"
#include "text_algorithms/knuth_morris_pratt.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 20;
constexpr size_t iterations = 5;

class TextFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {10 * 1000, 0},
        {1000 * 1000, 0}
    };
  }

  void setUp(int64 experimentValue) override
  {
    text.reserve(experimentValue);
    text.clear();
    using lib::random;
    for (auto i: range<uint32>(0, experimentValue))
      text += char('a' + random() % 20);
  }

  std::string text;
};

BASELINE_F(Borders, KMP, TextFixture, samples, iterations)
{
  KnuthMorrisPratt kmp;
  kmp.feed(text.begin(), text.end());
  if (!kmp.borders().empty())
    celero::DoNotOptimizeAway(kmp.borders().front());
}

BENCHMARK_F(Borders, Hasher, TextFixture, samples, iterations)
{
  Hasher hasher;
  hasher.feed(text.begin(), text.end());
  uint32 longest = 0;
  for (auto i: range<uint32>(1, text.size())) {
   if (hasher.get_hash(0, i) == hasher.get_hash(text.size() - i, i)) {
     longest = i;
   }
  }
  celero::DoNotOptimizeAway(longest);
}
