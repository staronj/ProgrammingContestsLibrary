// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "io.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(io_suite)

struct Noncopyable {
  Noncopyable() = default;
  Noncopyable(const Noncopyable&) = delete;
  Noncopyable(Noncopyable&&) = default;
  Noncopyable& operator=(const Noncopyable&) = delete;

  friend std::ostream& operator<<(std::ostream& stream, const Noncopyable&) {
    return stream << "noncopyable";
  }
};


BOOST_AUTO_TEST_CASE(ostream_printing_test) {
  {
    auto pair = std::make_pair(1, "Ala");
    std::ostringstream stream;
    stream << pair;
    BOOST_CHECK_EQUAL(stream.str(), "1 Ala");
  }

  {
    auto pair = std::make_pair(1, Noncopyable());
    std::ostringstream stream;
    stream << fancy << pair;
    BOOST_CHECK_EQUAL(stream.str(), "(1, noncopyable)");
  }

  {
    Noncopyable non;
    auto tuple = std::make_tuple(1, "Ala", std::ref(non));
    std::ostringstream stream;
    stream << tuple;
    BOOST_CHECK_EQUAL(stream.str(), "1 Ala noncopyable");
  }

  {
    auto tuple = std::make_tuple(1, "Ala", 'c');
    std::ostringstream stream;
    stream << fancy << tuple;
    BOOST_CHECK_EQUAL(stream.str(), "(1, Ala, c)");
  }

  {
    auto tuple = std::make_tuple(1, std::make_pair("ma", "kota"), 'c');
    std::ostringstream stream;
    stream << fancy << tuple;
    BOOST_CHECK_EQUAL(stream.str(), "(1, (ma, kota), c)");
  }

  {
    auto pair = std::make_pair(1, std::make_tuple("ma", "kota", 'c'));
    std::ostringstream stream;
    stream << fancy << pair;
    BOOST_CHECK_EQUAL(stream.str(), "(1, (ma, kota, c))");
  }

  {
    std::vector<int> v = {1, 2, 3};
    std::ostringstream stream;
    stream << v;
    BOOST_CHECK_EQUAL(stream.str(), "1 2 3");
  }

  {
    std::vector<int> v = {1, 2, 3};
    std::ostringstream stream;
    stream << fancy << v;
    BOOST_CHECK_EQUAL(stream.str(), "(1, 2, 3)");
  }
}


BOOST_AUTO_TEST_CASE(print_test) {
  {
    std::ostringstream stream;
    print(stream, "");
    BOOST_CHECK_EQUAL(stream.str(), "\n");
  }

  {
    std::ostringstream stream;
    print(stream, "Ala");
    BOOST_CHECK_EQUAL(stream.str(), "Ala\n");
  }

  {
    std::ostringstream stream;
    print(stream, "Ala %0 kota", "ma");
    BOOST_CHECK_EQUAL(stream.str(), "Ala ma kota\n");
  }

  {
    std::ostringstream stream;
    Noncopyable non;
    print(stream, "%0 %1 %0 %2", "Ala", "kota", non);
    BOOST_CHECK_EQUAL(stream.str(), "Ala kota Ala noncopyable\n");
  }

  {
    std::ostringstream stream;
    BOOST_CHECK_THROW(print(stream, "%0 %2", "Ala"), std::exception);
  }

  {
    std::ostringstream stream;
    BOOST_CHECK_THROW(print(stream, "%"), std::exception);
  }

  {
    std::ostringstream stream;
    BOOST_CHECK_THROW(print(stream, " Ala %"), std::exception);
  }

  {
    std::ostringstream stream;
    print(stream, "%%");
    BOOST_CHECK_EQUAL(stream.str(), "%\n");
  }

  {
    std::ostringstream stream;
    print(stream, "%%0 %0 %%%0", "Ala");
    BOOST_CHECK_EQUAL(stream.str(), "%0 Ala %Ala\n");
  }

  {
    std::ostringstream stream;
    print(stream, "%0%1 %2%3", std::boolalpha, true, fancy, std::make_pair(1, 2));
    BOOST_CHECK_EQUAL(stream.str(), "true (1, 2)\n");
  }

  {
    std::ostringstream stream;
    print(stream, "%0", flush);
    BOOST_CHECK_EQUAL(stream.str(), "\n");
  }
}

BOOST_AUTO_TEST_CASE(pair_tuple_input_test) {
  {
    std::istringstream stream("1 2");
    std::pair<int, int> pair;
    stream >> pair;
    BOOST_CHECK(pair == std::make_pair(1, 2));
  }

  {
    std::istringstream stream("Ala a");
    std::pair<std::string, char> pair;
    stream >> pair;
    BOOST_CHECK(pair == std::make_pair(std::string("Ala"), 'a'));
  }

  {
    std::istringstream stream("Ala 1 2");
    std::tuple<std::string, int, int> tuple;
    stream >> tuple;
    BOOST_CHECK(tuple == std::make_tuple(std::string("Ala"), 1, 2));
  }

  {
    std::istringstream stream("1 2 3");
    int a,b,c;
    stream >> std::tie(a, b, c);
    BOOST_CHECK_EQUAL(a, 1);
    BOOST_CHECK_EQUAL(b, 2);
    BOOST_CHECK_EQUAL(c, 3);
  }
}

