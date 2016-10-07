// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "data_structures/indexer.h"
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
        {1000, 0},
        {100 * 1000, 0}
    };
  }

  void setUp(int64_t experimentValue) override
  {
    using lib::random;
    std::vector<std::string> strings;
    std::string string;
    for (auto i: range(int64_t(0), experimentValue)) {
      for (auto j: range(0, 6)) {
        string += char('a' + random() % 20);
      }
      strings.push_back(string);
      string.clear();
    }

    for (auto i: range(int64_t(0), experimentValue)) {
      queries.push_back(strings[random() % experimentValue]);
    }
  }

  std::vector<std::string> queries;
};


BASELINE_F(Indexer, Indexer, QueriesFixture, samples, iterations)
{
  Indexer<std::string> indexer;
  for (const auto& string: queries) {
    indexer.get_id(string);
  }

  for (auto i: range(size_t(0), indexer.size())) {
    indexer.get_value(i);
  }
}

