// Michał Wiatrowski, Jakub Staroń, 2016

#pragma once

#include "headers.h"
#include "iterators.h"

namespace pcl {

/**
 * Computes prefix-suffix table of text using KMP algorithm.
 */
class KnuthMorrisPratt {
public:
  using ptr = std::shared_ptr<KnuthMorrisPratt>; /// Smart pointer to class.

  /**
   * Builds KMP class from sequence.
   */
  template<typename Iterator>
  KnuthMorrisPratt(Iterator begin, Iterator end) {
    uint32 size = uint32(std::distance(begin, end));
    P_.resize(size + 1);
    P_[0] = 0;

    if (size == 0)
      return;

    P_[1] = 0;

    for (auto i: range(1u, size)) {
      uint32 t = P_[i];
      while(t > 0 && *(begin + i) != *(begin + t))
        t = P_[t];

      if(*(begin + i) == *(begin + t))
        ++t;

      P_[i + 1] = t;
    }

    uint32 border = P_.back();
    borders_.clear();
    while (border != 0) {
      borders_.push_back(border);
      border = P_[border];
    }
    borders_.shrink_to_fit();
  }

  KnuthMorrisPratt(const KnuthMorrisPratt &) = delete;
  KnuthMorrisPratt &operator=(const KnuthMorrisPratt &) = delete;

  KnuthMorrisPratt(KnuthMorrisPratt &&other) :
      P_(std::move(other.P_)) {}

  /**
   * Returns prefix-suffix array.
   */
  const std::vector<uint32>& result() const {
    return P_;
  }

  /**
   * Returns vector of borders (prefix-suffixes) in decreasing order
   */
  const std::vector<uint32>& borders() const {
    return borders_;
  }

private:
  std::vector<uint32> P_;
  std::vector<uint32> borders_;
};

} // namespace pcl
