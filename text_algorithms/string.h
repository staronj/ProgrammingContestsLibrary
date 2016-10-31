#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"

namespace lib {
namespace text {

/**
 * Functor-like predicate to test equality to given value.
 */
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

/**
 * Returns is_equal_to_predicate for given value.
 *
 * Example:
 * <pre>
 * std::vector<int> v = {1, 2, 4, 16};
 * std::any_of(v.begin(), v.end(), is_equal_to(4)); // returns true
 * </pre>
 */
template <typename Value>
is_equal_to_predicate<Value> is_equal_to(Value value) {
  return is_equal_to_predicate<Value>(std::move(value));
}

/**
 * Functor-like predicate to test belonging to given set.
 */
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

/**
 * Returns is_in_set_predicate for given characters.
 *
 * Example:
 * <pre>
 * std::string s = "Ala ma kota.";
 * std::find_if(s.begin(), s.end(), is_in_set(",.;")); // returns iterator to '.'
 * </pre>
 */
is_in_set_predicate<char> is_in_set(const char* characters) {
  return is_in_set_predicate<char>(characters, characters + std::strlen(characters));
}

/**
 * Returns vector of strigns created by splitting given sequence using predicate.
 *
 * includeEmpty indicates if empty sequences are included.
 *
 * Example:
 * <pre>
 * std::string s = "Ala ma kota.";
 * split(s.begin(), s.end(), std::isspace); // returns {"Ala", "ma", "kota."}
 *
 * std::string s = " 123 456  789";
 * split(s.begin(), s.end(), std::isspace, true); // returns {"", "123", "456, "", "789"}
 * </pre>
 */
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

/**
 * Split overload taking character c instead of predicate.
 */
template <typename Iterator>
std::vector<std::string> split(Iterator begin, Iterator end, char c, bool includeEmpty = false) {
  return split(begin, end, is_equal_to(c), includeEmpty);
}

/**
 * Split overload taking character set chars instead of predicate.
 */
template <typename Iterator>
std::vector<std::string> split(Iterator begin, Iterator end, const char* chars, bool includeEmpty = false) {
  return split(begin, end, is_in_set(chars), includeEmpty);
}

/**
 * Python-like join function.
 *
 * Example:
 * <pre>
 * std::vector<std::string> v = {"Ala", "ma", "kota."};
 * join(" ", v.begin(), v.end()); // returns "Ala ma kota."
 * </pre>
 */
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
