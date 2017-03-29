// Jakub Staroń, 2016-2017

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/huller.h"
#include "numeric.h"


using namespace pcl;

BOOST_AUTO_TEST_SUITE(huller_test)

BOOST_AUTO_TEST_CASE(empty_test) {
  Huller<int> huller;
  BOOST_CHECK_EQUAL(huller.empty(), true);
}

BOOST_AUTO_TEST_CASE(insert_one) {
  {
    Huller<int> huller;
    huller.insert(1, 0);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), -1);
  }

  {
    Huller<int> huller;
    huller.insert(0, 1);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 1);
  }
}

BOOST_AUTO_TEST_CASE(insert_two) {
  {
    Huller<int> huller;
    huller.insert(1, 0);
    huller.insert(-1, 0);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 1);
  }

  {
    Huller<int> huller;
    huller.insert(1, -1);
    huller.insert(-1, 1);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 3);
  }

  {
    Huller<int> huller;
    huller.insert(0, 0);
    huller.insert(1, 1);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 3);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 0);
  }

  {
    Huller<int> huller;
    huller.insert(0, 0);
    huller.insert(0, 1);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 1);
  }

  {
    Huller<int> huller;
    huller.insert(0, 1);
    huller.insert(0, 0);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 1);
  }
}

BOOST_AUTO_TEST_CASE(insert_three) {
  {
    Huller<int> huller;
    huller.insert(1, 0);
    huller.insert(-1, 0);
    huller.insert(0, -1); // Should not be inserted
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(3), 3);
    BOOST_CHECK_EQUAL(huller.evaluate(-3), 3);
  }

  {
    Huller<int> huller;
    huller.insert(1, 0);
    huller.insert(0, -1);
    huller.insert(-1, 0); // Should remove (0, -1)
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(3), 3);
    BOOST_CHECK_EQUAL(huller.evaluate(-3), 3);
  }

  {
    Huller<int> huller;
    huller.insert(0, -1);
    huller.insert(-1, 0);
    huller.insert(1, 0); // Should remove (0, -1)
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(3), 3);
    BOOST_CHECK_EQUAL(huller.evaluate(-3), 3);
  }

  {
    Huller<int> huller;
    huller.insert(1, 0);
    huller.insert(-1, 0);
    huller.insert(0, 1);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(3), 3);
    BOOST_CHECK_EQUAL(huller.evaluate(-3), 3);
  }
}

BOOST_AUTO_TEST_CASE(insert_four) {
  {
    Huller<int> huller;
    huller.insert(-100, -1000);
    huller.insert(1, 0);
    huller.insert(-1, 0);
    huller.insert(0, 1);

    BOOST_CHECK_EQUAL(huller.evaluate(0), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 2);
    BOOST_CHECK_EQUAL(huller.evaluate(3), 3);
    BOOST_CHECK_EQUAL(huller.evaluate(-3), 3);

    BOOST_CHECK_EQUAL(huller.evaluate(-20), 1000);
  }
}

template <typename ValueType, typename Comparator = std::less<ValueType>>
class NaiveHuller {
public:
  using value_type = ValueType;
  using function_type = std::pair<value_type, value_type>;
  using comparator_type = Comparator;
  using size_type = int32;
  using difference_type = int32;

  NaiveHuller(comparator_type comparator = comparator_type()):
    comparator_(comparator) { }

  void insert(function_type function) {
    set_.push_back(function);
  }

  void insert(value_type a, value_type b) {
    insert(std::make_pair(std::move(a), std::move(b)));
  }

  value_type evaluate(const value_type& x) const {
    return evaluateOn(x, find(x));
  }

  const function_type& find(const value_type& x) const {
    assert(!empty());
    return *Maximum(set_.begin(), set_.end(), [&x](const function_type& fun) {
      return evaluateOn(x, fun);
    }, comparator_).first;
  }

  bool empty() const {
    return set_.empty();
  }

private:
  static value_type evaluateOn(const value_type& x, const function_type& function) {
    return function.first * x + function.second;
  }

  std::vector<function_type> set_;
  comparator_type comparator_;
};

BOOST_AUTO_TEST_CASE(deterministic_test1) {
  Huller<int> huller;
  NaiveHuller<int> naiveHuller;

  for (auto i : range<int>(0, 100)) {
    huller.insert(i, 0);
    naiveHuller.insert(i, 0);
    for (auto j: range<int>(-100, 100)) {
      BOOST_CHECK_EQUAL(huller.evaluate(j), naiveHuller.evaluate(j));
    }
  }
}

BOOST_AUTO_TEST_CASE(deterministic_test2) {
  Huller<int> huller;
  NaiveHuller<int> naiveHuller;

  for (auto i : range<int>(0, 100)) {
    huller.insert(i, -i);
    naiveHuller.insert(i, -i);
    for (auto j: range<int>(-100, 300)) {
      BOOST_CHECK_EQUAL(huller.evaluate(j), naiveHuller.evaluate(j));
    }
  }
}

