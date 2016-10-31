#pragma once
// Jakub Staroń, 2016

#include "hash.h"

namespace lib {

/**
 * Data structure for computing hashes of substrings.
 *
 * Hasher takes range of iterators
 * (it haven't got to be charasters, only injective
 * conversible to hash::scalar_type).
 *
 * Preprocess it in O(length) time.
 * Queries takes constant time.
 *
 * It's guaranteed, that if v is queried subsequence
 * then returned hash is equal to hash::hash(v), ie
 * we can use hasher to implement multipattern search.
 */
class Hasher {
public:
  using ptr = std::shared_ptr<Hasher>; /// Smart pointer to class
  using hash_type = hash::hash_type;
  using scalar_type = hash::scalar_type;
  using index_type = uint32;

  /**
   * Contstruct hasher from given sequence.
   *
   * Elements of sequence must be injectivly conversible
   * to hash::scalar_type.
   */
  template<typename Iterator>
  Hasher(Iterator begin, Iterator end) {
    size_ = uint32(std::distance(begin, end));
    preprocessInverses();

    hashes_.reserve(size_ + 1);
    hash_type sum = hash::zero;
    hash_type power = hash::one;
    hashes_.push_back(hash::zero);
    for (const auto& elem: make_range(begin, end)) {
      hash_type tmp = hash::multiply(power, scalar_type(elem));
      sum = hash::add(sum, tmp);
      hashes_.push_back(sum);
      power = hash::multiply(power, hash::multipler);
    }
  }

  Hasher(const Hasher&) = delete;
  Hasher& operator=(const Hasher&) = delete;

  Hasher(Hasher&& other):
      inverses_(std::move(other.inverses_)),
      hashes_(std::move(other.hashes_)) { }

  /**
   * Returns hash of subsequence.
   */
  hash_type getHash(index_type begin, index_type length) const {
    hash_type tmp = hash::subtract(hashes_[begin + length] , hashes_[begin]);
    return hash::multiply(tmp, inverses_[begin]);
  }

private:
  void preprocessInverses() {
    inverses_.resize(size_ + 1);
    inverses_[0] = hash::one;
    const hash_type inverse = inverseHash(hash::multipler);
    for(auto i: range(0u, size_))
      inverses_[i + 1] = hash::multiply(inverses_[i], inverse);
  }

  static hash_type inverseHash(const hash_type& hash) {
    return {numeric::inverse(hash.first), numeric::inverse(hash.second)};
  }

  std::vector<hash_type> inverses_;
  std::vector<hash_type> hashes_;
  index_type size_;
};

} // namespace lib
