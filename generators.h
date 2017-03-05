#pragma once
// Jakub Staroń, 2017

#include "iterators.h"
#include "iterators/input_iterator.h"
#include "utils/maybe.h"

namespace lib {

/**
 * Base class for generators implementation.
 */
template <typename ValueType>
class Generator {
public:
  using value_type = ValueType;
  using pointer = std::unique_ptr<Generator>;

  Generator() = default;
  Generator(const Generator&) = delete;
  Generator& operator=(const Generator&) = delete;
  Generator(Generator&&) = default;
  virtual ~Generator() = default;

  /**
   * Generates new value and returns it.
   *
   * If there is no next value returns Nothing.
   */
  virtual Maybe<value_type> next() = 0;
};

template <class GeneratorType>
using generator_handle = std::unique_ptr<GeneratorType>;

/**
 * type_traits-like predicate. Equal to true_type if argument is generator.
 *
 * Example:
 * <pre>
 * static_assert(is_generator<DummyGenerator>, "our DummyGenerator is wrongly implemented!");
 * </pre>
 */
template <typename GeneratorType>
using is_generator = std::is_base_of<Generator<typename GeneratorType::value_type>, GeneratorType>;

template <typename GeneratorHandle>
struct is_generator_handle : std::false_type { };

template <typename GeneratorType>
struct is_generator_handle<std::unique_ptr<GeneratorType>> : is_generator<GeneratorType> { };

template <typename GeneratorType>
struct is_generator_handle<std::unique_ptr<GeneratorType>&> : is_generator<GeneratorType> { };

namespace detail {

template <typename GeneratorType, typename... Args>
generator_handle<GeneratorType> build_generator_handle(Args&&... args) {
  return generator_handle<GeneratorType>(new GeneratorType(std::forward<Args>(args)...));
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
generator_handle<Generator<typename decltype(callable())::value_type>>
{
  using value_type = typename decltype(callable())::value_type;

  class FromCallableGenerator: public Generator<value_type> {
  public:
    FromCallableGenerator(Callable&& callable):
        callable_(std::move(callable)) { }

    Maybe<value_type> next() final {
      return callable_();
    }

  private:
    Callable callable_;
  };

  return detail::build_generator_handle<FromCallableGenerator>(std::move(callable));
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
generator_handle<Generator<typename std::iterator_traits<Iterator>::value_type>>
{
  using value_type = typename std::iterator_traits<Iterator>::value_type;

  class SequenceGenerator: public Generator<value_type> {
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

  return detail::build_generator_handle<SequenceGenerator>(std::move(begin), std::move(end));
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

template <typename GeneratorType>
struct generator_iterator_helper {
  using self_type = generator_iterator_helper;
  using generator_type = GeneratorType;
  using generator_pointer = generator_type*;
  using value_type = typename generator_type::value_type;
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
    auto value = generator_->next();
    if (value != Nothing)
      value_ = value.get();
    else
      generator_ = nullptr;
  }

  reference value() const { return value_; }

  pointer ptr() const { return &value_; }

  bool equal(const self_type& other) const {
    return (this == &other) || (is_end() && other.is_end());
  }

private:
  bool is_end() const {
    return generator_ == nullptr;
  }

  generator_pointer generator_;
  value_type value_;
};

template <typename GeneratorType>
using generator_iterator = input_iterator<detail::generator_iterator_helper<GeneratorType>>;

template <typename GeneratorType>
class generator_iterator_range {
public:
  using iterator_type = generator_iterator<GeneratorType>;

  explicit generator_iterator_range(generator_handle<GeneratorType>&& generator):
      generator_(std::move(generator)) { }

  iterator_type begin() const {
    return generator_iterator<GeneratorType>(generator_.get());
  }

  iterator_type end() const {
    return generator_iterator<GeneratorType>();
  }

private:
  generator_handle<GeneratorType> generator_;
};

template <typename GeneratorHandle>
auto build_generator_iterator_range(GeneratorHandle&& generator) ->
generator_iterator_range<typename GeneratorHandle::element_type>
{
  return generator_iterator_range<typename GeneratorHandle::element_type>(std::move(generator));
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
    is_generator_handle<GeneratorHandle>::value,
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
template <typename GeneratorHandle, typename Callable>
auto iterate(GeneratorHandle&& generator, Callable&& callable) ->
typename std::enable_if<
    is_generator_handle<GeneratorHandle>::value
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
    is_generator_handle<GeneratorHandle>::value,
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
    is_generator_handle<GeneratorHandle>::value,
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
    is_generator_handle<GeneratorHandle>::value,
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
    is_generator_handle<GeneratorHandle>::value,
    generator_handle<Generator<typename std::remove_reference<GeneratorHandle>::type::element_type::value_type>>
>::type
{
  using value_type = typename std::remove_reference<GeneratorHandle>::type::element_type::value_type;

  return generate(
      std::bind([] (GeneratorHandle generator, Callable predicate) -> Maybe<value_type> {
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
    is_generator_handle<GeneratorHandle>::value,
    generator_handle<Generator<decltype(callable(generator->next().get()))>>
>::type
{
  using value_type = decltype(callable(generator->next().get()));

  return generate(
      std::bind([] (GeneratorHandle generator, Callable callable) -> Maybe<value_type> {
                  auto elem = generator->next();
                  return (elem == Nothing)? Nothing : Just<value_type>(callable(elem.get()));
                },
                std::move(generator),
                std::move(callable)
      )
  );
}

} // namespace lib
