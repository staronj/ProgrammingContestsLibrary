#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"

namespace lib {
namespace text {

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
  auto predicate = [c](char d) {
    return c == d;
  };
  return split(begin, end, predicate, includeEmpty);
}

/**
 * Split overload taking character set chars instead of predicate.
 */
template <typename Iterator>
std::vector<std::string> split(Iterator begin, Iterator end, const char* chars, bool includeEmpty = false) {
  const char* const str_begin = chars;
  const char* const str_end = chars + strlen(chars);
  auto predicate = [str_begin, str_end](char c) {
    return std::find(str_begin, str_end, c) != str_end;
  };
  return split(begin, end, predicate, includeEmpty);
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

/**
 * Python-like function strip.
 *
 * For given range and predicate returns string with removed starting
 * and ending characters which satisfy predicate.
 *
 * Example:
 * <pre>
 * std::string text = "    Ala ma kota.\n\n\t";
 * strip(text.begin(), text.end(), std::isspace); // returns "Ala ma kota."
 * </pre>
 */
template <typename Iterator, typename Predicate>
std::string strip(Iterator begin, Iterator end, Predicate predicate) {
  while (begin != end && predicate(*begin))
    ++begin;

  while (begin != end && predicate(*std::prev(end)))
    --end;

  return std::string(begin, end);
}

/**
 * Python-like strip function.
 *
 * This version takes range and character.
 */
template <typename Iterator>
std::string strip(Iterator begin, Iterator end, char c) {
  auto predicate = [c] (char d) {
    return c == d;
  };
  return strip(begin, end, predicate);
}

/**
 * Python-like strip function.
 *
 * This version takes range and set of characters as c-string.
 */
template <typename Iterator>
std::string strip(Iterator begin, Iterator end, const char* chars) {
  const char* const str_begin = chars;
  const char* const str_end = chars + strlen(chars);
  auto predicate = [str_begin, str_end](char c) {
    return std::find(str_begin, str_end, c) != str_end;
  };
  return strip(begin, end, predicate);
}

} // namespace text
} // namespace lib
