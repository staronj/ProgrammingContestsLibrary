#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators/is_iterator.h"

namespace lib {

/**
 * Template class for creating input iterators.
 *
 * Helper must:
 * * be destructible
 * * be copy assignable
 * * be copy constructible
 * * have value_type type alias
 * * have reference type alias
 * * have pointer type alias
 * * have difference_type type alias
 * * have void next() funciton
 * * have reference value() const function
 * * have pointer ptr() const function
 * * have bool equal(const Helper& other) const function
 *
 */
template <typename Helper>
class input_iterator {
public:
  using value_type = typename Helper::value_type;
  using reference = typename Helper::reference;
  using pointer = typename Helper::pointer;
  using difference_type = typename Helper::difference_type;
  using iterator_category = std::input_iterator_tag;
  using self_type = input_iterator;

  input_iterator() = default;

  template <typename... Args>
  explicit input_iterator(Args&&... args):
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

  friend bool operator==(const self_type& lhs, const self_type& rhs) {
    return lhs.helper_.equal(rhs.helper_);
  }

  friend bool operator!=(const self_type& lhs, const self_type& rhs) {
    return !(lhs == rhs);
  }

private:
  Helper helper_;
};

} // namespace lib