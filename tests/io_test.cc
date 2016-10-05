// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "io.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(io)

BOOST_AUTO_TEST_CASE(ostream_printing_test) {
  {
    auto tuple = std::make_tuple(1, 2, 3);
    std::ostringstream stream;
    stream << tuple;
  }
}

BOOST_AUTO_TEST_SUITE_END()
