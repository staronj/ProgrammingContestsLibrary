// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "iterators.h"
#include "io.h"
#include "numeric/number_theory.h"

using namespace lib::numeric;

BOOST_AUTO_TEST_SUITE(numeric_theory_test)

BOOST_AUTO_TEST_CASE(gcd_test) {
  BOOST_CHECK_EQUAL(GCD(1, 1), 1);
  BOOST_CHECK_EQUAL(GCD(8, 12), 4);
  BOOST_CHECK_EQUAL(GCD(100, 43), 1);
  BOOST_CHECK_EQUAL(GCD((1uLL << 63) + 16, (1uLL << 62) + 16), 16);
  BOOST_CHECK_EQUAL(GCD((1uLL << 63) + (1uLL << 42), (1uLL << 63) + (1uLL << 41)), (1uLL << 41));
}

BOOST_AUTO_TEST_CASE(extended_gcd_test) {
  using namespace lib;
  std::vector<int64_pair> tests = {
      {0, 1},
      {1, 0},
      {0, 2},
      {2, 0},
      {1, 1},
      {10, 3},
      {10, 100},
      {12, 16},
      {0xFFFFFFFBu, 0xFFFFFFBFu},
  };

  for(const auto& test: tests) {
    int64 a,b,x,y;
    std::tie(a, b) = test;
    std::tie(x, y) = ExtendedGCD(a, b);
    BOOST_CHECK_MESSAGE(a * x + b * y == GCD(a, b),
                        "Wrong extended gcd for pair (" + std::to_string(a) + ", " + std::to_string(b) + ")"
    );
  }
}

BOOST_AUTO_TEST_CASE(merge_congruences_test) {
  using namespace lib;
  using test_type = std::tuple<uint64_pair, uint64_pair, uint64_pair>;
  std::vector<test_type> tests = {
      test_type {
          {1, 2},
          {1, 3},
          {1, 6}
      },
      test_type {
          {2, 4},
          {4, 6},
          {10, 12}
      },
      test_type {
          {13, 16},
          {9, 12},
          {45, 48}
      },
  };

  for(const auto& test: tests) {
    uint64_pair a, b, ab;
    std::tie(a, b, ab) = test;
    uint64_pair result = MergeCongruences(a, b);

    std::ostringstream errorMessage;
    errorMessage << fancy;
    print(errorMessage, "Wrong result: congruences %0 and %1 gave %2", a, b, result);

    BOOST_CHECK_MESSAGE(ab == result, errorMessage.str());
  }
}

BOOST_AUTO_TEST_CASE(merge_congruences_contradicting_test) {
  using namespace lib;
  using test_type = std::tuple<uint64_pair, uint64_pair>;
  std::vector<test_type> tests = {
      test_type {
          {1, 2},
          {2, 4},
      },
      test_type {
          {3, 4},
          {2, 6},
      },
      test_type {
          {0, 16},
          {2, 12},
      },
  };

  for(const auto& test: tests) {
    uint64_pair a, b;
    std::tie(a, b) = test;
    BOOST_CHECK_THROW(MergeCongruences(a, b), std::runtime_error);
  }
}

BOOST_AUTO_TEST_CASE(merge_congruences_iterable_test) {
  using namespace lib;
  std::vector<uint64_pair> congruences = {
      {1, 2},
      {2, 3},
      {4, 5},
      {6, 7}
  };
  uint64_pair result{209, 210};
  BOOST_CHECK(MergeCongruences(congruences.begin(), congruences.end()) == result);
}

BOOST_AUTO_TEST_CASE(operations_32bits_test) {
  BOOST_CHECK_EQUAL(Add32(4, 5, 6), 3);
  BOOST_CHECK_EQUAL(Add32(0, 0, 13), 0);
  BOOST_CHECK_EQUAL(Add32(1u << 31, 1u << 31, (1u << 31) - 1), 2);

  BOOST_CHECK_EQUAL(Subtract32(4, 5, 6), 5);
  BOOST_CHECK_EQUAL(Subtract32(0, 0, 13), 0);
  BOOST_CHECK_EQUAL(Subtract32((1u << 31) - 2, (1u << 31) - 1, (1u << 31) - 1), (1u << 31) - 2);

  BOOST_CHECK_EQUAL(Multiply32(4, 5, 6), 2);
  BOOST_CHECK_EQUAL(Multiply32(0, 0, 13), 0);
  BOOST_CHECK_EQUAL(Multiply32((1u << 31) - 1, (1u << 31) - 1, (1u << 31)), 1);
  BOOST_CHECK_EQUAL(Multiply32((1u << 31) - 1, (1u << 31) - 2, (1u << 31)), 2);
}

BOOST_AUTO_TEST_CASE(power_modulo_test) {
  lib::uint64 modulo = lib::power(10, 9) + 7;
  BOOST_CHECK_EQUAL(PowerModulo32(2, lib::power(10, 17), modulo), 952065854);
  BOOST_CHECK_EQUAL(PowerModulo32(3, lib::power(10, 17), modulo), 368629774);
  BOOST_CHECK_EQUAL(PowerModulo32(3, lib::power(10, 17) + 1, modulo), 105889315);
}

