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

template <typename... Args>
class tuple_printer {
  static constexpr size_t arguments_count = sizeof...(Args);
  using index_sequence = typename generate_sequence<arguments_count>::type;
  using function_type = void(tuple_printer::*)();
  using table_type = std::array<function_type, arguments_count>;
  using tuple_type = std::tuple<Args...>;

  template <size_t N>
  void print_() {
    stream_ << std::get<N>(tuple_);
  }

  template <size_t... Indexes>
  static constexpr table_type build_table(integer_sequence<Indexes...>) {
    return {{&tuple_printer::print_<Indexes>...}};
  }

  static constexpr table_type functions_ = build_table(index_sequence());
public:
  tuple_printer (std::ostream& stream, const tuple_type& tuple):
      stream_(stream), tuple_(tuple) { }


  void print(size_t i) {
    if (i >= arguments_count)
      throw std::out_of_range("tuple_printer: out of range");

    (this->*functions_[i])();
  }

private:
  std::ostream& stream_;
  const tuple_type& tuple_;
};

template <typename... Args>
constexpr typename tuple_printer<Args...>::table_type tuple_printer<Args...>::functions_;

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

template <typename... Args>
void print(std::ostream& stream, const char* format, const Args&... args) {
  auto tuple = std::make_tuple(std::cref(args)...);
  detail::tuple_printer<const Args&...> tuple_printer(stream, tuple);
  for (const char* it = format, *prev = format; *it != '\0'; ) {
    if (*it == '\0') {
      stream.write(prev, it - prev);
      prev = it;
    }
    else if (*it == '%') {
      stream.write(prev, it - prev);
      ++it;
      if (*it == '%') {
        stream.put('%');
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
    else {
      ++it;
    }
  }
  stream.put('\n');
}

template <typename... Args>
void print(const char* format, const Args&... args) {
  print(std::cout, format, args...);
}

} // namespace lib
