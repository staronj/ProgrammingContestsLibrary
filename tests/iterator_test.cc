// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "iterators.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(iterator)

BOOST_AUTO_TEST_CASE(is_iterator_test) {
  BOOST_CHECK(is_iterator<const char*>::value);
  BOOST_CHECK(is_iterator<int*>::value);
  BOOST_CHECK(is_iterator<counting_iterator<int>>::value);
  BOOST_CHECK(is_iterator<std::vector<int>::const_iterator>::value);
  BOOST_CHECK(is_iterator<std::istream_iterator<int>>::value);

  int table[4];
  BOOST_CHECK(!is_iterator<decltype(table)>::value);
}

BOOST_AUTO_TEST_CASE(is_iterable_test) {
  BOOST_CHECK(is_iterable<std::vector<int>>::value);

  int table[4];
  BOOST_CHECK(is_iterable<decltype(table)>::value);
  BOOST_CHECK(!is_iterable<int*>::value);
}

BOOST_AUTO_TEST_CASE(counting_iterator_test) {
  BOOST_CHECK_EQUAL(make_counting_iterator(0) == make_counting_iterator(0), true);
  BOOST_CHECK_EQUAL(make_counting_iterator(0) == make_counting_iterator(1), false);
  BOOST_CHECK_EQUAL(make_counting_iterator(0) < make_counting_iterator(0), false);
  BOOST_CHECK_EQUAL(make_counting_iterator(0) < make_counting_iterator(1), true);

  auto it = make_counting_iterator(1);
  BOOST_CHECK(*it == 1);
  BOOST_CHECK(*++it == 2);
  BOOST_CHECK(*it == 2);
  BOOST_CHECK(*it++ == 2);
  BOOST_CHECK(*it == 3);
  BOOST_CHECK(it[3] == 6);
  BOOST_CHECK(it[-3] == 0);
  BOOST_CHECK(*it-- == 3);
  BOOST_CHECK(*it == 2);
  BOOST_CHECK(*--it == 1);
  BOOST_CHECK_EQUAL(make_counting_iterator(10) - make_counting_iterator(9), 1);
  BOOST_CHECK_EQUAL(*(it + 10), 11);


  BOOST_CHECK_EQUAL(make_counting_iterator(5u) - make_counting_iterator(10u), -5);
  auto it2 = make_counting_iterator(1u);

  BOOST_CHECK_EQUAL(*(it2 - 1), 0u);
  BOOST_CHECK_EQUAL(*(it2 - (-100)), 101u);

  std::vector<int> V(make_counting_iterator(0), make_counting_iterator(4));
  std::vector<int> expected = {0, 1, 2, 3};
  BOOST_CHECK(V == expected);
}

BOOST_AUTO_TEST_CASE(reverse_counting_iterator_test) {
  BOOST_CHECK_EQUAL(make_reverse_counting_iterator(0) < make_reverse_counting_iterator(0), false);
  BOOST_CHECK_EQUAL(make_reverse_counting_iterator(1) < make_reverse_counting_iterator(0), true);

  auto it = make_reverse_counting_iterator(10);
  BOOST_CHECK(*it == 10);
  BOOST_CHECK(*++it == 9);
  BOOST_CHECK(*it == 9);
  BOOST_CHECK(*it++ == 9);
  BOOST_CHECK(*it == 8);
  BOOST_CHECK(it[3] == 5);
  BOOST_CHECK(it[-3] == 11);

  std::vector<int> V(make_reverse_counting_iterator(4), make_reverse_counting_iterator(0));
  std::vector<int> expected = {4, 3, 2, 1};
  BOOST_CHECK(V == expected);
}

BOOST_AUTO_TEST_CASE(range_test) {
  {
    auto r = range(0, 4);
    std::vector<int> V(r.begin(), r.end());
    std::vector<int> expected = {0, 1, 2, 3};
    BOOST_CHECK(V == expected);
  }

  {
    auto r = rrange(0, 4);
    std::vector<int> V(r.begin(), r.end());
    std::vector<int> expected = {3, 2, 1, 0};
    BOOST_CHECK(V == expected);
  }
}

BOOST_AUTO_TEST_SUITE_END()