BOOST_AUTO_TEST_CASE(square_test) {
  BOOST_CHECK_EQUAL(SquareCeiling(0), 0);
  BOOST_CHECK_EQUAL(SquareFloor(0), 0);
  BOOST_CHECK_EQUAL(SquareCeiling(1), 1);
  BOOST_CHECK_EQUAL(SquareFloor(1), 1);
  BOOST_CHECK_EQUAL(SquareCeiling(2), 2);
  BOOST_CHECK_EQUAL(SquareFloor(2), 1);
  BOOST_CHECK_EQUAL(SquareCeiling(4), 2);
  BOOST_CHECK_EQUAL(SquareFloor(4), 2);

  BOOST_CHECK_EQUAL(SquareCeiling(1000 * 1000), 1000);
  BOOST_CHECK_EQUAL(SquareFloor(1000 * 1000), 1000);

  BOOST_CHECK_EQUAL(SquareCeiling(1000 * 1000 + 1), 1001);
  BOOST_CHECK_EQUAL(SquareFloor(1000 * 1000 + 1), 1000);

  lib::uint64 billion = 1000uLL * 1000uLL * 1000uLL;
  BOOST_CHECK_EQUAL(SquareCeiling(billion * billion), billion);
  BOOST_CHECK_EQUAL(SquareFloor(billion * billion), billion);

  BOOST_CHECK_EQUAL(SquareCeiling(billion * billion + 1), billion + 1);
  BOOST_CHECK_EQUAL(SquareFloor(billion * billion + 1), billion);
}

BOOST_AUTO_TEST_CASE(divisors_test) {
  using namespace lib;

  auto ExpectedDivisors = [](uint32 n) {
    std::vector<uint32> divisors;
    for (auto d: range<uint32>(1, n + 1)) {
      if (n % d == 0)
        divisors.push_back(d);
    }
    return divisors;
  };

  {
    uint32 N = 1;
    BOOST_CHECK(Divisors(N) == ExpectedDivisors(N));
  }

  {
    uint32 N = 12;
    BOOST_CHECK(Divisors(N) == ExpectedDivisors(N));
  }

  {
    uint32 N = 13;
    BOOST_CHECK(Divisors(N) == ExpectedDivisors(N));
  }

  {
    uint32 N = 12345678;
    BOOST_CHECK(Divisors(N) == ExpectedDivisors(N));
  }
}

BOOST_AUTO_TEST_CASE(prime_divisors_test) {
  using namespace lib;

  {
    uint32 N = 1;
    auto divisors = PrimeDivisors(N);
    BOOST_CHECK(divisors.end() == std::unique(divisors.begin(), divisors.end()));
  }

  {
    uint32 N = 12;
    auto divisors = PrimeDivisors(N);
    BOOST_CHECK(divisors.end() == std::unique(divisors.begin(), divisors.end()));
  }

  {
    uint32 N = 13;
    auto divisors = PrimeDivisors(N);
    BOOST_CHECK(divisors.end() == std::unique(divisors.begin(), divisors.end()));
  }

  {
    uint32 N = 12345678;
    auto divisors = PrimeDivisors(N);
    BOOST_CHECK(divisors.end() == std::unique(divisors.begin(), divisors.end()));
  }
}

BOOST_AUTO_TEST_CASE(factorization_test) {
  using namespace lib;

  auto Defactorize = [](const std::vector<uint32>& v) {
    uint32 result = 1;
    for (auto i: v)
      result *= i;
    return result;
  };

  {
    uint32 N = 1;
    auto factorization = Factorize(N);
    BOOST_CHECK(std::is_sorted(factorization.begin(), factorization.end()));
    BOOST_CHECK_EQUAL(Defactorize(factorization), N);
    BOOST_CHECK(std::all_of(factorization.begin(), factorization.end(), IsPrime));
  }

  {
    uint32 N = 2;
    auto factorization = Factorize(N);
    BOOST_CHECK(std::is_sorted(factorization.begin(), factorization.end()));
    BOOST_CHECK_EQUAL(Defactorize(factorization), N);
    BOOST_CHECK(std::all_of(factorization.begin(), factorization.end(), IsPrime));
  }

  {
    uint32 N = 12345678;
    auto factorization = Factorize(N);
    BOOST_CHECK(std::is_sorted(factorization.begin(), factorization.end()));
    BOOST_CHECK_EQUAL(Defactorize(factorization), N);
    BOOST_CHECK(std::all_of(factorization.begin(), factorization.end(), IsPrime));
  }

  {
    uint32 N = 64;
    auto factorization = Factorize(N);
    BOOST_CHECK(std::is_sorted(factorization.begin(), factorization.end()));
    BOOST_CHECK_EQUAL(Defactorize(factorization), N);
    BOOST_CHECK(std::all_of(factorization.begin(), factorization.end(), IsPrime));
  }

  {
    uint32 N = 47;
    auto factorization = Factorize(N);
    BOOST_CHECK(std::is_sorted(factorization.begin(), factorization.end()));
    BOOST_CHECK_EQUAL(Defactorize(factorization), N);
    BOOST_CHECK(std::all_of(factorization.begin(), factorization.end(), IsPrime));
  }
}

