// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "iterators.h"
#include "numeric.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(numeric_test)

BOOST_AUTO_TEST_CASE(divides_test) {
  BOOST_CHECK(divides(1, 2));
  BOOST_CHECK(divides(2, 2));
  BOOST_CHECK(!divides(3, 2));
  BOOST_CHECK(!divides(3, 1));
}

BOOST_AUTO_TEST_CASE(gcd_test) {
  BOOST_CHECK_EQUAL(GCD(1, 1), 1);
  BOOST_CHECK_EQUAL(GCD(8, 12), 4);
  BOOST_CHECK_EQUAL(GCD(100, 43), 1);
}

BOOST_AUTO_TEST_CASE(pow_test) {
  BOOST_CHECK_EQUAL(power(1, 1000000), 1);
  BOOST_CHECK_EQUAL(power(-1, 1000 * 1000 * 1000 + 1), -1);
  BOOST_CHECK_EQUAL(power(101, 0), 1);
  BOOST_CHECK_EQUAL(power(2, 10), 1024);
}

BOOST_AUTO_TEST_CASE(pow_modulo_test) {
  uint64 modulo = pow(10, 9) + 7;
  BOOST_CHECK_EQUAL(pow_modulo(2, power(10, 17), modulo), 952065854);
  BOOST_CHECK_EQUAL(pow_modulo(3, power(10, 17), modulo), 368629774);
  BOOST_CHECK_EQUAL(pow_modulo(3, power(10, 17) + 1, modulo), 105889315);
}

BOOST_AUTO_TEST_CASE(signum_test) {
  BOOST_CHECK_EQUAL(signum(0), 0);
  BOOST_CHECK_EQUAL(signum(-100), -1);
  BOOST_CHECK_EQUAL(signum(1000), 1);
  BOOST_CHECK_EQUAL(signum(std::numeric_limits<int>::lowest()), -1);
  BOOST_CHECK_EQUAL(signum(std::numeric_limits<int64>::lowest()), -1);
  BOOST_CHECK_EQUAL(signum(std::numeric_limits<int>::max()), 1);
  BOOST_CHECK_EQUAL(signum(std::numeric_limits<int64>::max()), 1);
}

BOOST_AUTO_TEST_CASE(sqrt_test) {
  BOOST_CHECK_EQUAL(square(0), 0);
  BOOST_CHECK_EQUAL(square(1), 1);
  BOOST_CHECK_EQUAL(square(100), 10);
  BOOST_CHECK_EQUAL(square((1uLL << 62) - 1), (1uLL << 31) - 1);
}

BOOST_AUTO_TEST_CASE(pop_count_test) {
  BOOST_CHECK_EQUAL(pop_count(0), 0);
  BOOST_CHECK_EQUAL(pop_count(1), 1);
  BOOST_CHECK_EQUAL(pop_count(0xFFFFFFFF), 32);
  BOOST_CHECK_EQUAL(pop_count(7), 3);
  BOOST_CHECK_EQUAL(pop_count(0xFFFFFFFFFFFFFFFFuLL), 64);
  BOOST_CHECK_EQUAL(pop_count(0xFFFFFFFFFFFFFFuLL), 56);
}

BOOST_AUTO_TEST_CASE(trailing_zeros_test) {
  BOOST_CHECK_EQUAL(trailing_zeros(1), 0);
  BOOST_CHECK_EQUAL(trailing_zeros(2), 1);
  BOOST_CHECK_EQUAL(trailing_zeros(0xFFFFFFFF), 0);
  BOOST_CHECK_EQUAL(trailing_zeros(1uLL << 33), 33);
  BOOST_CHECK_EQUAL(trailing_zeros(1uLL << 63), 63);
  BOOST_CHECK_EQUAL(trailing_zeros(47 * 64), 6);
  BOOST_CHECK_EQUAL(trailing_zeros((1uLL << 42) * 11111), 42);
}

BOOST_AUTO_TEST_CASE(integer_log2_test) {
  BOOST_CHECK_EQUAL(integer_log2(0), 0);
  BOOST_CHECK_EQUAL(integer_log2(1), 0);
  BOOST_CHECK_EQUAL(integer_log2(0xFFFFFFFF), 31);
  BOOST_CHECK_EQUAL(integer_log2(7), 2);
  BOOST_CHECK_EQUAL(integer_log2(8), 3);
  BOOST_CHECK_EQUAL(integer_log2(0xFFFFFFFFFFFFFFFFuLL), 63);
  BOOST_CHECK_EQUAL(integer_log2(0xFFFFFFFFFFFFFFuLL), 55);
}

BOOST_AUTO_TEST_CASE(primes_test) {
  constexpr uint32 N = 1000 * 1000;
  auto primes = sieve_of_eratostenes(1000 * 1000);
  for (auto i: range(0u, N)) {
    BOOST_CHECK_MESSAGE(primes[i] == miller_rabin_prime_test(i),
                        "eratostenes and miller-rabin not equal for i = " << i);
  }

  auto primes2 = linear_sieve(N);
  for (auto i: range(0u, N)) {
    BOOST_CHECK_MESSAGE(primes[i] == primes2[i],
                        "eratostenes and linear sieve not equal for i = " << i);
  }

  BOOST_CHECK(sieve_of_eratostenes(100 * 1000 * 1000) == linear_sieve(100 * 1000 * 1000));
}

BOOST_AUTO_TEST_SUITE_END()
