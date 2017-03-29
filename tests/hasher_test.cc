// Jakub Staroń, 2016-2017

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "io.h"
#include "text_algorithms/hasher.h"
#include "numeric/number_theory.h"

using namespace pcl;

BOOST_AUTO_TEST_SUITE(hasher_test)

BOOST_AUTO_TEST_CASE(multipler_test) {
  BOOST_CHECK(numeric::IsPrimitiveRoot(uint32_prime1, hash::multipler.first.value()));
  BOOST_CHECK(numeric::IsPrimitiveRoot(uint32_prime2, hash::multipler.second.value()));
}

BOOST_AUTO_TEST_CASE(string_hash_test) {
  BOOST_CHECK(hash::hash("") == hash::zero);
  BOOST_CHECK(hash::hash("Ala") == hash::hash("Ala"));
  BOOST_CHECK(hash::hash("Ala") != hash::hash("ma"));
}

BOOST_AUTO_TEST_CASE(hasher_test) {
  {
    std::string text = "";
    Hasher hasher(text.begin(), text.end());
    BOOST_CHECK(hasher.getHash(0, 0) == hash::hash(""));
  }

  {
    std::string text = "a";
    Hasher hasher(text.begin(), text.end());
    BOOST_CHECK(hasher.getHash(0, 0) == hasher.getHash(1, 0));
    BOOST_CHECK(hasher.getHash(0, 1) == hash::hash("a"));
  }

  {
    std::string text = "aa";
    Hasher hasher(text.begin(), text.end());
    BOOST_CHECK(hasher.getHash(0, 1) == hasher.getHash(1, 1));
    BOOST_CHECK(hasher.getHash(0, 1) == hash::hash("a"));
    BOOST_CHECK(hasher.getHash(0, 2) == hash::hash("aa"));
  }

  {
    std::string text = "ababaabab";
    Hasher hasher(text.begin(), text.end());
    BOOST_CHECK(hasher.getHash(0, 2) == hasher.getHash(2, 2));
    BOOST_CHECK(hasher.getHash(0, 2) != hasher.getHash(1, 2));
    BOOST_CHECK(hasher.getHash(0, 2) == hasher.getHash(5, 2));
    BOOST_CHECK(hasher.getHash(0, 2) == hasher.getHash(7, 2));
    BOOST_CHECK(hasher.getHash(0, 4) == hasher.getHash(5, 4));
    BOOST_CHECK(hasher.getHash(0, 3) != hasher.getHash(6, 3));

    BOOST_CHECK(hasher.getHash(5, 3) == hash::hash("aba"));
    BOOST_CHECK(hasher.getHash(3, 4) == hash::hash("baab"));
  }

  {
    std::string text = "ababaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
    Hasher hasher(text.begin(), text.end());
    BOOST_CHECK(hasher.getHash(0, 2) == hasher.getHash(46, 2));
    BOOST_CHECK(hasher.getHash(2, 2) == hasher.getHash(46, 2));
    BOOST_CHECK(hasher.getHash(1, 2) != hasher.getHash(46, 2));
    BOOST_CHECK(hasher.getHash(4, 42) == hasher.getHash(5, 42));
    BOOST_CHECK(hasher.getHash(5, 42) != hasher.getHash(6, 42));

    BOOST_CHECK(hasher.getHash(46, 2) == hash::hash("ab"));
  }
}

BOOST_AUTO_TEST_SUITE_END()