BOOST_AUTO_TEST_CASE(read_test) {
  {
    std::istringstream stream("1 2 3");
    int a, b, c;
    read(stream, a, b, c);
    BOOST_CHECK_EQUAL(a, 1);
    BOOST_CHECK_EQUAL(b, 2);
    BOOST_CHECK_EQUAL(c, 3);
  }

  {
    std::istringstream stream("Ala 1 2");
    std::tuple<std::string, int, int> tuple;
    read(stream, tuple);
    BOOST_CHECK(tuple == std::make_tuple(std::string("Ala"), 1, 2));
  }

  {
    std::istringstream stream("1 2 3");
    int a, b;
    stream >> a >> ignore<int>() >> b;
    BOOST_CHECK_EQUAL(a, 1);
    BOOST_CHECK_EQUAL(b, 3);
  }

  {
    std::istringstream stream("1 2 3");
    int a, b;
    read(stream, a, ignore<int>(), b);
    BOOST_CHECK_EQUAL(a, 1);
    BOOST_CHECK_EQUAL(b, 3);
  }
}

#ifdef USE_INT128_TYPES

BOOST_AUTO_TEST_CASE(int128_test) {
  {
    std::ostringstream stream;
    int128 n = 0;
    stream << n;
    BOOST_CHECK_EQUAL(stream.str(), "0");
  }

  {
    std::ostringstream stream;
    int128 n = 1;
    stream << n;
    BOOST_CHECK_EQUAL(stream.str(), "1");
  }

  {
    std::ostringstream stream;
    int128 n = -1;
    stream << n;
    BOOST_CHECK_EQUAL(stream.str(), "-1");
  }

  {
    uint128 million = 1000 * 1000;
    std::ostringstream stream;
    int128 n = million * million * million * million * million * million + million;
    stream << n;
    BOOST_CHECK_EQUAL(stream.str(), "1000000000000000000000000000001000000");
  }

  {
    uint128 million = 1000 * 1000;
    std::ostringstream stream;
    int128 n = million * million * million * million * million * million + million;
    stream << (-n);
    BOOST_CHECK_EQUAL(stream.str(), "-1000000000000000000000000000001000000");
  }
}

#endif

BOOST_AUTO_TEST_CASE(lines_iterator_test) {
  {
    std::istringstream stream;
    std::vector<std::string> result{lines_iterator(&stream), lines_iterator()};
    std::vector<std::string> expected_result = {""};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::istringstream stream("Ala");
    std::vector<std::string> result{lines_iterator(&stream), lines_iterator()};
    std::vector<std::string> expected_result = {"Ala"};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::istringstream stream("Ala\n");
    std::vector<std::string> result{lines_iterator(&stream), lines_iterator()};
    std::vector<std::string> expected_result = {"Ala", ""};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::istringstream stream("Ala\nma\n\nkota");
    std::vector<std::string> result{lines_iterator(&stream), lines_iterator()};
    std::vector<std::string> expected_result = {"Ala", "ma", "", "kota"};
    BOOST_CHECK(result == expected_result);
  }
}

BOOST_AUTO_TEST_CASE(newline_test) {
  std::ostringstream stream;
  stream << newline;
  BOOST_CHECK_EQUAL(stream.str(), "\n");
}

BOOST_AUTO_TEST_CASE(iterate_lines_test) {
  {
    std::istringstream stream;
    std::vector<std::string> result;
    for (const auto& line: iterate_lines(stream)) {
      result.push_back(line);
    }
    std::vector<std::string> expected_result = {""};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::istringstream stream("Ala");
    std::vector<std::string> result;
    for (const auto& line: iterate_lines(stream)) {
      result.push_back(line);
    }
    std::vector<std::string> expected_result = {"Ala"};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::istringstream stream("Ala\n");
    std::vector<std::string> result;
    for (const auto& line: iterate_lines(stream)) {
      result.push_back(line);
    }
    std::vector<std::string> expected_result = {"Ala", ""};
    BOOST_CHECK(result == expected_result);
  }

  {
    std::istringstream stream("Ala\nma\n\nkota");
    std::vector<std::string> result;
    for (const auto& line: iterate_lines(stream)) {
      result.push_back(line);
    }
    std::vector<std::string> expected_result = {"Ala", "ma", "", "kota"};
    BOOST_CHECK(result == expected_result);
  }
}

BOOST_AUTO_TEST_SUITE_END()
