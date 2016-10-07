// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/find_and_union.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(find_and_union_test)

BOOST_AUTO_TEST_CASE(find_and_union_test) {
  FindAndUnion findAndUnion(10);

  BOOST_CHECK(findAndUnion.find_root(0) != findAndUnion.find_root(1));
  BOOST_CHECK(findAndUnion.find_root(0) == findAndUnion.find_root(0));
  BOOST_CHECK(findAndUnion.union_sets(0, 1));
  BOOST_CHECK(findAndUnion.find_root(0) == findAndUnion.find_root(1));

  BOOST_CHECK(findAndUnion.union_sets(2, 3));
  BOOST_CHECK(findAndUnion.union_sets(4, 5));
  BOOST_CHECK(findAndUnion.union_sets(6, 7));
  BOOST_CHECK(findAndUnion.union_sets(8, 9));
  BOOST_CHECK(findAndUnion.find_root(3) != findAndUnion.find_root(5));
  BOOST_CHECK(findAndUnion.find_root(5) != findAndUnion.find_root(7));
  BOOST_CHECK(findAndUnion.find_root(6) == findAndUnion.find_root(7));

  BOOST_CHECK(findAndUnion.union_sets(3, 5));
  BOOST_CHECK(findAndUnion.union_sets(7, 8));
  BOOST_CHECK(findAndUnion.union_sets(2, 9));

  BOOST_CHECK(findAndUnion.find_root(4) == findAndUnion.find_root(6));
}

BOOST_AUTO_TEST_SUITE_END()
