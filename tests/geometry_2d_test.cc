// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "geometry/2d/2d.h"

using namespace lib::geometry::_2d;

BOOST_AUTO_TEST_SUITE(geometry_2d_suite)

BOOST_AUTO_TEST_CASE(point_test) {
  point<int> a{1, 2};
  BOOST_CHECK_EQUAL(a.x, 1);
  BOOST_CHECK_EQUAL(a.y, 2);

  point<int> b{2, 4};
  BOOST_CHECK_EQUAL(a + b, (point<int>{3, 6}));
  BOOST_CHECK_EQUAL(a - b, (point<int>{-1, -2}));
  BOOST_CHECK_EQUAL(a * 2, (point<int>{2, 4}));
  BOOST_CHECK(a == a);
  BOOST_CHECK(a != b);
  BOOST_CHECK(!(a != a));
  BOOST_CHECK_EQUAL(ScalarProduct(a, b), 10);
  BOOST_CHECK_EQUAL(VectorProduct(a, b), 0);

  {
    std::istringstream stream("8 9");
    point<int> p;
    stream >> p;
    BOOST_CHECK_EQUAL(p, (point<int>{8, 9}));
  }
}

BOOST_AUTO_TEST_SUITE_END()
