#pragma once
// Jakub Staroń, 2016

#include "iterators.h"

namespace lib {

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

std::ostream& simple(std::ostream& stream) {
  stream.iword(detail::kSimpleFancyFlagID) = detail::simple_printing_type;
  return stream;
}

std::ostream& fancy(std::ostream& stream) {
  stream.iword(detail::kSimpleFancyFlagID) = detail::fancy_printing_type;
  return stream;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, const std::pair<T1, T2>& pair);

template <typename... Args>
std::ostream& operator<<(std::ostream& stream, const std::tuple<Args...>& tuple);

template <typename Iterable>
typename std::enable_if<detail::allow_print_operator<Iterable>(), std::ostream&>::type
operator<<(std::ostream& stream, const Iterable& iterable);

#ifdef HAVE_INT128_TYPES

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

std::ostream& operator<<(std::ostream& stream, int128 n) {
  if (n < 0) {
    stream.put('-');
    n = -n;
  }
  stream << uint128(n);
  return stream;
}

#endif

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
 * To allow constructions like
 * int a, b, c;
 * std::cin >> std::tie(a, b, c);
 */
template <typename... Args>
std::istream& operator>>(std::istream& stream, std::tuple<Args...>&& tuple) {
  return stream >> tuple;
}

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

template <typename... Args>
void print(const char* format, const Args&... args) {
  print(std::cout, format, args...);
}

std::ostream& flush(std::ostream& stream) {
  return stream.flush();
}

/**
 * takes arguments as rref to allow constructions like
 * int a, b;
 * read(std::cin, a, ignore<int>(), b);
 */
template <typename... Args>
void read(std::istream& stream, Args&&... args) {
  auto tuple = std::make_tuple(std::ref(args)...);
  stream >> tuple;
}

template <typename T>
struct ignore {
  ignore() = default;

  friend std::istream& operator>>(std::istream& stream, const ignore&) {
    T ignored;
    return stream >> ignored;
  }
};

class lines_iterator {
public:
  using self_type = lines_iterator;
  using value_type = std::string;
  using reference = const value_type&;
  using pointer = const value_type*;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  lines_iterator():
      stream_(nullptr), line_() { }

  lines_iterator(std::istream* stream):
      stream_(stream) {
    next_line();
  }

  reference operator*() const {
    return line_;
  }

  pointer operator->() const {
    return &line_;
  }

  self_type& operator++() {
    next_line();
    return *this;
  }

  friend bool operator==(const self_type& lhs, const self_type& rhs) {
    return lhs.is_end() == rhs.is_end();
  }

private:
  void next_line() {
    if (stream_) {
      if (stream_->eof())
        stream_ = nullptr;
      else if (!stream_->good())
        throw std::ios::failure("lines_iterator - stream in invalid state after line read.");
      else
        std::getline(*stream_, line_);
    }
  }

  bool is_end() const {
    return stream_ == nullptr;
  }

  std::istream* stream_;
  std::string line_;
};

iterator_range<lines_iterator> iterate_lines(std::istream& stream) {
  return make_range(lines_iterator(&stream), lines_iterator());
}

} // namespace lib
