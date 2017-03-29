#pragma once
// Jakub Staroń, 2016-2017

#include "headers.h"

namespace pcl {

/**
 * Returns vector of length 2 * (end - being) - 1 with
 * diameters of both odd and even palindromes.
 *
 * On even positions (0, 2, 4, ...) there will be diameters
 * of odd palindromes and on odd positions (1, 3, 5, ...) - even ones.
 *
 * Example:
 * <pre>
 * std::string text = "aaabba";
 * Palindromes(text.begin(), text.end()); // returns {1, 2, 3, 2, 1, 0, 1, 4, 1, 0, 1}
 * </pre>
 */
template <typename Iterator>
std::vector<uint32> Palindromes(Iterator begin, Iterator end) {
  static_assert(
      std::is_same<
          typename std::iterator_traits<Iterator>::iterator_category,
          std::random_access_iterator_tag>::value,
      "Iterator must be random access!");

  int32 length = std::distance(begin, end);
  std::vector<uint32> result(2 * length - 1, 0);

  auto T = [begin](int32 i) {
    return begin[i/2];
  };

  auto inRange = [length](int32 i) {
    return 0 <= i && i < 2 * length;
  };

  int32 j = 0; // How big is current palindrome
  for (int32 i = 0; i < 2 * length - 1; ) {
    // Lets expand current palindrome
    while (inRange(i - j) && inRange(i + j + 1) && T(i - j) == T(i + j + 1))
      j++;

    result[i] = j;

    // lets set palindromes we know diameter of
    int32 k = 1;
    for( ; k < j && result[i - k] != j - k; ++k)
      result[i + k] = std::min<uint32>(result[i - k], j - k);

    //
    j = std::max(0, j - k);

    i += k;
  }

  return result;
}

} // namespace pcl
