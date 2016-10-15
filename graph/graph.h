#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"

namespace lib {
namespace graph {

using id_type = uint32;
using size_type = uint32;
using empty_type = std::tuple<>;

template <typename VertexData>
class Vertex: public VertexData {
public:
  Vertex(id_type id, VertexData data):
      VertexData(std::move(data)), id_(id) { }

  id_type id() const {
    return id_;
  }

private:
  id_type id_;
};


template <template <typename Graph, typename EdgeData> class Edge, typename EdgeData = empty_type, typename VertexData = empty_type>
class StaticGraph {
public:
  using self_type = StaticGraph;
  using edge_type = Edge<StaticGraph, EdgeData>;
  using vertex_type = Vertex<VertexData>;
  using edge_reference = const edge_type&;
  using vertex_reference = const vertex_type&;
  friend edge_type;

  StaticGraph(size_type n = 0, VertexData data = VertexData()) {
    for (auto i: range<size_type>(0, n))
      add_vertex(data);
  }

  void add_vertex(VertexData data = VertexData()) {
    edges_from_.emplace_back();
    id_type id = vertices_count();
    vertices_.emplace_back(id, data);
  }
  
  void add_edge(id_type from, id_type to, EdgeData data = EdgeData()) {
    id_type id = edges_count();
    edges_.emplace_back(id, from, to, data);
    edges_.back().register_in(this);
  }

  auto edges_from(id_type v) const ->
  iterator_range<indirect_iterator<typename std::vector<edge_type>::const_iterator, typename std::vector<id_type>::const_iterator>> {
      auto begin = make_indirect_iterator(edges_.begin(), edges_from_.at(v).begin());
      auto end = make_indirect_iterator(edges_.begin(), edges_from_.at(v).end());
      return make_range(begin, end);
  }

  size_type degree_of(id_type id) {
    return size_type(edges_from_.at(id).size());
  }

  edge_reference edge(id_type id) {
    return edges_.at(id);
  }

  vertex_reference vertex(id_type id) {
    return vertices_.at(id);
  }

  size_type vertices_count() const {
    return size_type(vertices_.size());
  }

  size_type edges_count() const {
    return size_type(edges_.size());
  }

private:
  void register_edge(id_type edge_id, id_type vertex_id) {
    edges_from_.at(vertex_id).push_back(edge_id);
  }

  std::vector<std::vector<id_type>> edges_from_;
  std::vector<edge_type> edges_;
  std::vector<vertex_type> vertices_;
};


template <typename Graph, typename EdgeData>
class UndirectedEdge: public EdgeData {
public:
  friend Graph;

  UndirectedEdge(id_type id, id_type first, id_type second, EdgeData data):
      id_(id), first_(first), second_(second), EdgeData(std::move(data)) { }

  id_type pass_from(id_type v) const {
    return (v == first_)? second_ : first_;
  }

private:
  void register_in(Graph* graph) {
    graph->register_edge(id_, first_);
    graph->register_edge(id_, second_);
  }

  id_type id_;
  id_type first_;
  id_type second_;
};

using UndirectedGraph = StaticGraph<UndirectedEdge>;

} // namespace graph
} // namespace lib
