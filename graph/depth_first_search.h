#pragma once
// Jakub Staroń, 2016-2017

#include "graph/graph.h"
#include "numeric.h"

namespace pcl {
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
      if (!exited(v))
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
        on_exit(v);
        stack_.pop_back();
      }
      else {
        parent_[v] = parent;
        entered_[v] = true;
        on_enter(v);

        for (auto edge: graph_->edges_from(v)) {
          id_type u = edge.pass_from(v);
          if (!entered_[u])
            stack_.emplace_back(u, v);
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

  bool entered(id_type v) const {
    return entered_.at(v);
  }

  bool exited(id_type v) const {
    return exited_.at(v);
  }


  using callback_function_type = std::function<void(id_type)>;

  /**
   * Registers callback called on entering vertex.
   */
  void register_on_enter(callback_function_type on_enter) {
    on_enter_.push_back(std::move(on_enter));
  }

  /**
   * Registers callback called on exiting vertex.
   */
  void register_on_exit(callback_function_type on_exit) {
    on_exit_.push_back(std::move(on_exit));
  }

private:
  void on_enter(id_type v) {
    for (auto& obj: on_enter_)
      obj(v);
  }

  void on_exit(id_type v) {
    for (auto& obj: on_exit_)
      obj(v);
  }

  const Graph* graph_;
  size_type vertices_count_;
  bit_vector entered_;
  bit_vector exited_;
  std::vector<id_type> parent_;

  std::vector<std::pair<id_type, id_type>> stack_;

  std::vector<callback_function_type> on_enter_;
  std::vector<callback_function_type> on_exit_;
};

template <typename Graph>
constexpr id_type DepthFirstSearch<Graph>::invalid_vertex;

} // namespace graph
} // namespace pcl
