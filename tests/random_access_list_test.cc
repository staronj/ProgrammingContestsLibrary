// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/random_access_list.h"


using namespace lib;

BOOST_AUTO_TEST_SUITE(random_access_list_test)

BOOST_AUTO_TEST_CASE(empty) {
  RandomAccessList<int> list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK_EQUAL(list.empty(), true);
  BOOST_CHECK(list.begin() == list.end());

  const RandomAccessList<int>& const_list = list;
  BOOST_CHECK(const_list.begin() == const_list.end());

  BOOST_CHECK_EQUAL(is_random_access_iterator<RandomAccessList<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_random_access_iterator<RandomAccessList<int>::const_iterator>::value, true);
}

BOOST_AUTO_TEST_CASE(construction) {
  {
    RandomAccessList<int> list = {};
    BOOST_CHECK_EQUAL(list.size(), 0);
    BOOST_CHECK_EQUAL(list.empty(), true);
    BOOST_CHECK(list.begin() == list.end());
    BOOST_CHECK_EQUAL(list.end() - list.begin(), 0);

    BOOST_CHECK_EQUAL(list.begin() < list.begin(), false);
    BOOST_CHECK_EQUAL(list.end() < list.end(), false);
    BOOST_CHECK_EQUAL(list.end() < list.begin(), false);
    BOOST_CHECK_EQUAL(list.begin() < list.end(), false);
  }

  {
    RandomAccessList<int> list = {1};
    BOOST_CHECK_EQUAL(list.size(), 1);
    BOOST_CHECK_EQUAL(list.empty(), false);
    BOOST_CHECK(list.begin() != list.end());
    BOOST_CHECK_EQUAL(list.end() - list.begin(), 1);
    BOOST_CHECK_EQUAL(*list.begin(), 1);
    BOOST_CHECK_EQUAL(*(--list.end()), 1);
    BOOST_CHECK_EQUAL(list.front(), 1);
    BOOST_CHECK_EQUAL(list.back(), 1);
    BOOST_CHECK(++list.begin() == list.end());

    BOOST_CHECK_EQUAL(list.begin() < list.begin(), false);
    BOOST_CHECK_EQUAL(list.end() < list.end(), false);
    BOOST_CHECK_EQUAL(list.end() < list.begin(), false);
    BOOST_CHECK_EQUAL(list.begin() < list.end(), true);
  }

  {
    RandomAccessList<int> list = {1, 2, 3};
    BOOST_CHECK_EQUAL(list.size(), 3);
    BOOST_CHECK_EQUAL(list.empty(), false);
    BOOST_CHECK(list.begin() != list.end());
    BOOST_CHECK_EQUAL(list.end() - list.begin(), 3);
    BOOST_CHECK_EQUAL(*list.begin(), 1);
    BOOST_CHECK_EQUAL(*(--list.end()), 3);
    BOOST_CHECK_EQUAL(list.front(), 1);
    BOOST_CHECK_EQUAL(list.back(), 3);
  }

  {
    constexpr int size = 10 * 1000;
    RandomAccessList<int> list(counting_iterator<int>(0), counting_iterator<int>(size));
    BOOST_CHECK_EQUAL(list.size(), size);
    BOOST_CHECK_EQUAL(list.empty(), false);
    BOOST_CHECK(list.begin() != list.end());
    BOOST_CHECK_EQUAL(list.end() - list.begin(), size);
    BOOST_CHECK_EQUAL(*list.begin(), 0);
    BOOST_CHECK_EQUAL(*(--list.end()), size - 1);
    BOOST_CHECK_EQUAL(list.front(), 0);
    BOOST_CHECK_EQUAL(list.back(), size - 1);

    auto it = list.begin();
    for (auto i: range<int>(0, size)) {
      BOOST_CHECK_EQUAL(*it, i);
      BOOST_CHECK_EQUAL(list[i], i);
      BOOST_CHECK_EQUAL(*(list.begin() + i), i);
      BOOST_CHECK(it + (size - i) == list.end());
      it++;
    }
    BOOST_CHECK_EQUAL(it == list.end(), true);
  }
}

BOOST_AUTO_TEST_CASE(clear) {
  RandomAccessList<int> list(counting_iterator<int>(0), counting_iterator<int>(1000));
  list.clear();

  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK_EQUAL(list.empty(), true);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK_EQUAL(list.end() - list.begin(), 0);
}

