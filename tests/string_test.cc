// Jakub Staroń, 2016-2017

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "text_algorithms/string.h"
#include "io.h"

using namespace pcl;

auto IsSpace = [](char c) {return std::isspace(c); };

auto IsDigit = [](char c) {return std::isdigit(c); };

BOOST_AUTO_TEST_SUITE(string)

BOOST_AUTO_TEST_CASE(split_test_character) {
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
}

BOOST_AUTO_TEST_CASE(split_test_set) {
  {
    std::string text = "Ala ma kota";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ";, ");
    std::vector<std::string> expected_result = {"Ala", "ma", "kota"};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = "";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ";, ");
    std::vector<std::string> expected_result = {};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = " ";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ";, ");
    std::vector<std::string> expected_result = {};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = " Ala   ma kota   \t  ";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ";, ");
    std::vector<std::string> expected_result = {"Ala", "ma", "kota", "\t"};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = " Ala a  a";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ";, ", true);
    std::vector<std::string> expected_result = {"", "Ala", "a", "", "a"};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = "";
    std::vector<std::string> result = text::split(text.begin(), text.end(), ";, ", true);
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

BOOST_AUTO_TEST_CASE(split_test_predicate) {
  {
    std::string text = "Ala1ma3kota";
    std::vector<std::string> result = text::split(text.begin(), text.end(), IsDigit);
    std::vector<std::string> expected_result = {"Ala", "ma", "kota"};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::string text = "Ala\tma\nkota";
    std::vector<std::string> result = text::split(text.begin(), text.end(), IsSpace);
    std::vector<std::string> expected_result = {"Ala", "ma", "kota"};
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

BOOST_AUTO_TEST_CASE(strip_test) {
  {
    std::string text = "    Ala ma kota.\n\n\t";
    BOOST_CHECK_EQUAL(text::strip(text.begin(), text.end(), IsSpace), "Ala ma kota.");
  }

  {
    std::string text = " \n\n \t    ";
    BOOST_CHECK_EQUAL(text::strip(text.begin(), text.end(), IsSpace), "");
  }

  {
    std::string text = "";
    BOOST_CHECK_EQUAL(text::strip(text.begin(), text.end(), IsSpace), "");
  }

  {
    std::string text = "aaaabaaabaaaaa";
    BOOST_CHECK_EQUAL(text::strip(text.begin(), text.end(), 'a'), "baaab");
  }

  {
    std::string text = "abababababbbababcababcabababab";
    BOOST_CHECK_EQUAL(text::strip(text.begin(), text.end(), "ab"), "cababc");
  }
}

BOOST_AUTO_TEST_SUITE_END()
