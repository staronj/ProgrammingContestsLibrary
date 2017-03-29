#pragma once
// Jakub Staroń, 2016-2017

#include "headers.h"

namespace pcl {

/**
 * Power tree.
 *
 * Operations:
 * * querying for sum in range
 * * adding (possibly negative) value to given position.
 *
 * Memory O(n), query and insert in O(log n)
 */
template<class ValueType>
class PowerTree {
public:
  using value_type = ValueType;
  using size_type = uint32;
  using index_type = int32;
  using ptr = std::shared_ptr<PowerTree>;

  PowerTree(size_type n) :
      load_(n, value_type(0)) { }

  /**
   * Adds value on given position.
   */
  void insert(index_type n, value_type value) {
    n++;
    while (n <= size()) {
      load_[n - 1] += value;
      n += step(n);
    }
  }

  /**
   * Returns sum of values on range [first, last].
   */
  value_type query(index_type first, index_type last) {
    auto result = prefixQuery(last) - prefixQuery(first - 1);
    return result;
  }

  /**
   * Returns sum of values on range [0, n].
   *
   * Can be slightly faster than query.
   */
  value_type prefixQuery(index_type n) {
    n++;
    value_type result = 0;
    while (n > 0) {
      result += load_[n - 1];
      n -= step(n);
    }
    return result;
  }

  /**
   * Returns number of elements in power tree.
   */
  size_type size() const {
    return size_type(load_.size());
  }

private:
  static index_type step(index_type n) {
    return ((n ^ (n - 1)) + 1) / 2;
  }

  std::vector<value_type> load_;
};

} // namespace pcl
