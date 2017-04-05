#pragma once
// Jakub Staroń, 2016-2017

#include "iterators.h"
#include "generators.h"

namespace pcl {

namespace detail {

const int kSimpleFancyFlagID = std::ios_base::xalloc();

enum printing_type {
  simple_printing_type = 0,
  fancy_printing_type = 1
};

class delimiter_printer {
public:
  delimiter_printer(std::ostream& stream): first_(true) {
    fancy_ = (stream.iword(kSimpleFancyFlagID) == fancy_printing_type);
  }

  const char* prefix() {
    return fancy_? "(" : "";
  }

  const char* delimiter() {
    if (!first_) {
      return fancy_? ", " : " ";
    }
    else {
      first_ = false;
      return "";
    }
  }

  const char* postfix() {
    return fancy_? ")" : "";
  }

private:
  bool first_;
  bool fancy_;
};

template <typename T>
constexpr bool allow_print_operator() {
  return
      is_iterable<T>::value &&
      !std::is_same<T, std::string>::value &&
      !std::is_same<T, const char*>::value &&
      !std::is_same<typename std::remove_extent<T>::type, char>::value;
}

} // namespace detail

/**
 * Sets stream printing mode to simple. Default option.
 *
 * Example:
 * <pre>
 * std::cout << simple << std::make_pair(1, 2) << std::endl;
 * </pre>
 * Prints:
 * 1 2
 */
std::ostream& simple(std::ostream& stream) {
  stream.iword(detail::kSimpleFancyFlagID) = detail::simple_printing_type;
  return stream;
}

/**
 * Sets stream printing mode to fancy. Helpful for printing debug messages.
 *
 * Example:
 * <pre>
 * std::cout << simple << std::make_pair(1, 2) << std::endl;
 * </pre>
 * Prints:
 * (1, 2)
 */
std::ostream& fancy(std::ostream& stream) {
  stream.iword(detail::kSimpleFancyFlagID) = detail::fancy_printing_type;
  return stream;
}

/**
 * Overload operator<< for ostream and pair.
 */
template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, const std::pair<T1, T2>& pair);

/**
 * Overload operator<< for ostream and tuple.
 */
template <typename... Args>
std::ostream& operator<<(std::ostream& stream, const std::tuple<Args...>& tuple);

/**
 * Overload operator<< for ostream and every iterable (eg vector, map, array).
 */
template <typename Iterable>
typename std::enable_if<detail::allow_print_operator<Iterable>(), std::ostream&>::type
operator<<(std::ostream& stream, const Iterable& iterable);

#ifdef USE_INT128_TYPES

/**
 * Overload operator<< for ostream and uint128.
 */
std::ostream& operator<<(std::ostream& stream, uint128 n) {
  constexpr int32 buffer_size = 64;
  constexpr uint128 ten = 10;
  char buffer[buffer_size];
  int32 index = buffer_size - 1;

  do {
    uint128 digit = n % 10;
    buffer[index--] = char('0' + digit);
    n /= 10;
  } while (n > 0 && index >= 0);

  stream.write(buffer + index + 1, buffer_size - index - 1);
  return stream;
}

/**
 * Overload operator<< for ostream and int128.
 */
std::ostream& operator<<(std::ostream& stream, int128 n) {
  if (n < 0) {
    stream.put('-');
    n = -n;
  }
  stream << uint128(n);
  return stream;
}

#endif

/**
 * Overload operator<< for istream and pair.
 */
template <typename T1, typename T2>
std::istream& operator>>(std::istream& stream, std::pair<T1, T2>& pair);

/**
 * Overload operator<< for istream and tuple.
 */
template <typename... Args>
std::istream& operator>>(std::istream& stream, std::tuple<Args...>& tuple);

/**
 * Helper for marking input as ignored.
 *
 * Example:
 * <pre>
 * int a, b;
 * std::cin >> a >> ignore<int>() >> b;
 * </pre>
 */
template <typename T>
struct ignore {
  ignore() = default;

  friend std::istream& operator>>(std::istream& stream, const ignore&&) {
    T ignored;
    return stream >> ignored;
  }

