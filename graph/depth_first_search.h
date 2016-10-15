#pragma once
// Jakub Staroń, 2016

#include "graph/graph.h"
#include "numeric.h"

namespace lib {
namespace graph {

template <typename Graph>
class DepthFirstSearch {
public:
  static constexpr id_type invalid_vertex = uint32_infinity;

  DepthFirstSearch(const Graph& graph):
      graph_(&graph), vertices_count_(graph.vertices_count()) {
    entered_.assign(vertices_count_, false);
    exited_.assign(vertices_count_, false);
    parent_.assign(vertices_count_, invalid_vertex);
  }

  void run() {
    for (auto v: range<id_type>(0, vertices_count_)) {
      if (!exited_[v])
        run_from(v);
    }
  }

  void run_from(id_type start) {
    if (exited_.at(start))
      return;

    stack_.emplace_back(start, start);
    while (!stack_.empty()) {
      id_type v, parent;
      std::tie(v, parent) = stack_.back();
      if (exited_[v]) {
        stack_.pop_back();
      }
      else if (entered_[v]) {
        exited_[v] = true;
        exit(v);
        stack_.pop_back();
      }
      else {
        parent_[v] = parent;
        entered_[v] = true;
        enter(v);

        for (auto edge: graph_->edges_from(v)) {
          id_type u = edge.pass_from(v);
          if (!entered_[u])
            stack_.emplace_back(u, v);
        }
      }
    }
  }

  void enter(id_type v) { }
  void exit(id_type v) { }

private:
  const Graph* graph_;
  size_type vertices_count_;
  bit_vector entered_;
  bit_vector exited_;
  std::vector<id_type> parent_;

  std::vector<std::pair<id_type, id_type>> stack_;
};

template <typename Graph>
static constexpr id_type DepthFirstSearch<Graph>::invalid_vertex;

} // namespace graph
} // namespace lib
