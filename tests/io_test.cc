// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "io.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(io)

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
}

BOOST_AUTO_TEST_SUITE_END()
