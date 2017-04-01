// Jakub Staroń, 2016-2017

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "logger.h"

using namespace pcl::logging;

BOOST_AUTO_TEST_SUITE(logger_test)

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
    std::ostringstream stream;
    log_master().subscribe("test1", log_level::error, "", &stream);
    Logger logger("test1");
    logger.debug("%0", 1);
    BOOST_CHECK_EQUAL(stream.str(), "");
    logger.info("%0", 2);
    BOOST_CHECK_EQUAL(stream.str(), "");
    logger.error("%0", 3);
    BOOST_CHECK_EQUAL(stream.str(), "3\n");
  }

  {
    std::ostringstream stream1;
    std::ostringstream stream2;
    std::ostringstream stream3;
    log_master().subscribe("test2", log_level::debug, "", &stream1);
    log_master().subscribe("test2", log_level::info, "", &stream2);
    log_master().subscribe("test2", log_level::error, "", &stream3);

    Logger logger("test2");
    logger.error("%0", 1);
    logger.info("%0", 2);
    logger.debug("%0", 3);
    BOOST_CHECK_EQUAL(stream1.str(), "1\n2\n3\n");
    BOOST_CHECK_EQUAL(stream2.str(), "1\n2\n");
    BOOST_CHECK_EQUAL(stream3.str(), "1\n");
  }
}

BOOST_AUTO_TEST_SUITE_END()
