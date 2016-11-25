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
  BOOST_CHECK(is_iterator<counting_iterator<uint32>>::value);

  int table[4];
  BOOST_CHECK(!is_iterator<decltype(table)>::value);
}

BOOST_AUTO_TEST_CASE(is_iterable_test) {
  BOOST_CHECK(is_iterable<std::vector<int>>::value);

  int table[4];
  BOOST_CHECK(is_iterable<decltype(table)>::value);
  BOOST_CHECK(!is_iterable<int*>::value);

  {
    using iterator = counting_iterator<uint32>;
    BOOST_CHECK(is_iterable<iterator_range<iterator>>::value);
  }

  {
    using iterator = std::vector<uint32>::const_iterator;
    BOOST_CHECK(is_iterable<iterator_range<iterator>>::value);
  }

  {
    using iterator = uint32*;
    BOOST_CHECK(is_iterable<iterator_range<iterator>>::value);
  }
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

BOOST_AUTO_TEST_CASE(iterator_range_test) {
  {
    std::vector<uint32> v = {1, 2, 3, 4};

    {
      auto range = make_range(v.begin(), v.end());
      BOOST_CHECK_EQUAL(range.size(), v.size());
      BOOST_CHECK_EQUAL(range.empty(), v.empty());
    }

    {
      auto range = make_range(v.begin(), v.begin());
      BOOST_CHECK_EQUAL(range.size(), 0);
      BOOST_CHECK_EQUAL(range.empty(), true);
    }
  }

  {
    auto r = range(0, 100);
    BOOST_CHECK_EQUAL(r.size(), 100);
    BOOST_CHECK_EQUAL(r.empty(), false);
  }

  {
    auto r = range(0, 0);
    BOOST_CHECK_EQUAL(r.size(), 0);
    BOOST_CHECK_EQUAL(r.empty(), true);
  }

  {
    std::list<uint32> v = {1, 2, 3, 4};

    {
      auto range = make_range(v.begin(), v.end());
      BOOST_CHECK_EQUAL(range.size(), v.size());
      BOOST_CHECK_EQUAL(range.empty(), v.empty());
    }

    {
      auto range = make_range(v.begin(), v.begin());
      BOOST_CHECK_EQUAL(range.size(), 0);
      BOOST_CHECK_EQUAL(range.empty(), true);
    }
  }
}

BOOST_AUTO_TEST_CASE(mapping_iterator_test) {
  std::vector<uint32> values = {1, 2, 3};

  using iterator = mapping_iterator<std::vector<uint32>::iterator, std::function<uint32(uint32)>>;

  auto begin = iterator(values.begin(), [](uint32 n) { return 2 * n; });
  auto end = iterator(values.end());
  std::vector<uint32> result(begin, end);
  std::vector<uint32> expected = {2, 4, 6};
  BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(indirect_iterator_test) {
  std::vector<std::string> values = {"Ala", "ma", "kota"};
  std::vector<int> indexes = {2, 2, 0, 1};
  auto it = make_indirect_iterator(values.begin(), indexes.begin());
  auto end = make_indirect_iterator(values.begin(), indexes.end());
  std::vector<std::string> result(it, end);
  std::vector<std::string> expected = {"kota", "kota", "Ala", "ma"};
  BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_SUITE_END()
