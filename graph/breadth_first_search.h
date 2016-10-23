#pragma once
// Jakub Staroń, 2016

#include "graph/graph.h"
#include "numeric.h"
#include "io.h"

namespace lib {
namespace graph {

template <typename Graph>
class BreadthFirstSearch {
public:
  using distance_type = uint32;
  static constexpr id_type invalid_vertex = uint32_infinity;
  static constexpr distance_type invalid_distance = uint32_infinity;

  BreadthFirstSearch(const Graph& graph):
      graph_(&graph), vertices_count_(graph.vertices_count()) {
    parent_.assign(vertices_count_, invalid_vertex);
    distance_.assign(vertices_count_, invalid_distance);
  }

  void run_from(id_type start) {
    if (visited(start))
      return;

    std::queue<id_type> Q;
    distance_[start] = 0;
    parent_[start] = start;
    Q.push(start);
    on_visit(start);

    while (!Q.empty()) {
      id_type v = Q.front();
      Q.pop();

      for (auto edge: graph_->edges_from(v)) {
        id_type u = edge.pass_from(v);
        if (distance_[u] == invalid_distance) {
          distance_[u] = distance_[v] + 1;
          parent_[u] = v;
          Q.push(u);
          on_visit(u);
        }
      }
    }
  }

  id_type parent(id_type v) const {
    return parent_.at(v);
  }

  const std::vector<id_type>& parents() const {
    return parent_;
  }

  bool visited(id_type v) const {
    return distance_.at(v) != invalid_distance;
  }

  distance_type distance(id_type v) const {
    return distance_.at(v);
  }

  const std::vector<distance_type>& distances() const {
    return distance_;
  }

  using callback_function_type = std::function<void(id_type)>;

  /**
   * Registers callback called on visiting vertex.
   */
  void register_on_visit(callback_function_type on_visit) {
    on_visit_.push_back(std::move(on_visit));
  }

private:
  void on_visit(id_type v) {
    for (auto& obj: on_visit_)
      obj(v);
  }

  const Graph* graph_;
  distance_type vertices_count_;
  std::vector<id_type> parent_;
  std::vector<distance_type> distance_;
  std::vector<callback_function_type> on_visit_;
};

template <typename Graph>
constexpr id_type BreadthFirstSearch<Graph>::invalid_vertex;

template <typename Graph>
constexpr typename BreadthFirstSearch<Graph>::distance_type BreadthFirstSearch<Graph>::invalid_distance;

} // namespace graph
} // namespace lib
