// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/indexer.h"


using namespace lib;

BOOST_AUTO_TEST_SUITE(indexer_test)

BOOST_AUTO_TEST_CASE(indexer_test) {
  Indexer<std::string> indexer;
  BOOST_CHECK_THROW(indexer.get_value(0), std::out_of_range);

  BOOST_CHECK_EQUAL(indexer.get_id("Ala"), 0);
  BOOST_CHECK_EQUAL(indexer.get_value(0), "Ala");
  BOOST_CHECK_EQUAL(indexer.get_id("ma"), 1);
  BOOST_CHECK_EQUAL(indexer.get_value(0), "Ala");
  BOOST_CHECK_EQUAL(indexer.get_value(1), "ma");
  BOOST_CHECK_EQUAL(indexer.get_id("Ala"), 0);
  BOOST_CHECK_EQUAL(indexer.get_id("kota"), 2);
  BOOST_CHECK_EQUAL(indexer.get_id("ma"), 1);
  BOOST_CHECK_EQUAL(indexer.get_id("Ala"), 0);

  BOOST_CHECK_EQUAL(indexer.get_value(0), "Ala");
  BOOST_CHECK_EQUAL(indexer.get_value(1), "ma");
  BOOST_CHECK_EQUAL(indexer.get_value(2), "kota");

  BOOST_CHECK_THROW(indexer.get_value(3), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
