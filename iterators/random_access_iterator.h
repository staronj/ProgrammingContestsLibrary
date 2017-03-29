#pragma once
// Jakub Staroń, 2016-2017

#include "headers.h"
#include "iterators/is_iterator.h"

namespace pcl {

/**
 * Template class for creating random access iterators.
 *
 * Helper must:
 * * satisfy bidirectional iterator helper requirements
 * * have void advance(difference_type) function
 * * have difference_type difference(const Helper& other) const function
 * * have bool less(const Helper& other) const function
 */
template <typename Helper>
class random_access_iterator {
public:
  using value_type = typename Helper::value_type;
  using reference = typename Helper::reference;
  using pointer = typename Helper::pointer;
  using difference_type = typename Helper::difference_type;
  using iterator_category = std::random_access_iterator_tag;
  using self_type = random_access_iterator;

  random_access_iterator() = default;
  random_access_iterator(const random_access_iterator&) = default;
  random_access_iterator(random_access_iterator&) = default;
  random_access_iterator(random_access_iterator&&) = default;
  random_access_iterator& operator=(random_access_iterator&&) = default;
  random_access_iterator& operator=(const random_access_iterator&) = default;

  template <typename... Args>
  explicit random_access_iterator(Args&&... args):
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

  self_type& operator+=(difference_type n) {
    helper_.advance(n);
    return *this;
  }

  self_type& operator-=(difference_type n) {
    helper_.advance(-n);
    return *this;
  }

  friend self_type operator+(self_type lhs, difference_type rhs) {
    lhs.helper_.advance(rhs);
    return lhs;
  }

  friend self_type operator+(difference_type lhs, self_type rhs) {
    rhs.helper_.advance(lhs);
    return rhs;
  }

  friend self_type operator-(self_type lhs, difference_type rhs) {
    lhs.helper_.advance(-rhs);
    return lhs;
  }

  reference operator[](difference_type n) const {
    return *(*this + n);
  }

  friend difference_type operator-(const self_type& lhs, const self_type& rhs) {
    return lhs.helper_.difference(rhs.helper_);
  }

  friend bool operator==(const self_type& lhs, const self_type& rhs) {
    return lhs.helper_.equal(rhs.helper_);
  }

  friend bool operator!=(const self_type& lhs, const self_type& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const self_type& lhs, const self_type& rhs) {
    return lhs.helper_.less(rhs.helper_);
  }
  
  friend bool operator>(const self_type& lhs, const self_type& rhs) {
    return rhs < lhs;
  }

  friend bool operator<=(const self_type& lhs, const self_type& rhs) {
    return !(rhs < lhs);
  }
  
  friend bool operator>=(const self_type& lhs, const self_type& rhs) {
    return rhs <= lhs;
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