BOOST_AUTO_TEST_CASE(push) {
  RandomAccessList<int> list;
  list.push_back(2);
  list.push_front(1);
  list.push_back(3);
  list.push_front(0);

  BOOST_CHECK_EQUAL(list.size(), 4);
  BOOST_CHECK_EQUAL(list.empty(), false);

  std::vector<int> result(list.begin(), list.end());
  std::vector<int> expected = {0, 1, 2, 3};

  BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(pop) {
  constexpr auto N = 1000;
  constexpr auto size = 3 * N;
  RandomAccessList<int> list(counting_iterator<int>(0), counting_iterator<int>(size));

  for (auto i: range<int>(0, N)) {
    list.pop_front();
    list.pop_back();
  }

  BOOST_CHECK_EQUAL(list.size(), N);
  BOOST_CHECK_EQUAL(list.empty(), false);

  std::vector<int> result(list.begin(), list.end());
  std::vector<int> expected(counting_iterator<int>(N), counting_iterator<int>(2 * N));

  BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(erase) {
  {
    RandomAccessList<int> list = {1};
    BOOST_CHECK(list.erase(list.begin()) == list.end());
  }

  {
    RandomAccessList<int> list = {1, 2};
    BOOST_CHECK(list.erase(list.begin() + 1) == list.end());
  }

  {
    RandomAccessList<int> list = {1, 2};
    auto it = list.begin() + 1;
    BOOST_CHECK(list.erase(list.begin()) == it);
  }

  auto predicate = [](int k) {
    return (k % 3 == 0) != (k % 5 == 0);
  };

  constexpr auto size = 10 * 1000;
  auto numbers = range<int>(0, size);
  RandomAccessList<int> list(numbers.begin(), numbers.end());

  auto it = list.begin();
  for (auto i: numbers) {
    if (predicate(*it))
      it = list.erase(it);
    else
      ++it;
  }

  std::vector<int> result(list.begin(), list.end());
  std::vector<int> expected(numbers.begin(), numbers.end());
  expected.erase(std::remove_if(expected.begin(), expected.end(), predicate), expected.end());

  BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(swap) {
  RandomAccessList<int> list1 = {1, 2, 3};
  RandomAccessList<int> list2 = {1, 2, 3, 4};
  list1.swap(list2);
  std::swap(list1, list2);
  list2.swap(list1);
  BOOST_CHECK_EQUAL(list1.size(), 4);
  BOOST_CHECK_EQUAL(list2.size(), 3);
}

BOOST_AUTO_TEST_CASE(find) {
  auto AlwaysFalse = [](const int&) { return false; };
  auto AlwaysTrue = [](const int&) { return true; };

  {
    RandomAccessList<int> list;
    BOOST_CHECK(list.find(AlwaysFalse) == list.end());
    BOOST_CHECK(list.find(AlwaysTrue) == list.end());
  }

  {
    RandomAccessList<int> list = {1};
    BOOST_CHECK(list.find(AlwaysFalse) == list.end());
    BOOST_CHECK(list.find(AlwaysTrue) == list.begin());
  }

  {
    RandomAccessList<int> list = {1, 2};
    BOOST_CHECK(list.find(AlwaysFalse) == list.end());
    BOOST_CHECK(list.find(AlwaysTrue) == list.begin());

    auto FindTwo = [](const int& i) { return i > 1; };
    BOOST_CHECK(list.find(FindTwo) == list.begin() + 1);
  }

  {
    constexpr auto size = 10 * 1000;
    RandomAccessList<int> list(counting_iterator<int>(0), counting_iterator<int>(size));

    BOOST_CHECK(list.find(AlwaysFalse) == list.end());
    BOOST_CHECK(list.find(AlwaysTrue) == list.begin());

    for (auto i: range<int>(0, size)) {
      auto FindI = [i](const int& j) { return j >= i; };
      BOOST_CHECK(list.find(FindI) == list.begin() + i);
    }
  }
}

BOOST_AUTO_TEST_CASE(index_of) {
  RandomAccessList<int> list;
  BOOST_CHECK_EQUAL(list.index_of(list.begin()), 0);
  BOOST_CHECK_EQUAL(list.index_of(list.end()), 0);

  list = {1, 2, 3};
  BOOST_CHECK_EQUAL(list.index_of(list.begin()), 0);
  BOOST_CHECK_EQUAL(list.index_of(list.begin() + 2), 2);
  BOOST_CHECK_EQUAL(list.index_of(list.end()), 3);

  auto it = list.begin() + 1;
  BOOST_CHECK_EQUAL(list.index_of(it), 1);
  list.push_front(0);
  BOOST_CHECK_EQUAL(list.index_of(it), 2);
}

BOOST_AUTO_TEST_SUITE_END()
