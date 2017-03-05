// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "generators.h"
#include "numeric.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 100;
constexpr size_t iterations = 1;

BASELINE(IterateRange, Generator, samples, iterations)
{
  std::vector<uint64> v(counting_iterator<uint64>(0), counting_iterator<uint64>(Million));
  auto generator = generate(v);
  //generator = map(generator, [](uint64 i) { return i ^ (i + i); });
  uint64 sum = 0;
  for (auto i: iterate(generator)) {
    //sum += i;
    sum += i ^ (i + i);
  }
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK(IterateRange, Range, samples, iterations)
{
  std::vector<uint64> v(counting_iterator<uint64>(0), counting_iterator<uint64>(Million));
  uint64 sum = 0;
  for (auto i: v) {
    sum += i ^ (i + i);
  }
  celero::DoNotOptimizeAway(sum);
}