BOOST_AUTO_TEST_CASE(random_test1) {
  auto Random = []() {
    return Random32(21) - 10;
  };

  for (auto n: range<int>(0, 1000)) {
    Huller<int> huller;
    NaiveHuller<int> naiveHuller;

    for (auto i : range<int>(0, 10)) {
      auto a = Random();
      auto b = Random();
      huller.insert(a, b);
      naiveHuller.insert(a, b);
      for (auto j: range<int>(-100, 100)) {
        BOOST_CHECK_EQUAL(huller.evaluate(j), naiveHuller.evaluate(j));
      }
    }
  }
}

/// ----------------------------------------- Reversed Part ----------------------------------

BOOST_AUTO_TEST_CASE(insert_two_reversed) {
  {
    Huller<int, std::greater<int>> huller;
    huller.insert(1, 0);
    huller.insert(-1, 0);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), -1);
  }

  {
    Huller<int, std::greater<int>> huller;
    huller.insert(1, -1);
    huller.insert(-1, 1);
    BOOST_CHECK_EQUAL(huller.evaluate(0), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(2), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), -3);
  }

  {
    Huller<int, std::greater<int>> huller;
    huller.insert(0, 0);
    huller.insert(1, 1);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), -1);
  }

  {
    Huller<int, std::greater<int>> huller;
    huller.insert(0, 0);
    huller.insert(0, 1);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 0);
  }

  {
    Huller<int, std::greater<int>> huller;
    huller.insert(0, 1);
    huller.insert(0, 0);
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(2), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), 0);
  }
}

BOOST_AUTO_TEST_CASE(insert_three_reversed) {
  {
    Huller<int, std::greater<int>> huller;
    huller.insert(1, 0);
    huller.insert(-1, 0);
    huller.insert(0, 1); // Should not be inserted
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(3), -3);
    BOOST_CHECK_EQUAL(huller.evaluate(-3), -3);
  }

  {
    Huller<int, std::greater<int>> huller;
    huller.insert(1, 0);
    huller.insert(0, 1);
    huller.insert(-1, 0); // Should remove (0, 1)
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(3), -3);
    BOOST_CHECK_EQUAL(huller.evaluate(-3), -3);
  }

  {
    Huller<int, std::greater<int>> huller;
    huller.insert(0, 1);
    huller.insert(-1, 0);
    huller.insert(1, 0); // Should remove (0, 1)
    BOOST_CHECK_EQUAL(huller.evaluate(0), 0);
    BOOST_CHECK_EQUAL(huller.evaluate(1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(3), -3);
    BOOST_CHECK_EQUAL(huller.evaluate(-3), -3);
  }

  {
    Huller<int, std::greater<int>> huller;
    huller.insert(1, 0);
    huller.insert(-1, 0);
    huller.insert(0, -1);
    BOOST_CHECK_EQUAL(huller.evaluate(0), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(3), -3);
    BOOST_CHECK_EQUAL(huller.evaluate(-3), -3);
  }
}

BOOST_AUTO_TEST_CASE(insert_four_reversed) {
  {
    Huller<int, std::greater<int>> huller;
    huller.insert(100, 1000);
    huller.insert(1, 0);
    huller.insert(-1, 0);
    huller.insert(0, -1);

    BOOST_CHECK_EQUAL(huller.evaluate(0), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(-1), -1);
    BOOST_CHECK_EQUAL(huller.evaluate(2), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(-2), -2);
    BOOST_CHECK_EQUAL(huller.evaluate(3), -3);
    BOOST_CHECK_EQUAL(huller.evaluate(-3), -3);

    BOOST_CHECK_EQUAL(huller.evaluate(-20), -1000);
  }
}

BOOST_AUTO_TEST_CASE(deterministic_test1_reversed) {
  Huller<int, std::greater<int>> huller;
  NaiveHuller<int, std::greater<int>> naiveHuller;

  for (auto i : range<int>(0, 100)) {
    huller.insert(i, 0);
    naiveHuller.insert(i, 0);
    for (auto j: range<int>(-100, 100)) {
      BOOST_CHECK_EQUAL(huller.evaluate(j), naiveHuller.evaluate(j));
    }
  }
}

BOOST_AUTO_TEST_CASE(deterministic_test2_reversed) {
  Huller<int, std::greater<int>> huller;
  NaiveHuller<int, std::greater<int>> naiveHuller;

  for (auto i : range<int>(0, 100)) {
    huller.insert(i, -i);
    naiveHuller.insert(i, -i);
    for (auto j: range<int>(-100, 300)) {
      BOOST_CHECK_EQUAL(huller.evaluate(j), naiveHuller.evaluate(j));
    }
  }
}

BOOST_AUTO_TEST_CASE(random_test1_reversed) {
  auto Random = []() {
    return Random32(21) - 10;
  };

  for (auto n: range<int>(0, 1000)) {
    Huller<int, std::greater<int>> huller;
    NaiveHuller<int, std::greater<int>> naiveHuller;

    for (auto i : range<int>(0, 10)) {
      auto a = Random();
      auto b = Random();
      huller.insert(a, b);
      naiveHuller.insert(a, b);
      for (auto j: range<int>(-100, 100)) {
        BOOST_CHECK_EQUAL(huller.evaluate(j), naiveHuller.evaluate(j));
      }
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
