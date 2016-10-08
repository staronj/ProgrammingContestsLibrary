// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/max_queue.h"


using namespace lib;

BOOST_AUTO_TEST_SUITE(max_queue_test)

BOOST_AUTO_TEST_CASE(empty) {
  MaxQueue<int> queue;
  BOOST_CHECK_EQUAL(queue.size(), 0);
  BOOST_CHECK_EQUAL(queue.empty(), true);
}

BOOST_AUTO_TEST_CASE(push_pop) {
  MaxQueue<int> queue;
  queue.push(10);
  BOOST_CHECK_EQUAL(queue.max(), 10);
  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK_EQUAL(queue.empty(), false);
  queue.push(9);
  BOOST_CHECK_EQUAL(queue.max(), 10);
  queue.pop();
  BOOST_CHECK_EQUAL(queue.max(), 9);
  queue.push(9);
  queue.push(8);
  queue.push(7);
  queue.pop();
  queue.pop();
  BOOST_CHECK_EQUAL(queue.max(), 8);
}

BOOST_AUTO_TEST_CASE(size) {
  MaxQueue<int> queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  queue.push(4);
  queue.push(5);
  queue.push(1);
  BOOST_CHECK_EQUAL(queue.size(), 6);
  queue.pop();
  queue.pop();
  queue.pop();
  queue.pop();
  BOOST_CHECK_EQUAL(queue.max(), 5);
  queue.pop();
  BOOST_CHECK_EQUAL(queue.max(), 1);
  queue.pop();
  BOOST_CHECK_EQUAL(queue.empty(), true);
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
