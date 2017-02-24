// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "iterators.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 100;
constexpr size_t iterations = 100;

class fibonacci_generator: public generator<uint32> {
public:
  fibonacci_generator(uint32 N):
      F1(0), F2(1), I(N) { }

  Maybe<value_type> next() final {
    I--;

    if (I <= 0)
      return Nothing;

    uint64 T = F2;
    F2 = F1 + F2;
    F1 = T;

    return F2;
  }

private:
  uint64 F1;
  uint64 F2;
  int32 I;
};

constexpr uint32 kNumberOfIterations = 100;

BASELINE(IterateRange, Generator, samples, iterations)
{
  uint64 sum = 0;
  for (auto i: iterate_generator(new fibonacci_generator(kNumberOfIterations))) {
    sum += i;
  }
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK(IterateRange, Range, samples, iterations)
{
  uint64 sum = 0;
  uint64 F1 = 0;
  uint64 F2 = 1;
  for (auto i: range<uint32>(0, kNumberOfIterations)) {
    uint64 T = F2;
    F2 = F1 + F2;
    F1 = T;

    sum += F2;
  }
  celero::DoNotOptimizeAway(sum);
}
