// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/van_emde_boas_set.h"
#include "io.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(van_emde_boas_suite)

constexpr uint32 kTreeSize = 21;

BOOST_AUTO_TEST_CASE(creation_test) {
  VanEmdeBoasSet<kTreeSize> tree;
  BOOST_CHECK_EQUAL(tree.empty(), true);
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(one_insert_test) {
  VanEmdeBoasSet<kTreeSize> tree;
  tree.insert(1);

  BOOST_CHECK_EQUAL(tree.empty(), false);
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK(tree.begin() != tree.end());
}

BOOST_AUTO_TEST_CASE(full_tree_test) {
  VanEmdeBoasSet<kTreeSize> tree;

  constexpr uint32 elements = (1u << kTreeSize) - 1;
  for (auto i: range<uint32>(0, elements))
    tree.insert(i);

  BOOST_CHECK_EQUAL(tree.empty(), false);
  BOOST_CHECK_EQUAL(tree.size(), elements);
  BOOST_CHECK(tree.begin() != tree.end());
  BOOST_CHECK(tree.rbegin() != tree.rend());
}

BOOST_AUTO_TEST_CASE(iterator_test) {
  VanEmdeBoasSet<kTreeSize> tree;

  for (auto h: range<uint32>(0, 25)) {
    tree.insert(h);

    uint32 position = 0;
    for (auto i = tree.begin(); i != tree.end(); i++, position++) {
      uint32 k = 0;
      for (auto j = tree.begin(); j != i; j++) {
        k++;
      }
      BOOST_CHECK_EQUAL(k, position);
    }

    position = 0;
    for (auto i = tree.rbegin(); i != tree.rend(); i++, position++) {
      uint32 k = 0;
      for (auto j = tree.rbegin(); j != i; j++) {
        k++;
      }
      BOOST_CHECK_EQUAL(k, position);
    }
  }
}

BOOST_AUTO_TEST_CASE(correction_test) {
  uint32 N = (1u << kTreeSize) - 1;
  VanEmdeBoasSet<kTreeSize> tree;
  std::set<uint32> set;

  for (auto i: range<uint32>(0, 100000)) {
    uint32 K = Random32() % 100;
    uint32 n = Random32() % N;

    BOOST_CHECK_EQUAL(set.empty(), tree.empty());
    BOOST_CHECK_EQUAL(set.size(), tree.size());

    if (K == 0 || set.empty()) {
      bool a = set.insert(n).second;
      bool b = tree.insert(n);

      BOOST_CHECK_EQUAL(a, b);
    }
    else if (0 < K && K <= 10) {
      bool a = (set.count(n) == 1);
      bool b = tree.find(n);

      BOOST_CHECK_EQUAL(a, b);
    }
    else if (10 < K && K <= 40) {
      bool a = (set.erase(n) > 0);
      bool b = tree.erase(n);
      BOOST_CHECK_EQUAL(a, b);
    }
    else if (K < 40 && K <= 70) {
      uint32 a = (set.upper_bound(n) == set.end()) ? n : (*set.upper_bound(n));
      uint32 b = (tree.last() <= n) ? n : tree.successor(n);
      BOOST_CHECK_EQUAL(a, b);
    }
    else {
      uint32 a = (set.lower_bound(n) == set.begin()) ? n : *(--set.lower_bound(n));
      uint32 b = (tree.first() >= n) ? n : tree.predecessor(n);
      BOOST_CHECK_EQUAL(a, b);
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