  friend std::istream& operator>>(std::istream& stream, const ignore&) {
    T ignored;
    return stream >> ignored;
  }
};

namespace detail {

template<std::size_t...>
struct integer_sequence{};

template <size_t N>
struct generate_sequence {
private:
  template<std::size_t M, std::size_t... Is>
  struct helper {
    using type = typename helper<M-1, M-1, Is...>::type;
  };;

  template<std::size_t... Is>
  struct helper<0, Is...> {
    using type = integer_sequence<Is...>;
  };

public:
  using type = typename helper<N>::type;
};

template <typename Functor, size_t N>
struct dynamize {
  using functor_type = Functor;
private:
  using index_sequence = typename generate_sequence<N>::type;
  using function_type = void(functor_type::*)();
  using table_type = std::array<function_type, N>;

  template <size_t... Indexes>
  static constexpr table_type build_table(integer_sequence<Indexes...>) {
    return {{&functor_type::template operator()<Indexes>...}};
  }

  static constexpr table_type functions_ = build_table(index_sequence());
public:
  dynamize(functor_type&& functor):
      functor_(std::move(functor)) { }

  void call(size_t i) {
    if (i >= N)
      throw std::out_of_range("tuple_printer: out of range");

    (functor_.*functions_[i])();
  }

private:
  functor_type functor_;
};

template <typename Functor, size_t N>
constexpr typename dynamize<Functor, N>::table_type dynamize<Functor, N>::functions_;

template <typename... Args>
class tuple_printer {
  static constexpr size_t arguments_count = sizeof...(Args);
  using tuple_type = std::tuple<Args...>;

  struct impl {
    std::ostream& stream_;
    const tuple_type& tuple_;

    template <size_t N>
    void operator()() {
      stream_ << std::get<N>(tuple_);
    }
  };

public:
  tuple_printer (std::ostream& stream, const tuple_type& tuple):
    dynamize_(impl{stream, tuple}) { }

  void print(size_t i) {
    dynamize_.call(i);
  }

private:
  dynamize<impl, arguments_count> dynamize_;
};

template <typename... Args>
class tuple_reader {
  static constexpr size_t arguments_count = sizeof...(Args);
  using tuple_type = std::tuple<Args...>;

  struct impl {
    std::istream& stream_;
    tuple_type& tuple_;

    template <size_t N>
    void operator()() {
      stream_ >> std::get<N>(tuple_);
    }
  };

public:
  tuple_reader (std::istream& stream, tuple_type& tuple):
      dynamize_(impl{stream, tuple}) { }

  void read(size_t i) {
    dynamize_.call(i);
  }

private:
  dynamize<impl, arguments_count> dynamize_;
};

} // namespace detail

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, const std::pair<T1, T2>& pair) {
  detail::delimiter_printer printer(stream);
  stream << printer.prefix();
  stream << printer.delimiter() << pair.first;
  stream << printer.delimiter() << pair.second;
  stream << printer.postfix();
  return stream;
}

template <typename... Args>
std::ostream& operator<<(std::ostream& stream, const std::tuple<Args...>& tuple) {
  detail::delimiter_printer delimiter_printer(stream);
  detail::tuple_printer<Args...> tuple_printer(stream, tuple);
  stream << delimiter_printer.prefix();
  for (auto i: range<size_t>(0, sizeof...(Args))) {
    stream << delimiter_printer.delimiter();
    tuple_printer.print(i);
  }
  stream << delimiter_printer.postfix();
  return stream;
}

template <typename Iterable>
typename std::enable_if<detail::allow_print_operator<Iterable>(), std::ostream&>::type
operator<<(std::ostream& stream, const Iterable& iterable) {
  detail::delimiter_printer printer(stream);
  stream << printer.prefix();
  for (const auto& elem: iterable) {
    stream << printer.delimiter() << elem;
  }
  stream << printer.postfix();
  return stream;
}

template <typename T1, typename T2>
std::istream& operator>>(std::istream& stream, std::pair<T1, T2>& pair) {
  return stream >> pair.first >> pair.second;
}

