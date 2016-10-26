// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "io.h"
#include "text_algorithms/hasher.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(hasher_test)

BOOST_AUTO_TEST_CASE(multipler_test) {
  BOOST_CHECK(is_primitive_root(uint32_prime1, hash::multipler.first.value()));
  BOOST_CHECK(is_primitive_root(uint32_prime2, hash::multipler.second.value()));
}

BOOST_AUTO_TEST_CASE(hasher_test) {
  {
    std::string text = "";
    Hasher hasher;
    hasher.feed(text.begin(), text.end());
  }

  {
    std::string text = "a";
    Hasher hasher;
    hasher.feed(text.begin(), text.end());
    BOOST_CHECK(hasher.get_hash(0, 0) == hasher.get_hash(1, 0));
  }

  {
    std::string text = "aa";
    Hasher hasher;
    hasher.feed(text.begin(), text.end());
    BOOST_CHECK(hasher.get_hash(0, 1) == hasher.get_hash(1, 1));
  }

  {
    std::string text = "ababaabab";
    Hasher hasher;
    hasher.feed(text.begin(), text.end());
    BOOST_CHECK(hasher.get_hash(0, 2) == hasher.get_hash(2, 2));
    BOOST_CHECK(hasher.get_hash(0, 2) != hasher.get_hash(1, 2));
    BOOST_CHECK(hasher.get_hash(0, 2) == hasher.get_hash(5, 2));
    BOOST_CHECK(hasher.get_hash(0, 2) == hasher.get_hash(7, 2));
    BOOST_CHECK(hasher.get_hash(0, 4) == hasher.get_hash(5, 4));
    BOOST_CHECK(hasher.get_hash(0, 3) != hasher.get_hash(6, 3));
  }

  {
    std::string text = "ababaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
    Hasher hasher;
    hasher.feed(text.begin(), text.end());
    BOOST_CHECK(hasher.get_hash(0, 2) == hasher.get_hash(46, 2));
    BOOST_CHECK(hasher.get_hash(2, 2) == hasher.get_hash(46, 2));
    BOOST_CHECK(hasher.get_hash(1, 2) != hasher.get_hash(46, 2));
    BOOST_CHECK(hasher.get_hash(4, 42) == hasher.get_hash(5, 42));
    BOOST_CHECK(hasher.get_hash(5, 42) != hasher.get_hash(6, 42));
  }
}

BOOST_AUTO_TEST_SUITE_END()
