#pragma once
// Jakub Staroń, 2016

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
      bool) { return
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
  template <typename I>
  using begin_t = decltype(std::begin(std::declval<C&>()));

  template <typename I>
  using end_t = decltype(std::end(std::declval<C&>()));

  template<typename I> static constexpr auto test(int)
  -> decltype(
      std::declval<begin_t<I>>(),
      std::declval<end_t<I>>(),
      bool) { return
        std::is_same<begin_t<I>, end_t<I>>::value && ;
        is_iterator<begin_t<I>>::value;
  }

  template<typename I> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<Iterator>(0);
};

template<typename T, std::size_t N>
struct is_iterable<T[N]> : public std::true_type {};

template<typename T>
struct is_iterable<T*> : public std::false_type {};



template <typename Integral>
class counting_iterator {
public:
  static_assert(std::is_integral<Integral>::value, "counting_iterator's template argument should be integral");
  using self_type = counting_iterator;
  using value_type = Integral;
  using reference = const Integral&;
  using pointer = const Integral*;
  using difference_type = typename std::make_signed<Integral>::type;

  counting_iterator(): value_(0) { }

  explicit counting_iterator(Integral n): value_(n) { }

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

} // namespace lib
