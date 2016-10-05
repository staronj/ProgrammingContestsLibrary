#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "operators.h"

namespace lib {

template<typename Iterator>
struct is_iterator {
private:
  template<typename I> static constexpr auto test(int)
  -> decltype(
      *std::declval<const I>(),
      std::declval<const I>() == std::declval<const I>(),
      std::declval<const I>() != std::declval<const I>(),
      ++ std::declval<I&>(),
      std::declval<I&>() ++,
      std::iterator_traits<I>(),
      bool()) { return
        std::is_destructible<I>::value &&
        std::is_copy_constructible<I>::value &&
        std::is_copy_assignable<I>::value;
  }

  template<typename I> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<Iterator>(0);
};

template<typename T>
struct is_iterable {
  template <typename C>
  using begin_t = decltype(std::begin(std::declval<C&>()));

  template <typename C>
  using end_t = decltype(std::end(std::declval<C&>()));

  template<typename I> static constexpr auto test(int)
  -> decltype(
      std::declval<begin_t<I>>(),
      std::declval<end_t<I>>(),
      bool()) { return
        std::is_same<begin_t<I>, end_t<I>>::value &&
        is_iterator<begin_t<I>>::value;
  }

  template<typename I> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<T>(0);
};

template <typename Integral>
class counting_iterator {
public:
  static_assert(std::is_integral<Integral>::value, "counting_iterator's template argument should be integral");
  using self_type = counting_iterator;
  using value_type = Integral;
  using reference = const Integral&;
  using pointer = const Integral*;
  using difference_type = typename std::make_signed<Integral>::type;
  using iterator_category = std::random_access_iterator_tag;

  counting_iterator(): value_(0) { }

  explicit counting_iterator(Integral n): value_(n) { }

  reference operator*() const {
    return value_;
  }

  self_type& operator++() {
    ++value_;
    return *this;
  }

  self_type& operator--() {
    --value_;
    return *this;
  }

  self_type& operator+=(difference_type n) {
    value_ += n;
    return *this;
  }

  friend self_type operator+(const self_type& obj, difference_type n) {
    return self_type(obj.value_ + n);
  }

  friend difference_type operator-(const self_type& lhs, const self_type& rhs) {
    return lhs.value_ - rhs.value_;
  }

  value_type operator[](difference_type n) {
    return value_type(value_ + n);
  }

  friend difference_type operator<(const self_type& lhs, const self_type& rhs) {
    return lhs.value_ < rhs.value_;
  }

  friend difference_type operator==(const self_type& lhs, const self_type& rhs) {
    return lhs.value_ == rhs.value_;
  }

private:
  Integral value_;
};

template <typename Integral>
auto make_counting_iterator(Integral n) -> counting_iterator<Integral> {
  return counting_iterator<Integral>(n);
}

template <typename Integral>
class reverse_counting_iterator {
public:
  static_assert(std::is_integral<Integral>::value, "reverse_counting_iterator's template argument should be integral");
  using self_type = reverse_counting_iterator;
  using value_type = Integral;
  using reference = const Integral&;
  using pointer = const Integral*;
  using difference_type = typename std::make_signed<Integral>::type;
  using iterator_category = std::random_access_iterator_tag;

  reverse_counting_iterator(): value_(0) { }

  explicit reverse_counting_iterator(Integral n): value_(n) { }

  reference operator*() const {
    return value_;
  }

  self_type& operator++() {
    --value_;
    return *this;
  }

  self_type& operator--() {
    ++value_;
    return *this;
  }

  self_type& operator+=(difference_type n) {
    value_ -= n;
    return *this;
  }

  friend self_type operator+(const self_type& obj, difference_type n) {
    return self_type(obj.value_ - n);
  }

  friend difference_type operator-(const self_type& lhs, const self_type& rhs) {
    return rhs.value_ - lhs.value_;
  }

  value_type operator[](difference_type n) {
    return value_type(value_ - n);
  }

  friend difference_type operator<(const self_type& lhs, const self_type& rhs) {
    return rhs.value_ < lhs.value_;
  }

  friend difference_type operator==(const self_type& lhs, const self_type& rhs) {
    return lhs.value_ == rhs.value_;
  }

private:
  Integral value_;
};

template <typename Integral>
auto make_reverse_counting_iterator(Integral n) -> reverse_counting_iterator<Integral> {
  return reverse_counting_iterator<Integral>(n);
}

template <typename Iterator>
class iterator_range {
public:
  using iterator_type = Iterator;
  using reference = const Iterator&;

  static_assert(is_iterator<Iterator>::value, "iterator_range's template argument should be iterator!");

  explicit iterator_range(Iterator begin, Iterator end):
      begin_(std::move(begin)), end_(std::move(end)) { }

  reference begin() const {
    return begin_;
  }

  reference end() const {
    return end_;
  }

private:
  iterator_type begin_;
  iterator_type end_;
};

template <typename Iterator>
auto make_range(Iterator begin, Iterator end) -> iterator_range<Iterator> {
  return iterator_range<Iterator>(std::move(begin), std::move(end));
}

template <typename Integral>
auto range(Integral begin, Integral end) ->
typename std::enable_if<std::is_integral<Integral>::value, iterator_range<counting_iterator<Integral>>>::type {
  return make_range(make_counting_iterator(begin), make_counting_iterator(end));
}

template <typename Integral>
auto rrange(Integral begin, Integral end) ->
typename std::enable_if<std::is_integral<Integral>::value, iterator_range<reverse_counting_iterator<Integral>>>::type {
  return make_range(make_reverse_counting_iterator(end - 1), make_reverse_counting_iterator(begin - 1));
}


} // namespace lib
