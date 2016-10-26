// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "numeric.h"
#include "numeric/prime_field.h"
#include "io.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(prime_field_suite)

BOOST_AUTO_TEST_CASE(creation_test) {
  {
    numeric::prime_field<7> n(10);
    BOOST_CHECK_EQUAL(n.value(), 3);
  }

  {
    numeric::prime_field<5> n(10);
    BOOST_CHECK_EQUAL(n.value(), 0);
  }

  {
    numeric::prime_field<5> n(4);
    numeric::prime_field<5> m(-1);
    BOOST_CHECK_EQUAL(n.value(), 4);
    BOOST_CHECK_EQUAL(m.value(), 4);
    BOOST_CHECK_EQUAL(n.value(), m.value());
  }

  {
    uint64 value = (1uLL << 63) + uint64(1000 * 1000 * 1000);
    numeric::prime_field<5> n(value);
    BOOST_CHECK_EQUAL(n.value(), 3);
  }

  {
    int64 value = (1uLL << 62) + uint64(1000 * 1000 * 1000);
    numeric::prime_field<5> n(value);
    BOOST_CHECK_EQUAL(n.value(), 4);
  }

  {
    int64 value = (1uLL << 62) + uint64(1000 * 1000 * 1000);
    numeric::prime_field<5> n(-value);
    BOOST_CHECK_EQUAL(n.value(), 1);
  }

  {
    uint64 value = uint64(uint32_prime1) * uint64(uint32_prime1) + 100;
    numeric::prime_field<uint32_prime1> n(value);
    BOOST_CHECK_EQUAL(n.value(), 100);
  }

  {
    numeric::prime_field<uint32_prime1> n(uint32_prime1 - 1);
    numeric::prime_field<uint32_prime1> m(-1);
    BOOST_CHECK_EQUAL(n.value(), m.value());
  }

  {
    numeric::prime_field<uint32_prime1> n(-int64(uint32_prime1) - int64(uint32_prime1) - 10);
    BOOST_CHECK_EQUAL(n, -10);
  }

  {
    int64 value = uint32_prime1;
    numeric::prime_field<uint32_prime1> n((value << 20) + 10);
    BOOST_CHECK_EQUAL(n.value(), 10);
  }
}

BOOST_AUTO_TEST_CASE(addition_test) {
  {
    numeric::prime_field<5> a(2);
    numeric::prime_field<5> b(3);
    BOOST_CHECK_EQUAL(a + a, 4);
    BOOST_CHECK_EQUAL(a + b, 0);
    BOOST_CHECK_EQUAL(b + b, 1);
    BOOST_CHECK_EQUAL(a + a + b, a);
  }

  {
    numeric::prime_field<uint32_prime1> a(uint32_prime1 - 1);
    numeric::prime_field<uint32_prime1> b(uint32_prime1 - 2);
    BOOST_CHECK_EQUAL(a + b, b + a);
    BOOST_CHECK_EQUAL(a + b + 3, 0);
    BOOST_CHECK_EQUAL(3 + a + b, 0);
    BOOST_CHECK_EQUAL(a + 3 + b, 0);
    BOOST_CHECK_EQUAL(a + a + b + 8, 4);
  }
}

BOOST_AUTO_TEST_CASE(substraction_test) {
  {
    numeric::prime_field<5> a(2);
    numeric::prime_field<5> b(3);
    BOOST_CHECK_EQUAL(a - a, 0);
    BOOST_CHECK_EQUAL(a - b, 4);
    BOOST_CHECK_EQUAL(b - a, 1);
    BOOST_CHECK_EQUAL(b - a - a, 4);
  }

  {
    numeric::prime_field<uint32_prime1> a(uint32_prime1 - 1);
    numeric::prime_field<uint32_prime1> b(uint32_prime1 - 2);
    BOOST_CHECK_EQUAL(a - b, 1);
    BOOST_CHECK_EQUAL(b - a, uint32_prime1 - 1);
    BOOST_CHECK_EQUAL(b - a + 1, 0);
    BOOST_CHECK_EQUAL(b - (uint32_prime1 + 1) + 3, 0);
  }
}

BOOST_AUTO_TEST_CASE(multiplication_test) {
  {
    numeric::prime_field<5> a(1);
    numeric::prime_field<5> b(2);
    BOOST_CHECK_EQUAL(a * a, a);
    BOOST_CHECK_EQUAL(a * 1, a);
    BOOST_CHECK_EQUAL(1 * a, a);
    BOOST_CHECK_EQUAL(a * 0, 0);
    BOOST_CHECK_EQUAL(a * b, b);
    BOOST_CHECK_EQUAL(b * b * b, 3);
  }

  {
    numeric::prime_field<uint32_prime1> a(uint32_prime1 - 1);
    numeric::prime_field<uint32_prime1> b(uint32_prime1 - 2);
    BOOST_CHECK_EQUAL(a * b, 2);
    BOOST_CHECK_EQUAL(a * a, 1);
    BOOST_CHECK_EQUAL(b * b * b * b, 16);
  }
}

BOOST_AUTO_TEST_CASE(division_test) {
  {
    numeric::prime_field<5> a(2);
    numeric::prime_field<5> zero(0);
    BOOST_CHECK_EQUAL(2 / a, 1);
    BOOST_CHECK_EQUAL(a / 2, 1);
    BOOST_CHECK_EQUAL(4 / a, 2);
    BOOST_CHECK_EQUAL(9 / a, 2);
    BOOST_CHECK_EQUAL(a / 3, 4);
  }

  {
    numeric::prime_field<uint32_prime1> a(uint32_prime1 - 1);
    numeric::prime_field<uint32_prime1> b(uint32_prime1 - 2);
    BOOST_CHECK_EQUAL(b / a, 2);
    BOOST_CHECK_EQUAL(inverse(a / b), 2);
    BOOST_CHECK_EQUAL(a / a, 1);
    BOOST_CHECK_EQUAL(b / b, 1);
  }
}

BOOST_AUTO_TEST_CASE(io_test) {
  {
    std::ostringstream stream;
    numeric::prime_field<5> a(9);
    stream << a;
    BOOST_CHECK_EQUAL(stream.str(), "4");
  }

  {
    std::ostringstream stream;
    numeric::prime_field<uint32_prime1> a(uint32_prime1 - 1);
    stream << a;
    BOOST_CHECK_EQUAL(stream.str(), std::to_string(uint32_prime1 - 1));
  }

  {
    std::istringstream stream("1234");
    numeric::prime_field<5> a;
    stream >> a;
    BOOST_CHECK_EQUAL(a, 4);
  }

  {
    std::istringstream stream("-1");
    numeric::prime_field<5> a;
    stream >> a;
    BOOST_CHECK_EQUAL(a, 4);
  }

  {
    std::istringstream stream(std::to_string(uint32_prime1 - 1));
    numeric::prime_field<uint32_prime1> a;
    stream >> a;
    BOOST_CHECK_EQUAL(a, uint32_prime1 - 1);
  }
}

BOOST_AUTO_TEST_SUITE_END()
