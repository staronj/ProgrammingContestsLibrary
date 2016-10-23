// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "graph/graph.h"
#include "graph/depth_first_search.h"
#include "graph/breadth_first_search.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 5;
constexpr size_t iterations = 5;

class RandomGraphFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {100 * 1000, 10},
        {1000 * 1000, 3}
    };
  }

  void setUp(int64_t experimentValue) override
  {
    for (auto i: range<uint32>(0, experimentValue))
      graph.add_vertex();
    using lib::random;
    for (auto i: range<uint32>(0, 3 * experimentValue)) {
      graph.add_edge(random() % experimentValue, random() % experimentValue);
    }
  }

  using graph_type = graph::UndirectedGraph;
  graph_type graph;
};


BASELINE_F(GraphSearch, DFS, RandomGraphFixture, samples, iterations)
{
  graph::DepthFirstSearch<graph_type> dfs(graph);
  dfs.run();
  celero::DoNotOptimizeAway(dfs.parents()[123]);
}

BENCHMARK_F(GraphSearch, BFS, RandomGraphFixture, samples, iterations)
{
  graph::BreadthFirstSearch<graph_type> bfs(graph);
  bfs.run_from(0);
  celero::DoNotOptimizeAway(bfs.parents()[123]);
}

BENCHMARK_F(GraphSearch, DFSWithCallback, RandomGraphFixture, samples, iterations)
{
  graph::DepthFirstSearch<graph_type> dfs(graph);
  std::vector<graph::id_type> preorder;
  std::vector<graph::id_type> postorder;
  dfs.register_on_enter([&preorder](graph::id_type v){
    preorder.push_back(v);
  });

  dfs.register_on_enter([&postorder](graph::id_type v){
    postorder.push_back(v);
  });

  dfs.run();
  celero::DoNotOptimizeAway(dfs.parents()[123]);
  celero::DoNotOptimizeAway(preorder.back());
  celero::DoNotOptimizeAway(postorder.back());
}

BENCHMARK_F(GraphSearch, BFSWithCallback, RandomGraphFixture, samples, iterations)
{
  graph::BreadthFirstSearch<graph_type> bfs(graph);
  std::vector<graph::id_type> visit_order;
  bfs.register_on_visit([&visit_order](graph::id_type v){
    visit_order.push_back(v);
  });
  bfs.run_from(0);
  celero::DoNotOptimizeAway(bfs.parents()[123]);
  celero::DoNotOptimizeAway(visit_order.back());
}
