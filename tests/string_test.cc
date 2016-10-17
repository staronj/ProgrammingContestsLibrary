// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "text_algorithms/string.h"
#include "io.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(string)

BOOST_AUTO_TEST_CASE(is_equal_to_test) {
  BOOST_CHECK(text::is_equal_to('a')('a'));
  BOOST_CHECK(!text::is_equal_to('a')('b'));
  BOOST_CHECK(!text::is_equal_to('b')('a'));
}

BOOST_AUTO_TEST_CASE(is_in_set_test) {
  BOOST_CHECK(text::is_in_set("abc")('a'));
  BOOST_CHECK(text::is_in_set("abc")('b'));
  BOOST_CHECK(text::is_in_set("abc")('c'));
  BOOST_CHECK(!text::is_in_set("abc")('d'));
  BOOST_CHECK(!text::is_in_set("")('d'));
  BOOST_CHECK(!text::is_in_set("")('\0'));
}

BOOST_AUTO_TEST_CASE(split_test) {
  {
    std::string text = "Ala ma kota";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ' ');
    std::vector<std::string> expected_result = {"Ala", "ma", "kota"};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = "";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ' ');
    std::vector<std::string> expected_result = {};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = " ";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ' ');
    std::vector<std::string> expected_result = {};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = " Ala   ma kota   \t  ";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ' ');
    std::vector<std::string> expected_result = {"Ala", "ma", "kota", "\t"};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = " Ala a  a";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ' ', true);
    std::vector<std::string> expected_result = {"", "Ala", "a", "", "a"};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = "";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ' ', true);
    std::vector<std::string> expected_result = {""};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = "A;B,C D,,E;,.F G ;.;,;";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ";., ");
    std::vector<std::string> expected_result = {"A", "B", "C", "D", "E", "F", "G"};
    BOOST_CHECK(result == expected_result);
  }
}

BOOST_AUTO_TEST_CASE(join_test) {
  {
    std::vector<std::string> strings = {"Ala", "ma", "kota"};
    BOOST_CHECK_EQUAL(text::join(" ", strings.begin(), strings.end()), "Ala ma kota");
  }

  {
    std::vector<std::string> strings = {};
    BOOST_CHECK_EQUAL(text::join(" ", strings.begin(), strings.end()), "");
  }

  {
    std::vector<std::string> strings = {"Ala"};
    BOOST_CHECK_EQUAL(text::join(" ", strings.begin(), strings.end()), "Ala");
  }
}

BOOST_AUTO_TEST_SUITE_END()
