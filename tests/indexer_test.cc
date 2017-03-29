// Jakub Staroń, 2016-2017

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "data_structures/indexer.h"


using namespace pcl;

BOOST_AUTO_TEST_SUITE(indexer_test)

BOOST_AUTO_TEST_CASE(indexer_test) {
  Indexer<std::string> indexer;
  BOOST_CHECK_THROW(indexer.getValue(0), std::out_of_range);

  BOOST_CHECK_EQUAL(indexer.getID("Ala"), 0);
  BOOST_CHECK_EQUAL(indexer.getValue(0), "Ala");
  BOOST_CHECK_EQUAL(indexer.getID("ma"), 1);
  BOOST_CHECK_EQUAL(indexer.getValue(0), "Ala");
  BOOST_CHECK_EQUAL(indexer.getValue(1), "ma");
  BOOST_CHECK_EQUAL(indexer.getID("Ala"), 0);
  BOOST_CHECK_EQUAL(indexer.getID("kota"), 2);
  BOOST_CHECK_EQUAL(indexer.getID("ma"), 1);
  BOOST_CHECK_EQUAL(indexer.getID("Ala"), 0);

  BOOST_CHECK_EQUAL(indexer.getValue(0), "Ala");
  BOOST_CHECK_EQUAL(indexer.getValue(1), "ma");
  BOOST_CHECK_EQUAL(indexer.getValue(2), "kota");

  BOOST_CHECK_THROW(indexer.getValue(3), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
