// Jakub Staroń, 2016-2017

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "graph/graph.h"

using namespace pcl::graph;

BOOST_AUTO_TEST_SUITE(graph)


BOOST_AUTO_TEST_CASE(undirected_graph_test) {
  {
    UndirectedGraph graph;
    BOOST_CHECK_EQUAL(graph.vertices_count(), 0);
    BOOST_CHECK_EQUAL(graph.edges_count(), 0);
  }

  {
    UndirectedGraph graph(3);
    BOOST_CHECK_EQUAL(graph.vertices_count(), 3);
    BOOST_CHECK_EQUAL(graph.edges_count(), 0);

    graph.add_edge(0, 1);
    graph.add_edge(0, 2);

    BOOST_CHECK_EQUAL(graph.vertices_count(), 3);
    BOOST_CHECK_EQUAL(graph.edges_count(), 2);

    std::vector<id_type> vertices;
    for (const auto& edge: graph.edges_from(0)) {
      vertices.push_back(edge.pass_from(0));
    }
    std::vector<id_type> expected_vertices = {1, 2};
    BOOST_CHECK(vertices == expected_vertices);
  }
}


BOOST_AUTO_TEST_SUITE_END()