template <typename... Args>
std::istream& operator>>(std::istream& stream, std::tuple<Args...>& tuple) {
  detail::tuple_reader<Args...> tuple_reader(stream, tuple);
  for (auto i: range<size_t>(0, sizeof...(Args))) {
    tuple_reader.read(i);
  }
  return stream;
}


/**
 * Second version of overload operator>> for istream and pair.
 *
 * Needed to allow constructions like
 * <pre>
 * int a, b, c;
 * std::cin >> std::tie(a, b, c);
 * </pre>
 */
template <typename... Args>
std::istream& operator>>(std::istream& stream, std::tuple<Args...>&& tuple) {
  return stream >> tuple;
}

/**
 * Python-like print function.
 *
 * Example:
 * <pre>
 * print(std::cerr, "1 + 2 = %0, 2 + 3 = %1", 3, 5);
 * </pre>
 */
template <typename... Args>
void print(std::ostream& stream, const char* format, const Args&... args) {
  auto tuple = std::make_tuple(std::cref(args)...);
  detail::tuple_printer<const Args&...> tuple_printer(stream, tuple);
  constexpr char null = '\0';
  constexpr char percent = '%';
  for (const char* it = format, *prev = format; *it != '\0'; ) {
    while (*it != null && *it != percent)
      it++;

    stream.write(prev, it - prev);
    if (*it == percent) {
      ++it;
      if (*it == percent) {
        stream.put(percent);
      }
      else if (std::isdigit(*it)) {
        size_t index = size_t(*it - '0');
        tuple_printer.print(index);
      }
      else {
        throw std::invalid_argument("print - invalid character after %");
      }
      prev = ++it;
    }
  }
  stream.put('\n');
}

/**
 * Python-like print function. Prints to std::cout.
 */
template <typename... Args>
void print(const char* format, const Args&... args) {
  print(std::cout, format, args...);
}

/**
 * flush operator for usage with print.
 *
 * Example:
 * <pre>
 * print("important message%0", pcl::flush);
 * </pre>
 */
std::ostream& flush(std::ostream& stream) {
  return stream.flush();
}

/**
 * non-flushing newline operator.
 *
 * Example:
 * <pre>
 * std::cout << "Ala ma kota." << newline;
 * </pre>
 */
std::ostream& newline(std::ostream& stream) {
  return stream.put('\n');
}

/**
 * Python-like read function.
 *
 * Example:
 * <pre>
 * int a, b;
 * read(std::cin, a, ignore<int>(), b);
 * </pre>
 */
template <typename... Args>
void read(std::istream& stream, Args&&... args) {
  auto tuple = std::make_tuple(std::ref(args)...);
  stream >> tuple;
}

/**
 * Returns generator yielding lines from given stream.
 */
Generator<std::string> Lines(std::istream& stream) {
  return generate([&stream]() -> Maybe<std::string> {
    if (!stream.good())
      return Nothing;
    std::string line;
    std::getline(stream, line);
    return line;
  });
}

/**
 * Returns generator reading ginven number of
 * values from given stream.
 *
 * Note that this call itself do not read values
 * from stream (generator is lazy).
 *
 * Example:
 * <pre>
 * // Reads 10 ints from std::cin and stores them in vector
 * auto v = as_vector(ReadSequence<int>(std::cin, 10));
 * </pre>
 */
template <typename ValueType>
auto ReadSequence(std::istream& stream, uint32 count) ->
Generator<ValueType>
{
  class SequenceReader: public GeneratorBase<ValueType> {
  public:
    SequenceReader(std::istream& stream, uint32 count):
        stream_(stream), count_(count) { }

    Maybe<ValueType> next() final {
      if (count_ == 0)
        return Nothing;

      count_--;
      ValueType value;
      stream_ >> value;
      return Just<ValueType>(value);
    }

  private:
    std::istream& stream_;
    uint32 count_;
  };
  return detail::build_generator<SequenceReader>(stream, count);
}

} // namespace pcl
