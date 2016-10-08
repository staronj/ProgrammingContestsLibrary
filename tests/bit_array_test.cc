// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/bit_array.h"
#include "operators.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(bit_array_test)

BOOST_AUTO_TEST_CASE(empty) {
  BitArray array1;
  BOOST_CHECK_EQUAL(array1.size(), 0);
  BOOST_CHECK_EQUAL(array1.empty(), true);
  BOOST_CHECK_EQUAL(array1.count(), 0);
  BOOST_CHECK_EQUAL(array1.any(), false);
  BOOST_CHECK_EQUAL(array1.none(), true);
  BOOST_CHECK_EQUAL(array1.all(), true);
  array1.flip();
  BOOST_CHECK_EQUAL(array1.size(), 0);
  BOOST_CHECK_EQUAL(array1.empty(), true);
  BOOST_CHECK_EQUAL(array1.count(), 0);
  BOOST_CHECK_EQUAL(array1.any(), false);
  BOOST_CHECK_EQUAL(array1.none(), true);
  BOOST_CHECK_EQUAL(array1.all(), true);
  array1.reset(true);
  BOOST_CHECK_EQUAL(array1.size(), 0);
  BOOST_CHECK_EQUAL(array1.empty(), true);
  BOOST_CHECK_EQUAL(array1.count(), 0);
  BOOST_CHECK_EQUAL(array1.any(), false);
  BOOST_CHECK_EQUAL(array1.none(), true);
  BOOST_CHECK_EQUAL(array1.all(), true);

  BitArray array2(array1);
  BOOST_CHECK_EQUAL(array2.size(), 0);
  BOOST_CHECK_EQUAL(array2.empty(), true);

  BitArray array3(std::move(array2));
  BOOST_CHECK_EQUAL(array2.size(), 0);
  BOOST_CHECK_EQUAL(array2.empty(), true);

  BOOST_CHECK_EQUAL(array3.size(), 0);
  BOOST_CHECK_EQUAL(array3.empty(), true);

  BOOST_CHECK(array1 == array3);
}

BOOST_AUTO_TEST_CASE(small_sizes) {
  BitArray array1(1, false);
  BitArray array2(1, true);
  BitArray array3(2, false);
  BOOST_CHECK(array1 != array2);
  BOOST_CHECK(array1 != array3);
  BOOST_CHECK(array2 != array3);

  BOOST_CHECK_EQUAL(array1.empty(), false);
  BOOST_CHECK_EQUAL(array1.size(), 1);
  BOOST_CHECK_EQUAL(array1.count(), 0);
  BOOST_CHECK_EQUAL(array1.any(), false);
  BOOST_CHECK_EQUAL(array1.none(), true);
  BOOST_CHECK_EQUAL(array1.all(), false);

  BOOST_CHECK_EQUAL(array2.count(), 1);
  BOOST_CHECK_EQUAL(array2.any(), true);
  BOOST_CHECK_EQUAL(array2.none(), false);
  BOOST_CHECK_EQUAL(array2.all(), true);

  array1.flip();
  BOOST_CHECK(array1 == array2);
  array1.set(0, false);
  BOOST_CHECK(array1 != array2);
  array2.set(0, false);
  BOOST_CHECK(array1 == array2);

  BOOST_CHECK_EQUAL(array1.empty(), false);
  BOOST_CHECK_EQUAL(array1.size(), 1);
  BOOST_CHECK_EQUAL(array1.count(), 0);
  BOOST_CHECK_EQUAL(array1.any(), false);
  BOOST_CHECK_EQUAL(array1.none(), true);
  BOOST_CHECK_EQUAL(array1.all(), false);

  BOOST_CHECK_THROW(array1 | array3, std::exception);
  BOOST_CHECK_THROW(array1 & array3, std::exception);
}

BOOST_AUTO_TEST_CASE(medium_sizes) {
  BitArray array(128, false);
  array.set(127, true);
}

BOOST_AUTO_TEST_SUITE_END()
