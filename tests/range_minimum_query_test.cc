// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/range_minimum_query.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(range_minimum_query_test)

BOOST_AUTO_TEST_CASE(test1) {
  std::vector<uint32> values = {1, 3, 8, 4, 3, 5, 9, 0, 4, 2};
  RangeMinimumQuery<uint32> rmq(values.begin(), values.end());

  BOOST_CHECK_EQUAL(values.size(), rmq.size());
  BOOST_CHECK_EQUAL(rmq.minimum(0, 0), 0);
  BOOST_CHECK_EQUAL(rmq.minimum(0, 1), 0);
  BOOST_CHECK_EQUAL(rmq.minimum(0, 2), 0);
  BOOST_CHECK_EQUAL(rmq.minimum(1, 2), 1);
  BOOST_CHECK_EQUAL(rmq.minimum(1, 3), 1);
  BOOST_CHECK_EQUAL(rmq.minimum(0, 3), 0);
  BOOST_CHECK_EQUAL(rmq.minimum(2, 5), 4);
  BOOST_CHECK_EQUAL(rmq.minimum(0, 9), 7);
  BOOST_CHECK_EQUAL(rmq.minimum(8, 9), 9);
}

BOOST_AUTO_TEST_CASE(test2) {
  std::vector<uint32> values = {5, 10, 4, 6, 3, 9, 2, 7};
  RangeMinimumQuery<uint32> rmq(values.begin(), values.end());

  BOOST_CHECK_EQUAL(values.size(), rmq.size());
  BOOST_CHECK_EQUAL(rmq.minimum(0, 7), 6);

  BOOST_CHECK_EQUAL(rmq.minimum(0, 3), 2);
  BOOST_CHECK_EQUAL(rmq.minimum(4, 7), 6);

  BOOST_CHECK_EQUAL(rmq.minimum(0, 1), 0);
  BOOST_CHECK_EQUAL(rmq.minimum(2, 3), 2);
  BOOST_CHECK_EQUAL(rmq.minimum(4, 5), 4);
  BOOST_CHECK_EQUAL(rmq.minimum(6, 7), 6);

  BOOST_CHECK_EQUAL(rmq.minimum(1, 4), 4);
  BOOST_CHECK_EQUAL(rmq.minimum(2, 5), 4);
  BOOST_CHECK_EQUAL(rmq.minimum(3, 6), 6);
}

BOOST_AUTO_TEST_SUITE_END()
