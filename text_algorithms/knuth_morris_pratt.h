// Michał Wiatrowski, Jakub Staroń, 2016

#pragma once

#include "headers.h"
#include "iterators.h"

namespace lib {

class KnuthMorrisPratt {
public:
  KnuthMorrisPratt() = default;

  KnuthMorrisPratt(const KnuthMorrisPratt &) = delete;

  KnuthMorrisPratt &operator=(const KnuthMorrisPratt &) = delete;

  KnuthMorrisPratt(KnuthMorrisPratt &&other) :
      P_(std::move(other.P_)) {}

  template<typename Iterator>
  void feed(Iterator begin, Iterator end) {
    uint32 size = uint32(std::distance(begin, end));
    P_.resize(size + 1);
    P_[0] = 0;

    if (size == 0)
      return;

    for (auto i: range(1u, size)) {
      uint32 t = P_[i];
      while(t > 0 && *(begin + i) != *(begin + t))
        t = P_[t];

      if(*(begin + i) == *(begin + t))
        ++t;

      P_[i + 1] = t;
    }
  }

  const std::vector<uint32>& result() const {
    return P_;
  }

private:
  std::vector<uint32> P_;
};

} // namespace lib
