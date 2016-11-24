// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/power_tree.h"


using namespace lib;

BOOST_AUTO_TEST_SUITE(power_tree_suite)

BOOST_AUTO_TEST_CASE(small) {
  PowerTree<int64> tree(1);
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.query(0, 0), 0);
  BOOST_CHECK_EQUAL(tree.prefixQuery(0), 0);

  tree.insert(0, 100);
  tree.insert(0, -200);
  tree.insert(0, 100);
  tree.insert(0, 10);

  BOOST_CHECK_EQUAL(tree.query(0, 0), 10);
  BOOST_CHECK_EQUAL(tree.prefixQuery(0), 10);
}

BOOST_AUTO_TEST_CASE(medium) {
  PowerTree<int64> tree(10);
  BOOST_CHECK_EQUAL(tree.size(), 10);
  BOOST_CHECK_EQUAL(tree.query(0, 9), 0);
  BOOST_CHECK_EQUAL(tree.prefixQuery(9), 0);

  tree.insert(1, 100);
  tree.insert(2, 100);
  tree.insert(3, -200);
  tree.insert(4, 10);
  tree.insert(1, -2);
  tree.insert(0, -1);

  BOOST_CHECK_EQUAL(tree.prefixQuery(0), -1);
  BOOST_CHECK_EQUAL(tree.prefixQuery(1), 97);
  BOOST_CHECK_EQUAL(tree.prefixQuery(2), 197);
  BOOST_CHECK_EQUAL(tree.query(0, 9), 7);
  BOOST_CHECK_EQUAL(tree.query(2, 3), -100);
}

BOOST_AUTO_TEST_SUITE_END()
