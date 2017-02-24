// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "headers.h"
#include "iterators.h"
#include "numeric.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(header_suite)

BOOST_AUTO_TEST_CASE(random32_test1) {
  std::bitset<100> bits;
  for (auto i: range<uint32>(0, 10000)) {
    uint32 n = Random32(100);
    BOOST_CHECK(0 <= n && n < 100);
    bits[n] = true;
  }
  BOOST_CHECK(bits.all());
}

BOOST_AUTO_TEST_CASE(random32_test2) {
  constexpr uint32 begin = 1000 * 1000;
  constexpr uint32 end = begin + 100;

  std::bitset<100> bits;
  for (auto i: range<uint32>(0, 10000)) {
    uint32 n = Random32(begin, end);
    BOOST_CHECK(begin <= n && n < end);
    bits[n - begin] = true;
  }
  BOOST_CHECK(bits.all());
}

BOOST_AUTO_TEST_CASE(random64_test1) {
  std::bitset<100> bits;
  for (auto i: range<uint32>(0, 10000)) {
    auto n = Random64(100);
    BOOST_CHECK(0 <= n && n < 100);
    bits[n] = true;
  }
  BOOST_CHECK(bits.all());
}

BOOST_AUTO_TEST_CASE(random64_test2) {
  constexpr uint64 begin = uint32_infinity * 999uLL;
  constexpr uint64 end = begin + 100;

  std::bitset<100> bits;
  for (auto i: range<uint32>(0, 10000)) {
    auto n = Random64(begin, end);
    BOOST_CHECK(begin <= n && n < end);
    bits[n - begin] = true;
  }
  BOOST_CHECK(bits.all());
}

BOOST_AUTO_TEST_SUITE_END()
