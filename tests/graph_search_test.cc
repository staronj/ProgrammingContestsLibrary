// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "graph/depth_first_search.h"
#include "graph/breadth_first_search.h"
#include "iterators.h"
#include "io.h"

using namespace lib::graph;
using namespace lib;

struct SimpleGraphFixture {
  using graph_type = UndirectedGraph;
  SimpleGraphFixture() {
    for (auto i: range(0, 4))
      graph.add_vertex();

    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 0);
  }

  graph_type graph;
};

struct MediumGraphFixture {
  using graph_type = UndirectedGraph;
  MediumGraphFixture() {
    for (auto i: range(0, 6))
      graph.add_vertex();

    graph.add_edge(0, 1);
    graph.add_edge(1, 5);
    graph.add_edge(5, 3);
    graph.add_edge(3, 4);
    graph.add_edge(3, 0);
  }

  graph_type graph;
};

BOOST_AUTO_TEST_SUITE(depth_first_search_suite)


BOOST_AUTO_TEST_CASE(simple_graph_dfs) {
  using graph_type = SimpleGraphFixture::graph_type;
  SimpleGraphFixture fixture;
  DepthFirstSearch<graph_type> dfs(fixture.graph);

  std::vector<id_type> preorder;
  std::vector<id_type> postorder;
  dfs.register_on_enter([&preorder](id_type v) {
    preorder.push_back(v);
  });

  dfs.register_on_exit([&postorder](id_type v) {
    postorder.push_back(v);
  });

  dfs.run();

  std::vector<id_type> expected_parent = {0, 2, 3, 0};
  std::vector<id_type> expected_preorder = {0, 3, 2, 1};
  std::vector<id_type> expected_postorder = {1, 2, 3, 0};

  BOOST_CHECK(expected_parent == dfs.parents());
  BOOST_CHECK(expected_preorder == preorder);
  BOOST_CHECK(expected_postorder == postorder);
}

BOOST_AUTO_TEST_CASE(medium_graph_dfs) {
  using graph_type = MediumGraphFixture::graph_type;
  MediumGraphFixture fixture;
  DepthFirstSearch<graph_type> dfs(fixture.graph);

  std::vector<id_type> preorder;
  std::vector<id_type> postorder;
  dfs.register_on_enter([&preorder](id_type v) {
    preorder.push_back(v);
  });

  dfs.register_on_exit([&postorder](id_type v) {
    postorder.push_back(v);
  });

  dfs.run();

  std::vector<id_type> expected_parent = {0, 5, 2, 0, 3, 3};
  std::vector<id_type> expected_preorder = {0, 3, 4, 5, 1, 2};
  std::vector<id_type> expected_postorder = {4, 1, 5, 3, 0, 2};

  BOOST_CHECK(expected_parent == dfs.parents());
  BOOST_CHECK(expected_preorder == preorder);
  BOOST_CHECK(expected_postorder == postorder);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(breadth_first_search_suite)

BOOST_AUTO_TEST_CASE(simple_graph_bfs) {
  using graph_type = SimpleGraphFixture::graph_type;
  SimpleGraphFixture fixture;
  BreadthFirstSearch<graph_type> bfs(fixture.graph);

  std::vector<id_type> visit_order;
  bfs.register_on_visit([&visit_order](id_type v) {
    visit_order.push_back(v);
  });

  bfs.run_from(0);

  std::vector<id_type> expected_parent = {0, 0, 1, 0};
  std::vector<id_type> expected_visit_order = {0, 1, 3, 2};

  BOOST_CHECK(expected_parent == bfs.parents());
  BOOST_CHECK(expected_visit_order == visit_order);
}

BOOST_AUTO_TEST_CASE(medium_graph_bfs) {
  using graph_type = MediumGraphFixture::graph_type;
  using search_type = BreadthFirstSearch<graph_type>;
  MediumGraphFixture fixture;
  search_type bfs(fixture.graph);

  std::vector<id_type> visit_order;
  bfs.register_on_visit([&visit_order](id_type v) {
    visit_order.push_back(v);
  });

  bfs.run_from(0);

  std::vector<id_type> expected_parent = {0, 0, search_type::invalid_vertex, 0, 3, 1};
  std::vector<id_type> expected_visit_order = {0, 1, 3, 5, 4};

  BOOST_CHECK(expected_parent == bfs.parents());
  BOOST_CHECK(expected_visit_order == visit_order);
}

BOOST_AUTO_TEST_SUITE_END()