BOOST_AUTO_TEST_CASE(primes_test) {
  using namespace lib;

  constexpr uint32 million = 1000 * 1000;
  auto primes = Sieve(million);
  for (auto i: range<uint32>(0, million)) {
    BOOST_CHECK_MESSAGE(primes[i] == IsPrime(i),
                        "Sieve and IsPrime not equal for i = " << i);
  }

  BOOST_CHECK(IsPrime(uint32_prime1));
  BOOST_CHECK(IsPrime(uint32_prime2));
  BOOST_CHECK(IsPrime(999999999999999989uLL)); // biggest prime smaller than 10^18
  BOOST_CHECK(!IsPrime(999999999999999991uLL));

#ifdef USE_INT128_TYPES
  BOOST_CHECK(IsPrime(uint64_prime1));
#endif

  // they are not twin primes
  BOOST_CHECK(!IsPrime(uint32_prime1 - 2));
  BOOST_CHECK(!IsPrime(uint32_prime2 - 2));
  BOOST_CHECK(!IsPrime(uint64_prime1 - 2));

  // some pseudoprimes
  BOOST_CHECK(!IsPrime(341)); // 11 * 32
  BOOST_CHECK(!IsPrime(60787)); // 89 * 683
  BOOST_CHECK(!IsPrime(29341)); // 13 * 37 * 61
  BOOST_CHECK(!IsPrime(201)); // 3 * 67
  BOOST_CHECK(!IsPrime(9746347772161)); // 7 * 11 * 13 * 17 * 19 * 31 * 37 * 41 * 641
  BOOST_CHECK(!IsPrime(340561)); // 13 * 17 * 23 * 67
}

BOOST_AUTO_TEST_CASE(is_primitive_root_test) {
  BOOST_CHECK_EQUAL(IsPrimitiveRoot(0, 2), false);
  BOOST_CHECK_EQUAL(IsPrimitiveRoot(1, 2), true);

  BOOST_CHECK_EQUAL(IsPrimitiveRoot(2, 7), false);
  BOOST_CHECK_EQUAL(IsPrimitiveRoot(3, 7), true);
  BOOST_CHECK_EQUAL(IsPrimitiveRoot(1, 7), false);

  BOOST_CHECK_EQUAL(IsPrimitiveRoot(2, 47), false);
  BOOST_CHECK_EQUAL(IsPrimitiveRoot(46, 47), false);
  BOOST_CHECK_EQUAL(IsPrimitiveRoot(15, 47), true);

  BOOST_CHECK_EQUAL(IsPrimitiveRoot(259, 0xFFFFFFFB), true);
  BOOST_CHECK_EQUAL(IsPrimitiveRoot(258, 0xFFFFFFFB), false);
}

BOOST_AUTO_TEST_CASE(inverse_test) {
  BOOST_CHECK_EQUAL(Inverse(1, 2), 1);
  BOOST_CHECK_EQUAL(Inverse(2, 7), 4);
  BOOST_CHECK_EQUAL(Inverse(7, 13), 2);
  BOOST_CHECK_EQUAL(Inverse(12, 13), 12);
  BOOST_CHECK_EQUAL(Inverse(1, 13), 1);
}

BOOST_AUTO_TEST_CASE(multiplicative_order_test) {
  BOOST_CHECK_EQUAL(MultiplicativeOrder(1, 2), 1);
  BOOST_CHECK_EQUAL(MultiplicativeOrder(2, 7), 3);
  BOOST_CHECK_EQUAL(MultiplicativeOrder(3, 7), 6);
  BOOST_CHECK_THROW(MultiplicativeOrder(0, 7), std::runtime_error);
  BOOST_CHECK_EQUAL(MultiplicativeOrder(1, 13), 1);
  BOOST_CHECK_EQUAL(MultiplicativeOrder(2, 13), 12);
  BOOST_CHECK_EQUAL(MultiplicativeOrder(3, 13), 3);
  BOOST_CHECK_EQUAL(MultiplicativeOrder(12, 13), 2);
  BOOST_CHECK_EQUAL(MultiplicativeOrder(10, 13), 6);

  BOOST_CHECK_EQUAL(MultiplicativeOrder(3, 0xFFFFFFFBu), 2147483645u);
  BOOST_CHECK_EQUAL(MultiplicativeOrder(2, 0xFFFFFFFBu), 0xFFFFFFFBu - 1u);
}

BOOST_AUTO_TEST_CASE(discrete_logarithm_test) {
  BOOST_CHECK_EQUAL(DiscreteLogarithm(1, 1, 2), 0);
  BOOST_CHECK_EQUAL(DiscreteLogarithm(2, 4, 7), 2);
  BOOST_CHECK_EQUAL(DiscreteLogarithm(3, 5, 7), 5);
  BOOST_CHECK_EQUAL(DiscreteLogarithm(3, 1431655764, 0xFFFFFFFB), 2147483644);
}

BOOST_AUTO_TEST_SUITE_END()
