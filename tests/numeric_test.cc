// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "iterators.h"
#include "numeric.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(numeric_test)

BOOST_AUTO_TEST_CASE(divides_test) {
  BOOST_CHECK(divides(1, 2));
  BOOST_CHECK(divides(2, 2));
  BOOST_CHECK(!divides(3, 2));
  BOOST_CHECK(!divides(3, 1));
}

BOOST_AUTO_TEST_CASE(ceiling_divide_test) {
  BOOST_CHECK_EQUAL(ceiling_divide(1, 2), 1);
  BOOST_CHECK_EQUAL(ceiling_divide(0, 2), 0);
  BOOST_CHECK_EQUAL(ceiling_divide(99, 100), 1);
  BOOST_CHECK_EQUAL(ceiling_divide(50, 100), 1);
  BOOST_CHECK_EQUAL(ceiling_divide(0, 100), 0);
  BOOST_CHECK_EQUAL(ceiling_divide(-1, 100), 0);
}

BOOST_AUTO_TEST_CASE(pow_test) {
  BOOST_CHECK_EQUAL(power(1, 1000000), 1);
  BOOST_CHECK_EQUAL(power(-1, 1000 * 1000 * 1000 + 1), -1);
  BOOST_CHECK_EQUAL(power(101, 0), 1);
  BOOST_CHECK_EQUAL(power(2, 10), 1024);
}

BOOST_AUTO_TEST_CASE(signum_test) {
  BOOST_CHECK_EQUAL(signum(0), 0);
  BOOST_CHECK_EQUAL(signum(-100), -1);
  BOOST_CHECK_EQUAL(signum(1000), 1);
  BOOST_CHECK_EQUAL(signum(std::numeric_limits<int>::lowest()), -1);
  BOOST_CHECK_EQUAL(signum(std::numeric_limits<int64>::lowest()), -1);
  BOOST_CHECK_EQUAL(signum(std::numeric_limits<int>::max()), 1);
  BOOST_CHECK_EQUAL(signum(std::numeric_limits<int64>::max()), 1);
}

BOOST_AUTO_TEST_CASE(pop_count_test) {
  BOOST_CHECK_EQUAL(pop_count(0), 0);
  BOOST_CHECK_EQUAL(pop_count(1), 1);
  BOOST_CHECK_EQUAL(pop_count(0xFFFFFFFF), 32);
  BOOST_CHECK_EQUAL(pop_count(7), 3);
  BOOST_CHECK_EQUAL(pop_count(0xFFFFFFFFFFFFFFFFuLL), 64);
  BOOST_CHECK_EQUAL(pop_count(0xFFFFFFFFFFFFFFuLL), 56);
}

BOOST_AUTO_TEST_CASE(least_significant_one_test) {
  BOOST_CHECK_EQUAL(least_significant_one(1), 0);
  BOOST_CHECK_EQUAL(least_significant_one(2), 1);
  BOOST_CHECK_EQUAL(least_significant_one(0xFFFFFFFF), 0);
  BOOST_CHECK_EQUAL(least_significant_one(1uLL << 33), 33);
  BOOST_CHECK_EQUAL(least_significant_one(1uLL << 63), 63);
  BOOST_CHECK_EQUAL(least_significant_one(47 * 64), 6);
  BOOST_CHECK_EQUAL(least_significant_one((1uLL << 42) * 11111), 42);
}

BOOST_AUTO_TEST_CASE(most_significant_one_test) {
  BOOST_CHECK_EQUAL(most_significant_one(1), 0);
  BOOST_CHECK_EQUAL(most_significant_one(0xFFFFFFFF), 31);
  BOOST_CHECK_EQUAL(most_significant_one(7), 2);
  BOOST_CHECK_EQUAL(most_significant_one(8), 3);
  BOOST_CHECK_EQUAL(most_significant_one(0xFFFFFFFFFFFFFFFFuLL), 63);
  BOOST_CHECK_EQUAL(most_significant_one(0xFFFFFFFFFFFFFFuLL), 55);
}

BOOST_AUTO_TEST_CASE(square_test) {
  BOOST_CHECK_EQUAL(SquareCeiling(0), 0);
  BOOST_CHECK_EQUAL(SquareFloor(0), 0);
  BOOST_CHECK_EQUAL(SquareCeiling(1), 1);
  BOOST_CHECK_EQUAL(SquareFloor(1), 1);
  BOOST_CHECK_EQUAL(SquareCeiling(2), 2);
  BOOST_CHECK_EQUAL(SquareFloor(2), 1);
  BOOST_CHECK_EQUAL(SquareCeiling(4), 2);
  BOOST_CHECK_EQUAL(SquareFloor(4), 2);

  BOOST_CHECK_EQUAL(SquareCeiling(1000 * 1000), 1000);
  BOOST_CHECK_EQUAL(SquareFloor(1000 * 1000), 1000);

  BOOST_CHECK_EQUAL(SquareCeiling(1000 * 1000 + 1), 1001);
  BOOST_CHECK_EQUAL(SquareFloor(1000 * 1000 + 1), 1000);

  lib::uint64 billion = 1000uLL * 1000uLL * 1000uLL;
  BOOST_CHECK_EQUAL(SquareCeiling(billion * billion), billion);
  BOOST_CHECK_EQUAL(SquareFloor(billion * billion), billion);

  BOOST_CHECK_EQUAL(SquareCeiling(billion * billion + 1), billion + 1);
  BOOST_CHECK_EQUAL(SquareFloor(billion * billion + 1), billion);
}

BOOST_AUTO_TEST_CASE(prefix_sum_test) {
  {
    std::vector<uint32> values = {};
    std::vector<uint32> expected = {0};
    BOOST_CHECK(PrefixSums<uint32>(values.begin(), values.end()) == expected);
  }

  {
    std::vector<uint32> values = {1, 2, 3};
    std::vector<uint32> expected = {0, 1, 3, 6};
    BOOST_CHECK(PrefixSums<uint32>(values.begin(), values.end()) == expected);
  }

  {
    std::vector<int32> values = {-1, 2, -1};
    std::vector<int32> expected = {0, -1, 1, 0};
    BOOST_CHECK(PrefixSums<int32>(values.begin(), values.end()) == expected);
  }
}

BOOST_AUTO_TEST_CASE(maximum_test) {
  {
    std::vector<uint32> values = {1, 3, 8, 7, 0, 8};
    auto it = Maximum(counting_iterator<uint32>(0),
                      counting_iterator<uint32>(values.size()),
                      [&values](uint32 index) {
                        return values[index];
                      }
    );
    BOOST_CHECK_EQUAL(*it.first, 2);
  }

  {
    std::vector<uint32> values = {1, 0, 8, 7, 0, 8};
    auto it = Maximum(counting_iterator<uint32>(0),
                      counting_iterator<uint32>(values.size()),
                      [&values](uint32 index) {
                        return values[index];
                      },
                      std::greater<uint32>()
    );
    BOOST_CHECK_EQUAL(*it.first, 1);
  }

  {
    uint32 count = 0;
    auto it = Maximum(counting_iterator<uint32>(0),
                      counting_iterator<uint32>(100),
                      [&count](uint32 value) {
                        count++;
                        return value;
                      }
    );
    BOOST_CHECK_EQUAL(*it.first, 99);
    BOOST_CHECK_EQUAL(count, 100);
  }
}


BOOST_AUTO_TEST_SUITE_END()
