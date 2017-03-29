#pragma once
// Jakub Staroń, 2016-2017

#include "headers.h"

namespace pcl {
namespace text {

/**
 * Computes lexicographically minimal sequence rotation.
 *
 * Returns iterator to position where rotation begins.
 * Iterator must be random-access iterator.
 * type of *Iterator must be comparable.
 *
 * Example:
 * <pre>
 * std::string s = "cba";
 * MinimalStringRotation(s.begin(), s.end()); // returns iterator pointing to 'a'
 * </pre>
 */
template <typename Iterator>
Iterator MinimalStringRotation(Iterator begin, Iterator end) {
  static_assert(
      std::is_same<
          typename std::iterator_traits<Iterator>::iterator_category,
          std::random_access_iterator_tag>::value,
      "Iterator must be random access iterator!"
  );

  using size_type = typename std::iterator_traits<Iterator>::difference_type;
  const size_type length = std::distance(begin, end);

  size_type i = 0;
  size_type j = 1;

  size_type k = 1;
  size_type p = 1;

  while (j + k <= (2 * length)) {
    const auto& a = begin[(i + k - 1) % length];
    const auto& b = begin[(j + k - 1) % length];
    if (a > b) {
      i = j++;
      k = p = 1;
    }
    else if (a < b) {
      j += k;
      k = 1;
      p = j - i;
    }
    else if (a == b && k != p) {
      k++;
    }
    else {
      j += p;
      k = 1;
    }
  }
  return begin + i;
}

/**
 * Computes lexicographically minimal string rotation.
 *
 * Example:
 * <pre>
 * std::string s = "cba";
 * MinimalStringRotation(s); // returns "acb"
 * </pre>
 */
std::string MinimalStringRotation(const std::string& text) {
  std::string result;
  result.reserve(text.size());
  auto it = MinimalStringRotation(text.begin(), text.end());
  result.append(it, text.end());
  result.append(text.begin(), it);
  return result;
}

} // namespace text
} // namespace pcl
