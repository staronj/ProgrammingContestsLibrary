#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "operators.h"
#include "iterators/is_iterator.h"
#include "iterators/input_iterator.h"
#include "iterators/random_access_iterator.h"
#include "utils/maybe.h"

namespace lib {

/**
 * type_traits-like predicate. Equal to true_type if argument is iterable.
 *
 * Example:
 * <pre>
 * static_assert(is_iterable<std::vector<int>>::value, "vector should be iterable!");
 * </pre>
 */
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

namespace detail {

template <typename Integral>
struct counting_iterator_helper {
  static_assert(std::is_integral<Integral>::value, "counting_iterator's template argument should be integral");
  using value_type = Integral;
  using reference = value_type;
  using pointer = const value_type*;
  using difference_type = typename std::make_signed<Integral>::type;
  using self_type = counting_iterator_helper;

  counting_iterator_helper(): value_() { }
  explicit counting_iterator_helper(Integral n): value_(n) { }

  void next() { ++value_; }
  void prev() { --value_; }
  reference value() const { return value_; }
  pointer ptr() const { return &value_; }

  bool equal(const self_type& other) const
    { return value_ == other.value_; }

  void advance(difference_type diff)
    { value_ += diff; }

  difference_type difference(const self_type& other) const
    { return value_ - other.value_; }

  bool less(const self_type& other) const
    { return value_ < other.value_; }

private:
  Integral value_;
};

template <typename Integral>
struct reverse_counting_iterator_helper {
  static_assert(std::is_integral<Integral>::value, "reverse_counting_iterator's template argument should be integral");
  using value_type = Integral;
  using reference = value_type;
  using pointer = const value_type*;
  using difference_type = typename std::make_signed<Integral>::type;
  using self_type = reverse_counting_iterator_helper;

  reverse_counting_iterator_helper(): value_() { }
  reverse_counting_iterator_helper(Integral n): value_(n) { }

  void next() { --value_; }
  void prev() { ++value_; }
  reference value() const { return value_; }
  pointer ptr() const { return &value_; }

  bool equal(const self_type& other) const
  { return value_ == other.value_; }

  void advance(difference_type diff)
  { value_ -= diff; }

  difference_type difference(const self_type& other) const
  { return other.value_ - value_; }

  bool less(const self_type& other) const
  { return value_ > other.value_; }

private:
  Integral value_;
};

} // namespace detail

/**
 * Random access iterator for iterating over integral type.
 *
 * Example:
 * <pre>
 * std::vector<int> v(counting_iterator<int>(0), counting_iterator<int>(5));
 * </pre>
 *
 * Produces v with elements {0, 1, 2, 3, 4}
 */
template <typename Integral>
using counting_iterator = random_access_iterator<detail::counting_iterator_helper<Integral>>;

/**
 * Helper function for building counting_iterator.
 *
 * Example:
 * <pre>
 * std::vector<int> v(make_counting_iterator(0), make_counting_iterator(5));
 * </pre>
 *
 * Produces v with elements {0, 1, 2, 3, 4}
 */
template <typename Integral>
auto make_counting_iterator(Integral n) -> counting_iterator<Integral> {
  return counting_iterator<Integral>(n);
}

/**
 * Printing operator for counting iterator.
 */
template <typename T>
std::ostream& operator<<(std::ostream& stream, counting_iterator<T> it) {
  return stream << *it;
}

/**
 * Random access iterator for iterating over integral type in reversed order.
 *
 * Example:
 * <pre>
 * std::vector<int> v(reverse_counting_iterator<int>(4), reverse_counting_iterator<int>(-1));
 * </pre>
 *
 * Produces v with elements {4, 3, 2, 1, 0}
 */
template <typename Integral>
using reverse_counting_iterator = random_access_iterator<detail::reverse_counting_iterator_helper<Integral>>;


/**
 * Helper function for building reverse_counting_iterator.
 *
 * Example:
 * <pre>
 * std::vector<int> v(make_reverse_counting_iterator(4), make_reverse_counting_iterator(-1));
 * </pre>
 *
 * Produces v with elements {4, 3, 2, 1, 0}
 */
template <typename Integral>
auto make_reverse_counting_iterator(Integral n) -> reverse_counting_iterator<Integral> {
  return reverse_counting_iterator<Integral>(n);
}

/**
 * Helper class for encapsulating iterator pair as iterable.
 *
 * Example:
 * <pre>
 * void foo(Iterator begin, Iterator end) {
 *    for (const auto& elem: iterator_range<Iterator>(begin, end)) {
 *      ...
 *    }
 * }
 * </pre>
 */
template <typename Iterator>
class iterator_range {
public:
  static_assert(is_iterator<Iterator>::value, "iterator_range's template argument should be iterator!");

