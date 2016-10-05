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
  return is_iterable<T>::value && !std::is_same<T, std::string>::value && !std::is_same<T, const char*>::value;
}

template <typename... Args>
class tuple_printer {
  static constexpr size_t arguments_count = sizeof...(Args);
  using function_type = void(tuple_printer::*)();
  using table_type = std::array<function_type, arguments_count>;
  using tuple_type = std::tuple<Args...>;

  static constexpr table_type build_table() {
    return {{}};
  }

  static constexpr table_type functions_ = build_table();
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

std::ostream& simple(std::ostream& stream) {
  stream.iword(detail::kSimpleFancyFlagID) = detail::simple_printing_type;
  return stream;
}

std::ostream& add_none(std::ostream& stream) {
  stream.iword(detail::kSimpleFancyFlagID) = detail::fancy_printing_type;
  return stream;
}

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
  for (auto i: range(size_t(0), sizeof...(Args))) {
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


} // namespace lib
