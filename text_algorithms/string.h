#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"

namespace lib {
namespace text {

template <typename Value>
struct is_equal_to_predicate {
  using value_type = Value;

  is_equal_to_predicate(value_type value):
      value_(std::move(value)) { }

  bool operator()(const value_type& value) const {
    return value == value_;
  }

private:
  value_type value_;
};

template <typename Value>
is_equal_to_predicate<Value> is_equal_to(Value value) {
  return is_equal_to_predicate<Value>(std::move(value));
}

template <typename Value>
struct is_in_set_predicate {
  using value_type = Value;

  template <typename Iterator>
  is_in_set_predicate(Iterator begin, Iterator end):
      values_(begin, end) { }

  bool operator()(const value_type& value) const {
    return std::find(values_.begin(), values_.end(), value) != values_.end();
  }

private:
  std::vector<Value> values_;
};

is_in_set_predicate<char> is_in_set(const char* characters) {
  return is_in_set_predicate<char>(characters, characters + std::strlen(characters));
}

template <typename Iterator, typename Predicate>
std::vector<std::string> split(Iterator begin, Iterator end, Predicate predicate, bool includeEmpty = false) {
  std::vector<std::string> result;
  for (auto it = begin, prev = begin; ; ) {
    while (it != end && !predicate(*it))
      ++it;

    if (includeEmpty || prev != it)
      result.emplace_back(prev, it);

    if (it != end) {
      ++it;
      prev = it;
    }
    else {
      break;
    }
  }
  return result;
}

template <typename Iterator>
std::vector<std::string> split(Iterator begin, Iterator end, char c, bool includeEmpty = false) {
  return split(begin, end, is_equal_to(c), includeEmpty);
}

template <typename Iterator>
std::vector<std::string> split(Iterator begin, Iterator end, const char* chars, bool includeEmpty = false) {
  return split(begin, end, is_in_set(chars), includeEmpty);
}

template <typename Iterator>
std::string join(std::string separator, Iterator begin, Iterator end) {
  size_t size = 0;
  for (const auto& str: make_range(begin, end))
    size += 1 + str.size();

  std::string result;
  result.reserve(size);

  bool first = true;
  for (const auto& str: make_range(begin, end)) {
    if (!first)
      result += separator;
    else
      first = false;

    result += str;
  }
  return result;
}

} // namespace text
} // namespace lib
