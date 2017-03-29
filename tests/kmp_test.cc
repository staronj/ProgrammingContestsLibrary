// Jakub Staroń, 2016-2017

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "io.h"
#include "text_algorithms/knuth_morris_pratt.h"

using namespace pcl;

BOOST_AUTO_TEST_SUITE(knuth_morris_pratt_test)

BOOST_AUTO_TEST_CASE(knuth_morris_pratt_test) {
  {
    std::string text = "";
    std::vector<uint32> expected = {0};
    KnuthMorrisPratt kmp(text.begin(), text.end());
    BOOST_CHECK(kmp.result() == expected);
  }

  {
    std::string text = "a";
    std::vector<uint32> expected = {0, 0};
    KnuthMorrisPratt kmp(text.begin(), text.end());
    BOOST_CHECK(kmp.result() == expected);
  }

  {
    std::string text = "ab";
    std::vector<uint32> expected = {0, 0, 0};
    KnuthMorrisPratt kmp(text.begin(), text.end());
    BOOST_CHECK(kmp.result() == expected);
  }

  {
    std::string text = "aa";
    std::vector<uint32> expected = {0, 0, 1};
    KnuthMorrisPratt kmp(text.begin(), text.end());
    BOOST_CHECK(kmp.result() == expected);
  }

  {
    std::string text = "ababaaabab";
    std::vector<uint32> expected = {0, 0, 0, 1, 2, 3, 1, 1, 2, 3, 4};
    KnuthMorrisPratt kmp(text.begin(), text.end());
    BOOST_CHECK(kmp.result() == expected);

    const auto& borders = kmp.borders();
    std::vector<uint32> expected_borders = {4, 2};
    BOOST_CHECK(borders == expected_borders);
  }

  {
    std::string text = "aaaaaaaa";
    std::vector<uint32> expected = {0, 0, 1, 2, 3, 4, 5, 6, 7};
    KnuthMorrisPratt kmp(text.begin(), text.end());
    BOOST_CHECK(kmp.result() == expected);

    const auto& borders = kmp.borders();
    std::vector<uint32> expected_borders = {7, 6, 5, 4, 3, 2, 1};
    BOOST_CHECK(borders == expected_borders);
  }
}

BOOST_AUTO_TEST_SUITE_END()
