// Jakub Staroń, 2016-2017

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/max_stack.h"


using namespace pcl;

BOOST_AUTO_TEST_SUITE(max_stack_test)

BOOST_AUTO_TEST_CASE(empty) {
  MaxStack<int> stack;
  BOOST_CHECK_EQUAL(stack.size(), 0);
  BOOST_CHECK_EQUAL(stack.empty(), true);
}

BOOST_AUTO_TEST_CASE(emplace_test) {
  MaxStack<std::pair<int, int>> stack;
  stack.emplace(2, 3);
  stack.emplace(1, 2);
  BOOST_CHECK(stack.max() == std::make_pair(2, 3));
}

BOOST_AUTO_TEST_CASE(push_pop) {
  MaxStack<int> stack;
  stack.push(10);
  BOOST_CHECK_EQUAL(stack.max(), 10);
  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK_EQUAL(stack.empty(), false);
  stack.push(9);
  BOOST_CHECK_EQUAL(stack.max(), 10);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.max(), 10);
  stack.push(11);
  stack.push(12);
  stack.push(13);
  stack.pop();
  stack.pop();
  BOOST_CHECK_EQUAL(stack.max(), 11);
}

BOOST_AUTO_TEST_CASE(size) {
  MaxStack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  stack.push(4);
  stack.push(5);
  stack.push(1);
  BOOST_CHECK_EQUAL(stack.size(), 6);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.max(), 5);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.max(), 4);
  stack.pop();
  stack.pop();
  stack.pop();
  stack.pop();
  BOOST_CHECK_EQUAL(stack.empty(), true);
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(comparator_type_less) {
  struct Comparator {
    bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
      return lhs.first < rhs.first;
    }
  };

  MaxStack<std::pair<int, int>, Comparator> stack;
  stack.push(std::make_pair(1, 0));
  stack.push(std::make_pair(1, 1));
  stack.push(std::make_pair(1, 2));
  BOOST_CHECK_EQUAL(stack.max().second, 0);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.max().second, 0);
  stack.push(std::make_pair(2, 3));
  BOOST_CHECK_EQUAL(stack.max().second, 3);
}

BOOST_AUTO_TEST_CASE(comparator_type_less_or_equal) {
  struct Comparator {
    bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
      return lhs.first <= rhs.first;
    }
  };

  MaxStack<std::pair<int, int>, Comparator> stack;
  stack.push(std::make_pair(1, 0));
  stack.push(std::make_pair(1, 1));
  stack.push(std::make_pair(1, 2));
  BOOST_CHECK_EQUAL(stack.max().second, 2);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.max().second, 1);
  stack.push(std::make_pair(2, 3));
  BOOST_CHECK_EQUAL(stack.max().second, 3);
}

BOOST_AUTO_TEST_CASE(comparator_type_greater) {
  struct Comparator {
    bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
      return lhs.first > rhs.first;
    }
  };

  MaxStack<std::pair<int, int>, Comparator> stack;
  stack.push(std::make_pair(1, 0));
  stack.push(std::make_pair(1, 1));
  stack.push(std::make_pair(1, 2));
  BOOST_CHECK_EQUAL(stack.max().second, 0);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.max().second, 0);
  stack.push(std::make_pair(0, 3));
  BOOST_CHECK_EQUAL(stack.max().second, 3);
}

BOOST_AUTO_TEST_SUITE_END()
