#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"

namespace lib {

class FindAndUnion {
public:
  using ptr = std::shared_ptr<FindAndUnion>;
  using id_type = uint32;

  FindAndUnion(uint32 size):
      rank_(size, 0),
      parent_(make_counting_iterator(0u), make_counting_iterator(size)) { }

  id_type find_root(id_type v) {
    if(v != parent_[v])
      parent_[v] = find_root(parent_[v]);
    return parent_[v];
  }

  /*
   * Returns true if union was performed and false if
   * u and v defines the same set.
   */
  bool union_sets(id_type u, id_type v) {
    u = find_root(u);
    v = find_root(v);

    if (u == v)
      return false;

    if (rank_[u] > rank_[v])
      parent_[v] = u;
    else
      parent_[u] = v;

    if (rank_[u] == rank_[v])
      rank_[v]++;

    return true;
  }

private:
  std::vector<uint8> rank_;
  std::vector<id_type> parent_;
};

} // namespace lib
