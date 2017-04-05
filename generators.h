#pragma once
// Jakub Staroń, 2016-2017

#include "iterators.h"
#include "iterators/input_iterator.h"
#include "utils/maybe.h"

namespace pcl {

/**
 * Base class for generators implementation.
 */
template <typename ValueType>
class GeneratorBase {
public:
  using value_type = ValueType;
  using pointer = std::unique_ptr<GeneratorBase>;

  GeneratorBase() = default;
  GeneratorBase(const GeneratorBase&) = delete;
  GeneratorBase& operator=(const GeneratorBase&) = delete;
  GeneratorBase(GeneratorBase&&) = default;
  virtual ~GeneratorBase() = default;

  /**
   * Generates new value and returns it.
   *
   * If there is no next value returns Nothing.
   */
  virtual Maybe<value_type> next() = 0;
};

template <class ValueType>
using Generator = std::unique_ptr<GeneratorBase<ValueType>>;

/**
 * type_traits-like predicate. Equal to true_type if argument is generator.
 *
 * Example:
 * <pre>
 * static_assert(is_generator<DummyGenerator>, "our DummyGenerator is wrongly implemented!");
 * </pre>
 */
template <typename Generator>
struct is_generator : std::false_type { };

template <typename GeneratorType>
struct is_generator<std::unique_ptr<GeneratorType>> :
    std::is_base_of<GeneratorBase<typename GeneratorType::value_type>, GeneratorType> { };

template <typename GeneratorType>
struct is_generator<std::unique_ptr<GeneratorType>&> :
    std::is_base_of<GeneratorBase<typename GeneratorType::value_type>, GeneratorType> { };

namespace detail {

template <typename GeneratorType, typename... Args>
Generator<typename GeneratorType::value_type> build_generator(Args&& ... args) {
  return Generator<typename GeneratorType::value_type>(new GeneratorType(std::forward<Args>(args)...));
};

} // namespace detail

/**
 * Takes callable (e.g. lambda, function pointer, callable struct)
 * returning Maybe<type> and creates generator.
 *
 * Note that given function is lazy.
 *
 * Example:
 * <pre>
 * auto generator = generate([]() { return Just(42); }); // Infinite sequence of 42s
 * </pre>
 */
template <typename Callable>
auto generate(Callable&& callable) ->
Generator<typename decltype(callable())::value_type>
{
  using value_type = typename decltype(callable())::value_type;

  class FromCallableGenerator: public GeneratorBase<value_type> {
  public:
    FromCallableGenerator(Callable&& callable):
        callable_(std::move(callable)) { }

    Maybe<value_type> next() final {
      return callable_();
    }

  private:
    Callable callable_;
  };

  return detail::build_generator<FromCallableGenerator>(std::move(callable));
}

/**
 * Takes two iterators and returns generator
 * of given sequence.
 *
 * Note that given function is lazy.
 * Note that returned generator depends
 * on iterators being valid.
 *
 * Example:
 * <pre>
 * std::vector<int> v = {1, 2, 3};
 * auto generator = generate(v.begin(), v.end()); // generates 1, 2, 3
 * </pre>
 */
template <typename Iterator>
auto generate(Iterator begin, Iterator end) ->
Generator<typename std::iterator_traits<Iterator>::value_type>
{
  using value_type = typename std::iterator_traits<Iterator>::value_type;

  class SequenceGenerator: public GeneratorBase<value_type> {
  public:
    SequenceGenerator(Iterator&& begin, Iterator&& end):
        it_(std::move(begin)), end_(std::move(end)) { }

    Maybe<value_type> next() final {
      return (it_ == end_)? Nothing : Just<value_type>(*it_++);
    }

  private:
    Iterator it_;
    const Iterator end_;
  };

  return detail::build_generator<SequenceGenerator>(std::move(begin), std::move(end));
}

/**
 * Takes iterable and returns generator
 * of given sequence.
 *
 * Note that given function is lazy.
 * Note that returned generator depends
 * on iterators being valid.
 *
 * Example:
 * <pre>
 * std::vector<int> v = {1, 2, 3};
 * auto generator = generate(v); // generates 1, 2, 3
 * </pre>
 */
template <typename Iterable>
auto generate(Iterable& iterable) ->
typename std::enable_if<
    is_iterable<Iterable>::value,
    decltype(generate(iterable.begin(), iterable.end()))
>::type
{
  return generate(iterable.begin(), iterable.end());
}

namespace detail {

template <typename ValueType>
struct generator_iterator_helper {
  using value_type = ValueType;
  using self_type = generator_iterator_helper;
  using generator_type = GeneratorBase<ValueType>;
  using generator_pointer = generator_type*;
  using reference = const value_type&;
  using pointer = const value_type*;
  using difference_type = std::ptrdiff_t;

  generator_iterator_helper() = default;

  generator_iterator_helper(generator_pointer generator):
      generator_(std::move(generator)) {
    next();
  }

  void next() {
    assert(!is_end());
    value_ = generator_->next();
    if (value_ == Nothing)
      generator_ = nullptr;
  }

  reference value() const { return value_.get(); }

  pointer ptr() const { return &value_.get(); }

  bool equal(const self_type& other) const {
    return (this == &other) || (is_end() && other.is_end());
  }

private:
  bool is_end() const {
    return generator_ == nullptr;
  }

  generator_pointer generator_;
  Maybe<value_type> value_;
};

template <typename ValueType>
using generator_iterator = input_iterator<detail::generator_iterator_helper<ValueType>>;

template <typename ValueType>
class generator_iterator_range {
public:
  using iterator_type = generator_iterator<ValueType>;

  explicit generator_iterator_range(Generator<ValueType>&& generator):
      generator_(std::move(generator)) { }