  using iterator_type = Iterator;
  using size_type = typename std::iterator_traits<Iterator>::difference_type;

  iterator_range() = default;

  explicit iterator_range(Iterator begin, Iterator end):
      begin_(std::move(begin)), end_(std::move(end)) { }

  iterator_type begin() const {
    return begin_;
  }

  iterator_type end() const {
    return end_;
  }

  bool empty() const {
    return begin_ == end_;
  }

  size_type size() const {
    return std::distance(begin_, end_);
  }

private:
  iterator_type begin_;
  iterator_type end_;
};

/**
 * Helper function for encapsulating iterator pair as iterable.
 *
 * Example:
 * <pre>
 * void foo(Iterator begin, Iterator end) {
 *    for (const auto& elem: make_range(begin, end)) {
 *      ...
 *    }
 * }
 * </pre>
 */
template <typename Iterator>
auto make_range(Iterator begin, Iterator end) -> iterator_range<Iterator> {
  return iterator_range<Iterator>(std::move(begin), std::move(end));
}

/**
 * Python-like range function.
 *
 * Example:
 * <pre>
 * for (auto i: range(0, 5)) {
 *    std::cout << i << ' ';
 * }
 * </pre>
 * Prints: 0 1 2 3 4
 *
 * You can also explicitly specify the type:
 * <pre>
 * for (auto i: range<uint32>(0, 5)) {
 *    std::cout << i << ' ';
 * }
 * </pre>
 */
template <typename Integral>
auto range(Integral begin, Integral end) ->
typename std::enable_if<std::is_integral<Integral>::value, iterator_range<counting_iterator<Integral>>>::type {
  return make_range(make_counting_iterator(begin), make_counting_iterator(end));
}

/**
 * Python-like function for iterating over reversed range.
 *
 * Example:
 * <pre>
 * for (auto i: rrange(0, 5)) {
 *    std::cout << i << ' ';
 * }
 * </pre>
 * Prints: 4 3 2 1 0
 *
 * You can also explicitly specify the type:
 * <pre>
 * for (auto i: rrange<uint32>(0, 5)) {
 *    std::cout << i << ' ';
 * }
 * </pre>
 */
template <typename Integral>
auto rrange(Integral begin, Integral end) ->
typename std::enable_if<std::is_integral<Integral>::value, iterator_range<reverse_counting_iterator<Integral>>>::type {
  return make_range(make_reverse_counting_iterator(end - 1), make_reverse_counting_iterator(begin - 1));
}

/**
 * Iterator performing transformation on values.
 *
 * Example:
 * <pre>
 * std::vector<int> v = {1, 2, 3};
 * using iterator = std::vector<int>::iterator;
 * using mapping_it = mapping_iterator<iterator, std::function<int(int)>>;
 * mapping_it it(v.begin(), [](int n) { return 2*n; });
 * mapping_it end(v.end());
 *
 * for (const auto& elem: make_range(it, end)) {
 *   std::cout << elem << ' ';
 * }
 * </pre>
 * Prints 2 4 6
 */
template <typename Iterator, typename Mapper>
class mapping_iterator {
public:
  using self_type = mapping_iterator;
  using underlying_value_type = decltype(*std::declval<Iterator>());
  using value_type = decltype(std::declval<Mapper>()(std::declval<underlying_value_type>()));
  using reference = value_type;
  using pointer = typename std::remove_reference<value_type>::type*;
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

  mapping_iterator(const Mapper& mapper = Mapper()):
      iterator_(), mapper_(mapper) { }

  explicit mapping_iterator(Iterator iterator, const Mapper& mapper = Mapper()):
      iterator_(iterator), mapper_(mapper) { }

  reference operator*() const {
    return mapper_(*iterator_);
  }

  self_type& operator++() {
    ++iterator_;
    return *this;
  }

  self_type& operator--() {
    --iterator_;
    return *this;
  }

  self_type& operator+=(difference_type n) {
    iterator_ += n;
    return *this;
  }

  friend self_type operator+(const self_type& obj, difference_type n) {
    return self_type(obj.iterator_ + n, obj.mapper_);
  }

  friend difference_type operator-(const self_type& lhs, const self_type& rhs) {
    return lhs.iterator_ - rhs.iterator_;
  }

  value_type operator[](difference_type n) {
    return *(*this + n);
  }

  friend bool operator<(const self_type& lhs, const self_type& rhs) {
    return lhs.iterator_ < rhs.iterator_;
  }

