#pragma once
// Jakub Staroń, 2016

#include "hash.h"

namespace lib {

class Hasher {
public:
  using hash_type = hash::hash_type;
  using scalar_type = hash::scalar_type;
  using index_type = uint32;

  Hasher() = default;

  Hasher(const Hasher&) = delete;
  Hasher& operator=(const Hasher&) = delete;

  Hasher(Hasher&& other):
      powers_(std::move(other.powers_)),
      hashes_(std::move(other.hashes_)) { }

  template<typename Iterator>
  void feed(Iterator begin, Iterator end) {
    size_ = uint32(std::distance(begin, end));
    powers_.clear();
    hashes_.clear();
    powers_.resize(size_ + 1);
    hashes_.resize(size_ + 1);

    powers_[0] = hash::one;
    for(auto i: range(0u, size_))
      powers_[i + 1] = hash::multiply(powers_[i], hash::multipler);

    hashes_[0] = hash::zero;
    for(auto i: range(0u, size_)) {
      hash_type tmp = hash::multiply(powers_[i], scalar_type(*begin));
      hashes_[i + 1] = hash::add(tmp, hashes_[i]);
      ++begin;
    }
  }

  hash_type get_hash(index_type begin, index_type length) {
    hash_type tmp = hash::subtract(hashes_[begin + length] , hashes_[begin]);
    return hash::multiply(tmp, powers_[size_ - begin]);
  }

private:
  std::vector<hash_type> powers_;
  std::vector<hash_type> hashes_;
  index_type size_;
};

} // namespace lib
