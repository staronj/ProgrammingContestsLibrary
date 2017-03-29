#pragma once
// Jakub Staroń, 2016-2017

#include "headers.h"
#include "iterators/is_iterator.h"

namespace pcl {

/**
 * Template class for creating bidirectional iterators.
 *
 * Helper must:
 * * satisfy forward iterator helper requirements
 * * have void prev() function
 */
template <typename Helper>
class bidirectional_iterator {
public:
  using value_type = typename Helper::value_type;
  using reference = typename Helper::reference;
  using pointer = typename Helper::pointer;
  using difference_type = typename Helper::difference_type;
  using iterator_category = std::bidirectional_iterator_tag;
  using self_type = bidirectional_iterator;

  bidirectional_iterator() = default;

  template <typename... Args>
  explicit bidirectional_iterator(Args&&... args):
      helper_(std::forward<Args>(args)...) { }

  reference operator*() const { return helper_.value(); }
  pointer operator->() const { return helper_.ptr(); }

  self_type& operator++() {
    helper_.next();
    return *this;
  }

  self_type operator++(int) {
    auto it = *this;
    this->operator++();
    return it;
  }

  self_type& operator--() {
    helper_.prev();
    return *this;
  }

  self_type operator--(int) {
    auto it = *this;
    this->operator--();
    return it;
  }

  friend bool operator==(const self_type& lhs, const self_type& rhs) {
    return lhs.helper_.equal(rhs.helper_);
  }

  friend bool operator!=(const self_type& lhs, const self_type& rhs) {
    return !(lhs == rhs);
  }

  const Helper& getHelper() const {
    return helper_;
  }

  Helper& getHelper() {
    return helper_;
  }

private:
  Helper helper_;
};

} // namespace pcl