  iterator_type begin() const {
    return generator_iterator<ValueType>(generator_.get());
  }

  iterator_type end() const {
    return generator_iterator<ValueType>();
  }

private:
  Generator<ValueType> generator_;
};

template <typename Generator>
auto build_generator_iterator_range(Generator&& generator) ->
generator_iterator_range<typename Generator::element_type::value_type>
{
  return generator_iterator_range<typename Generator::element_type::value_type>(std::move(generator));
}

} // namespace detail

/**
 * Builds iterator_range from Generator.
 *
 * Consumes given generator.
 *
 * Example:
 * <pre>
 * for (auto elem: iterate(generator) {
 *  ...
 * }
 * </pre>
 */
template <typename GeneratorHandle>
auto iterate(GeneratorHandle&& generator) ->
typename std::enable_if<
    is_generator<GeneratorHandle>::value,
    decltype(detail::build_generator_iterator_range(std::move(generator)))
>::type
{
  return detail::build_generator_iterator_range(std::move(generator));
}

/**
 * Calls given callable on every element
 * of sequence generated by generator.
 *
 * Example:
 * <pre>
 * std::vector<int> v = {1, 2, 3};
 * iterate(generate(v), [](int i) { print("%0", i); });
 * </pre>
 */
template <typename Generator, typename Callable>
auto iterate(Generator&& generator, Callable&& callable) ->
typename std::enable_if<
    is_generator<Generator>::value
>::type
{
  while (true) {
    auto elem = generator->next();
    if (elem == Nothing)
      break;
    callable(std::move(elem.get()));
  }
  generator.reset();
}

/**
 * Builds vector from elements generated by given generator.
 *
 * Consumes given generator.
 *
 * Example:
 * <pre>
 * auto v = as_vector(generator);
 * </pre>
 */
template <typename GeneratorHandle>
auto as_vector(GeneratorHandle&& generator) ->
typename std::enable_if<
    is_generator<GeneratorHandle>::value,
    std::vector<typename std::remove_reference<GeneratorHandle>::type::element_type::value_type>
>::type
{
  using value_type = typename std::remove_reference<GeneratorHandle>::type::element_type::value_type;
  auto range = iterate(generator);
  return std::vector<value_type>(range.begin(), range.end());
}

/**
 * Builds list from elements generated by given generator.
 *
 * Consumes given generator.
 *
 * Example:
 * <pre>
 * auto l = as_list(generator);
 * </pre>
 */
template <typename GeneratorHandle>
auto as_list(GeneratorHandle&& generator) ->
typename std::enable_if<
    is_generator<GeneratorHandle>::value,
    std::list<typename std::remove_reference<GeneratorHandle>::type::element_type::value_type>
>::type
{
  using value_type = typename std::remove_reference<GeneratorHandle>::type::element_type::value_type;
  auto range = iterate(generator);
  return std::list<value_type>(range.begin(), range.end());
}

/**
 * Builds set from elements generated by given generator.
 *
 * Consumes given generator.
 *
 * Example:
 * <pre>
 * auto s = as_set(generator);
 * </pre>
 */
template <typename GeneratorHandle>
auto as_set(GeneratorHandle&& generator) ->
typename std::enable_if<
    is_generator<GeneratorHandle>::value,
    std::set<typename std::remove_reference<GeneratorHandle>::type::element_type::value_type>
>::type
{
  using value_type = typename std::remove_reference<GeneratorHandle>::type::element_type::value_type;
  auto range = iterate(generator);
  return std::set<value_type>(range.begin(), range.end());
}

/**
 * Takes generator and predicate, returns
 * generator generating filtered sequence.
 *
 * Note that function is lazy.
 * Example:
 * <pre>
 * std::string s = "Ala ma kota";
 * auto generator = generate(s);
 * generator = filter(generator, is_lower<char>); // returns sequence "lamakota"
 * </pre>
 */
template <typename GeneratorHandle, typename Callable>
auto filter(GeneratorHandle&& generator, Callable predicate) ->
typename std::enable_if<
    is_generator<GeneratorHandle>::value,
    Generator<typename std::remove_reference<GeneratorHandle>::type::element_type::value_type>
>::type
{
  using value_type = typename std::remove_reference<GeneratorHandle>::type::element_type::value_type;

  return generate(
      std::bind([] (typename std::remove_reference<GeneratorHandle>::type& generator, Callable predicate) -> Maybe<value_type> {
                  while (true) {
                    auto elem = generator->next();
                    if (elem == Nothing)
                      return Nothing;
                    else if (predicate(elem.get()))
                      return std::move(elem);
                  }
                },
                std::move(generator),
                std::move(predicate)
      )
  );
}

/**
 * Takes generator and callable, returns
 * generator generating mapped sequence.
 *
 * Note that function is lazy.
 * Example:
 * <pre>
 * std::vector<int> v = {1, 2, 3};
 * auto generator = generate(s);
 * generator = map(generator, [](int i){ return i+1; }); // returns sequence 2, 3, 4
 * </pre>
 */

template <typename GeneratorHandle, typename Callable>
auto map(GeneratorHandle&& generator, Callable callable) ->
typename std::enable_if<
    is_generator<GeneratorHandle>::value,
    Generator<decltype(callable(generator->next().get()))>
>::type
{
  using value_type = decltype(callable(generator->next().get()));

  return generate(
      std::bind([] (typename std::remove_reference<GeneratorHandle>::type& generator, Callable callable) -> Maybe<value_type> {
                  auto elem = generator->next();
                  return (elem == Nothing)? Nothing : Just<value_type>(callable(elem.get()));
                },
                std::move(generator),
                std::move(callable)
      )
  );
}

} // namespace pcl