  friend bool operator==(const self_type& lhs, const self_type& rhs) {
    return lhs.iterator_ == rhs.iterator_;
  }

private:
  Iterator iterator_;
  Mapper mapper_;
};

namespace detail {

template <typename ValuesIterator>
struct IndirectMapper {
  static_assert(is_iterator<ValuesIterator>::value, "first indirect_iterator's template argument must be iterator.");
  static_assert(
      std::is_same<typename std::iterator_traits<ValuesIterator>::iterator_category, std::random_access_iterator_tag>::value,
      "ValuesIterator must be random assess iterator."
  );

  using value_type = typename std::iterator_traits<ValuesIterator>::value_type;
  using reference = const value_type&;
  using index_type = typename std::iterator_traits<ValuesIterator>::difference_type;

  IndirectMapper() = default;

  IndirectMapper(ValuesIterator values):
      values_(values) { }

  reference operator()(index_type n) const {
    return *(values_ + n);
  }

private:
  ValuesIterator values_;
};

} // namespace detail

template <typename ValuesIterator, typename IndexesIterator>
using indirect_iterator = mapping_iterator<IndexesIterator, detail::IndirectMapper<ValuesIterator>>;

/**
 * Iterates over random acess iterator with usage of permutation.
 *
 * Example:
 * <pre>
 * std::vector<int> perm = {2, 0, 1};
 * std::vector<std::string> values = {"Ala", "ma", "kota"};
 * auto it = make_indirect_iterator(values.begin(), perm.begin());
 * auto end = make_indirect_iterator(values.begin(), perm.end());
 * for (const auto& elem: make_range(it, end)) {
 *    std::cout << elem << ' ';
 * }
 * </pre>
 * Prints: kota Ala ma
 */
template <typename ValuesIterator, typename IndexesIterator>
auto make_indirect_iterator(ValuesIterator values, IndexesIterator indexes) ->
indirect_iterator<ValuesIterator, IndexesIterator> {
  detail::IndirectMapper<ValuesIterator> mapper(values);
  return indirect_iterator<ValuesIterator, IndexesIterator>(std::move(indexes), std::move(mapper));
};

/**
 * Base class for generators implementation.
 *
 * Example:
 * <pre>
 * for (auto gen = new foo_generator(); gen->hasNext(); ) {
 *    auto value = gen->next();
 *    ...
 * }
 * </pre>
 */
template <typename ValueType>
class generator {
public:
  using ptr = std::shared_ptr<generator>;
  using value_type = ValueType;

  generator() = default;
  generator(const generator&) = delete;
  generator& operator=(const generator&) = delete;
  generator(generator&&) = default;
  virtual ~generator() = default;

  /**
   * Generates new value and returns it.
   *
   * If there is no next value returns Nothing.
   */
  virtual Maybe<value_type> next() = 0;
};

namespace detail {

template <typename ValueType>
struct generator_iterator_helper {
  using self_type = generator_iterator_helper;
  using generator_type = generator<ValueType>;
  using generator_pointer = typename generator<ValueType>::ptr;
  using value_type = typename generator_type::value_type;
  using reference = const value_type&;
  using pointer = const value_type*;
  using difference_type = std::ptrdiff_t;

  generator_iterator_helper() = default;

  generator_iterator_helper(generator_type* generator):
    generator_iterator_helper(generator_pointer(generator)) { }

  generator_iterator_helper(generator_pointer generator):
    generator_(std::move(generator)) {
      next();
  }
  
  void next() {
    auto value = generator_->next();
    if (value != Nothing)
      value_ = value.get();
    else
      generator_.reset();
  }
  
  reference value() const { return value_; }
  
  pointer ptr() const { return &value_; }

  bool equal(const self_type& other) const {
    return (this == &other) || (is_end() && other.is_end());
  }
  
private:
  bool is_end() const {
    return !bool(generator_.get());
  }
  
  generator_pointer generator_;
  value_type value_;
};

} // namespace detail

/**
 * Iterator for iterating over values returned by generator.
 *
 * Example:
 * <pre>
 * std::vector<uint32> v(generator_iterator<uint32>(new foo_generator()), generator_iterator<uint32>());
 * </pre>
 */
template <typename ValueType>
using generator_iterator = input_iterator<detail::generator_iterator_helper<ValueType>>;

/**
 * Returns iterator range to values generated by generator.
 *
 * Example:
 * <pre>
 * for (const auto& elem: iterate_generator(new foo_generator())) {
 *    ...
 * }
 * </pre>
 */
template <typename T>
iterator_range<generator_iterator<T>> iterate_generator(generator<T>* gen) {
  return make_range(generator_iterator<T>(gen), generator_iterator<T>());
}

} // namespace lib
