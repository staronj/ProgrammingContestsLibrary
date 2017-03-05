// Jakub Staroń, 2017

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "generators.h"
#include "io.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(generator_test_suite)

struct DummyGenerator: Generator<int> {
  Maybe<int> next() final {
    return (n < 3)? Just(n++) : Nothing;
  }

  int n = 0;
};

BOOST_AUTO_TEST_CASE(generator_test) {
  {
    generator_handle<Generator<int>> gen(new DummyGenerator());
    auto gen_range = iterate(gen);
    BOOST_CHECK(is_iterable<decltype(gen_range)>::value);
    for (auto i: gen_range) { }
  }

  {
    generator_handle<Generator<int>> gen(new DummyGenerator());
    std::ostringstream str;
    print(str, "%0", iterate(gen));
    BOOST_CHECK_EQUAL(str.str(), "0 1 2\n");
  }
}

BOOST_AUTO_TEST_CASE(generate_test) {
  {
    auto generator = generate([]() -> Maybe<int> {
      return Nothing;
    });
    BOOST_CHECK(generator->next() == Nothing);

    auto range = iterate(generator);
    BOOST_CHECK(range.begin() == range.end());
  }

  {
    int n = 3;
    auto generator = generate([&n]() -> Maybe<int> {
      return (n > 0)? Just(n--) : Nothing;
    });

    BOOST_CHECK(generator->next() == Just(3));
    BOOST_CHECK(generator->next() == Just(2));
    BOOST_CHECK(generator->next() == Just(1));
    BOOST_CHECK(generator->next() == Nothing);
  }

  {
    std::vector<int> v;
    auto generator = generate(v.begin(), v.end());

    BOOST_CHECK(generator->next() == Nothing);
  }

  {
    std::vector<int> v = {1, 2, 3};
    auto generator = generate(v.begin(), v.end());

    BOOST_CHECK(generator->next() == Just(1));
    BOOST_CHECK(generator->next() == Just(2));
    BOOST_CHECK(generator->next() == Just(3));
    BOOST_CHECK(generator->next() == Nothing);
  }

  {
    std::vector<int> v;
    auto generator = generate(v);

    BOOST_CHECK(generator->next() == Nothing);
  }

  {
    std::vector<int> v = {1, 2, 3};
    auto generator = generate(v);

    BOOST_CHECK(generator->next() == Just(1));
    BOOST_CHECK(generator->next() == Just(2));
    BOOST_CHECK(generator->next() == Just(3));
    BOOST_CHECK(generator->next() == Nothing);
  }
}

BOOST_AUTO_TEST_CASE(iterate_test) {
  {
    std::vector<int> v;
    auto generator = generate(v);
    auto range = iterate(generator);
    std::vector<int> result(range.begin(), range.end());
    BOOST_CHECK(v == result);
  }

  {
    std::vector<int> v = {1, 2, 3};
    auto generator = generate(v);
    auto range = iterate(generator);
    std::vector<int> result(range.begin(), range.end());
    BOOST_CHECK(v == result);
  }

  {
    std::vector<bool> v;
    auto generator = generate(v);
    auto range = iterate(generator);
    std::vector<bool> result(range.begin(), range.end());
    BOOST_CHECK(v == result);
  }

  {
    std::vector<bool> v = {true, false, true};
    auto generator = generate(v);
    auto range = iterate(generator);
    std::vector<bool> result(range.begin(), range.end());
    BOOST_CHECK(v == result);
  }
}

BOOST_AUTO_TEST_CASE(iterate_callback_test) {
  {
    std::vector<int> v;
    auto generator = generate(v);
    std::vector<int> result;
    iterate(generator, [&result](int i) {
      result.push_back(i);
    });
    BOOST_CHECK(v == result);
  }

  {
    std::vector<int> v = {1, 2, 3};
    auto generator = generate(v);
    std::vector<int> result;
    iterate(generator, [&result](int i) {
      result.push_back(i);
    });
    BOOST_CHECK(v == result);
  }

  {
    std::vector<bool> v;
    auto generator = generate(v);
    std::vector<bool> result;
    iterate(generator, [&result](bool b) {
      result.push_back(b);
    });
    BOOST_CHECK(v == result);
  }

  {
    std::vector<bool> v = {true, false, true};
    auto generator = generate(v);
    std::vector<bool> result;
    iterate(generator, [&result](bool b) {
      result.push_back(b);
    });
    BOOST_CHECK(v == result);
  }
}

BOOST_AUTO_TEST_CASE(as_sth_test) {
  {
    std::vector<int> v;
    auto generator = generate(v);
    auto result = as_vector(generator);
    BOOST_CHECK(v == result);
  }

  {
    std::vector<int> v = {1, 2, 3};
    auto generator = generate(v);
    auto result = as_vector(generator);
    BOOST_CHECK(v == result);
  }

  {
    std::vector<bool> v;
    auto generator = generate(v);
    auto result = as_vector(generator);
    BOOST_CHECK(v == result);
  }

  {
    std::vector<bool> v = {true, false, true};
    auto generator = generate(v);
    auto result = as_vector(generator);
    BOOST_CHECK(v == result);
  }

  {
    std::list<int> l;
    auto generator = generate(l);
    auto result = as_list(generator);
    BOOST_CHECK(l == result);
  }

  {
    std::list<int> l = {1, 2, 3};
    auto generator = generate(l);
    auto result = as_list(generator);
    BOOST_CHECK(l == result);
  }

  {
    std::set<int> s;
    auto generator = generate(s);
    auto result = as_set(generator);
    BOOST_CHECK(s == result);
  }

  {
    std::set<int> s = {1, 2, 3};
    auto generator = generate(s);
    auto result = as_set(generator);
    BOOST_CHECK(s == result);
  }
}

BOOST_AUTO_TEST_CASE(filter_test) {
  auto isEven = [](int i) { return i % 2 == 0; };
  {
    std::vector<int> v;
    auto generator = generate(v);
    auto filtered = filter(generator, isEven);
    auto result = as_vector(filtered);
    BOOST_CHECK(v == result);
  }

  {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
    std::vector<int> expected = {2, 4, 6};
    auto generator = generate(v);
    auto filtered = filter(generator, isEven);
    auto result = as_vector(filtered);
    BOOST_CHECK(expected == result);
  }

  {
    std::vector<bool> v = {true, true, false, false, true};
    std::vector<bool> expected = {true, true, true};
    auto generator = generate(v);
    auto filtered = filter(generator, [](bool b) { return b; });
    auto result = as_vector(filtered);
    BOOST_CHECK(expected == result);
  }
}

BOOST_AUTO_TEST_CASE(map_test) {
  auto addOne = [](int i) { return i + 1; };
  {
    std::vector<int> v;
    auto generator = generate(v);
    auto mapped = map(generator, addOne);
    auto result = as_vector(mapped);
    BOOST_CHECK(v == result);
  }

  {
    std::vector<int> v = {1, 2, 3, 4};
    std::vector<int> expected = {2, 3, 4, 5};
    auto generator = generate(v);
    auto mapped = map(generator, addOne);
    auto result = as_vector(mapped);
    BOOST_CHECK(expected == result);
  }

  {
    std::vector<bool> v = {true, true, false, false, true};
    std::vector<bool> expected = {false, false, true, true, false};
    auto generator = generate(v);
    auto mapped = map(generator, [](bool b) { return !b; });
    auto result = as_vector(mapped);
    BOOST_CHECK(expected == result);
  }
}


BOOST_AUTO_TEST_SUITE_END()
