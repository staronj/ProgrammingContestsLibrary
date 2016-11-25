// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "io.h"
#include "text_algorithms/palindromes.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(palindromes_test_suite)

BOOST_AUTO_TEST_CASE(palindromes_test) {
  {
    std::string text = "a";
    std::vector<uint32> expected = {1};
    auto result = Palindromes(text.begin(), text.end());
    BOOST_CHECK(result == expected);
  }

  {
    std::string text = "aa";
    std::vector<uint32> expected = {1, 2, 1};
    auto result = Palindromes(text.begin(), text.end());
    BOOST_CHECK(result == expected);
    print("%0, %1", result, expected);
  }

  {
    std::string text = "ab";
    std::vector<uint32> expected = {1, 0, 1};
    auto result = Palindromes(text.begin(), text.end());
    BOOST_CHECK(result == expected);
  }

  {
    std::string text = "aaa";
    std::vector<uint32> expected = {1, 2, 3, 2, 1};
    auto result = Palindromes(text.begin(), text.end());
    BOOST_CHECK(result == expected);
    print("%0, %1", result, expected);
  }

  {
    std::string text = "aba";
    std::vector<uint32> expected = {1, 0, 3, 0, 1};
    auto result = Palindromes(text.begin(), text.end());
    BOOST_CHECK(result == expected);
  }

  {
    std::string text = "aaabba";
    std::vector<uint32> expected = {1, 2, 3, 2, 1, 0, 1, 4, 1, 0, 1};
    auto result = Palindromes(text.begin(), text.end());
    BOOST_CHECK(result == expected);
  }

  {
    std::string text = "abaaba";
    std::vector<uint32> expected = {1, 0, 3, 0, 1, 6, 1, 0, 3, 0, 1};
    auto result = Palindromes(text.begin(), text.end());
    BOOST_CHECK(result == expected);
  }

  {
    std::string text = "aaaaaaabaaa";
    std::vector<uint32> expected = {1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, 7, 0, 1, 2, 3, 2, 1};
    auto result = Palindromes(text.begin(), text.end());
    BOOST_CHECK(result == expected);
  }
}

BOOST_AUTO_TEST_SUITE_END()
