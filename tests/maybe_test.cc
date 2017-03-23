// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include "utils/maybe.h"
#include <boost/test/unit_test.hpp>

using namespace lib;

BOOST_AUTO_TEST_SUITE(maybe_suite)

BOOST_AUTO_TEST_CASE(empty_maybe_test) {
  Maybe<std::string> maybe;
  BOOST_CHECK(maybe.empty());
  BOOST_CHECK(maybe == Nothing);
  BOOST_CHECK(Nothing == maybe);
  BOOST_CHECK_THROW(maybe.get(), std::exception);
}

BOOST_AUTO_TEST_CASE(initialization_by_value_test) {
  Maybe<std::string> maybe("42");
  BOOST_CHECK(!maybe.empty());
  BOOST_CHECK(maybe != Nothing);
  BOOST_CHECK(Nothing != maybe);
  BOOST_CHECK_EQUAL(maybe.get(), "42");
}

BOOST_AUTO_TEST_CASE(initialization_by_maybe_test) {
  Maybe<std::string> maybe1("42");
  Maybe<std::string> maybe2(maybe1);
  BOOST_CHECK(!maybe1.empty());
  BOOST_CHECK(!maybe2.empty());
  BOOST_CHECK_EQUAL(maybe1.get(), "42");
  BOOST_CHECK_EQUAL(maybe2.get(), "42");
}

BOOST_AUTO_TEST_CASE(initialization_by_moved_maybe_test) {
  Maybe<std::string> maybe1("42");
  Maybe<std::string> maybe2(std::move(maybe1));
  BOOST_CHECK(maybe1.empty());
  BOOST_CHECK(!maybe2.empty());
  BOOST_CHECK_EQUAL(maybe2.get(), "42");
}

BOOST_AUTO_TEST_CASE(assign_value_test) {
  Maybe<std::string> maybe;
  maybe = "42";
  BOOST_CHECK(!maybe.empty());
  BOOST_CHECK_EQUAL(maybe.get(), "42");
}

BOOST_AUTO_TEST_CASE(assign_maybe_test) {
  Maybe<std::string> maybe1;
  Maybe<std::string> maybe2("42");
  maybe1 = maybe2;
  BOOST_CHECK(!maybe1.empty());
  BOOST_CHECK(!maybe2.empty());
  BOOST_CHECK_EQUAL(maybe1.get(), "42");
  BOOST_CHECK_EQUAL(maybe2.get(), "42");
}

BOOST_AUTO_TEST_CASE(assign_moved_maybe_test) {
  Maybe<std::string> maybe1;
  Maybe<std::string> maybe2("42");
  maybe1 = std::move(maybe2);
  BOOST_CHECK(!maybe1.empty());
  BOOST_CHECK(maybe2.empty());
  BOOST_CHECK_EQUAL(maybe1.get(), "42"); 
}

BOOST_AUTO_TEST_CASE(construct_from_nothing) {
    Maybe<std::string> maybe(Nothing);
    BOOST_CHECK(maybe.empty());
}

BOOST_AUTO_TEST_CASE(construct_from_nothing_in_lambda) {
    auto maybePositive = [](int n) {
        return (n < 0)? Nothing : Just(n);
    };
    auto result1 = maybePositive(-1);
    BOOST_CHECK(result1.empty());
    auto result2 = maybePositive(2);
    BOOST_CHECK(!result2.empty());
    BOOST_CHECK_EQUAL(result2.get(), 2);
}

BOOST_AUTO_TEST_CASE(equal) {
  BOOST_CHECK_EQUAL(Maybe<int>(Nothing) == Maybe<int>(Nothing), true);
  BOOST_CHECK_EQUAL(Maybe<int>(Nothing) != Maybe<int>(Nothing), false);

  BOOST_CHECK_EQUAL(Maybe<int>(1) == Maybe<int>(Nothing), false);
  BOOST_CHECK_EQUAL(Maybe<int>(1) != Maybe<int>(Nothing), true);

  BOOST_CHECK_EQUAL(Maybe<int>(1) == Maybe<int>(1), true);
  BOOST_CHECK_EQUAL(Maybe<int>(1) != Maybe<int>(1), false);

  BOOST_CHECK_EQUAL(Just(1) == Just(1), true);
  BOOST_CHECK_EQUAL(Just(1) != Just(1), false);

  BOOST_CHECK_EQUAL(Just(1) == Just(2), false);
  BOOST_CHECK_EQUAL(Just(1) != Just(2), true);
}

BOOST_AUTO_TEST_SUITE_END()
