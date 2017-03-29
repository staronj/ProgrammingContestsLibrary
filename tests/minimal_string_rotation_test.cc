// Jakub Staroń, 2016-2017

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "io.h"
#include "text_algorithms/minimal_string_rotation.h"

using namespace pcl;

BOOST_AUTO_TEST_SUITE(minimal_string_rotation_test)

BOOST_AUTO_TEST_CASE(minimal_string_rotation_test) {
  {
    std::string text = "";
    std::string expected = "";
    BOOST_CHECK_EQUAL(text::MinimalStringRotation(text), expected);
  }

  {
    std::string text = "a";
    std::string expected = "a";
    BOOST_CHECK_EQUAL(text::MinimalStringRotation(text), expected);
  }

  {
    std::string text = "ba";
    std::string expected = "ab";
    BOOST_CHECK_EQUAL(text::MinimalStringRotation(text), expected);
  }

  {
    std::string text = "cba";
    std::string expected = "acb";
    BOOST_CHECK_EQUAL(text::MinimalStringRotation(text), expected);
  }

  {
    std::string text = "aabaaabaabaca";
    std::string expected = "aaabaaabaabac";
    BOOST_CHECK_EQUAL(text::MinimalStringRotation(text), expected);
  }

  {
    std::string text = "baaabaabacaaa";
    std::string expected = "aaabaaabaabac";
    BOOST_CHECK_EQUAL(text::MinimalStringRotation(text), expected);
  }

  {
    std::string text = "algorithm";
    std::string expected = "algorithm";
    BOOST_CHECK_EQUAL(text::MinimalStringRotation(text), expected);
  }
}

BOOST_AUTO_TEST_SUITE_END()
