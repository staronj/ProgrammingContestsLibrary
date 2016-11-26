#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "operators.h"
#include "numeric.h"

namespace lib {

namespace detail {
template<uint8 N>
struct IntegerHelper : public IntegerHelper<N - 1> {
};

template<>
struct IntegerHelper<1> {
  using integer_type = uint8;
};

template<>
struct IntegerHelper<9> {
  using integer_type = uint16;
};

template<>
struct IntegerHelper<17> {
  using integer_type = uint32;
};

template<>
struct IntegerHelper<33> {
  using integer_type = uint64;
};

template<uint8 BytesNeeded>
using IntegerType = typename IntegerHelper<BytesNeeded>::integer_type;

} // namespace detail

template<uint8 logM>
class VanEmdeBoasTree {
public:
  using integer_type = detail::IntegerType<logM>;

  enum Constants {
    ceiling_half_logM = (logM + 1) / 2,
    floor_half_logM = logM / 2,

    subtree_size = ceiling_half_logM,
    summary_size = floor_half_logM,
    subtrees_number = (1u << summary_size),
    high_shift = subtree_size,
    low_mask = (1u << high_shift) - 1
  };

  using subtree_type = VanEmdeBoasTree<subtree_size>;
  using summary_type = VanEmdeBoasTree<summary_size>;

  VanEmdeBoasTree() {
    setEmpty();
  }

  VanEmdeBoasTree(const VanEmdeBoasTree&) = delete;
  VanEmdeBoasTree(const VanEmdeBoasTree&&) = delete;
  VanEmdeBoasTree& operator=(const VanEmdeBoasTree&) = delete;

  bool empty() const {
    return minimum > maximum;
  }

  bool find(integer_type n) const {
    if (empty())
      return false;
    else if (minimum == n)
      return true;
    else
      return S[high(n)].find(low(n));
  }

  bool insert(integer_type n) {
    if (empty()) {
      minimum = maximum = n;
      return true;
    }
    else if (minimum == n)
      return false;

    if (n > maximum)
      maximum = n;
    else if (n < minimum)
      std::swap(n, minimum);

    summary.insert(high(n));
    return S[high(n)].insert(low(n));
  }

  integer_type last() const {
    assert(!empty());
    return maximum;
  }

  integer_type first() const {
    assert(!empty());
    return minimum;
  }

  bool erase(integer_type n) {
    if (empty() || n < minimum || n > maximum) {
      return false;
    }
    else if (minimum == maximum) {
      setEmpty();
      return true;
    }

    if (minimum == n) {
      const integer_type i = summary.first();
      n = minimum = (i << high_shift) + S[i].first();
    }

    bool result = S[high(n)].erase(low(n));

    if (S[high(n)].empty())
      summary.erase(high(n));

    if (n == maximum) {
      if (summary.empty())
        maximum = minimum;
      else {
        integer_type i = summary.last();
        maximum = (i << high_shift) + S[i].last();
      }
    }

    return result;
  }

  integer_type successor(integer_type n) const {
    assert(!empty());
    assert(n < maximum);
    if (n < minimum)
      return minimum;

    if (!S[high(n)].empty() && low(n) < S[high(n)].last()) {
      return (high(n) << high_shift) + S[high(n)].successor(low(n));
    }
    else {
      integer_type i = summary.successor(high(n));
      return (i << high_shift) + S[i].first();
    }
  }

  integer_type predecessor(integer_type n) const {
    assert(!empty());
    assert(n > minimum);
    if (n > maximum)
      return maximum;

    if (!S[high(n)].empty() && low(n) > S[high(n)].first()) {
      return (high(n) << high_shift) + S[high(n)].predecessor(low(n));
    }
    else if (summary.first() < high(n)) {
      integer_type i = summary.predecessor(high(n));
      return (i << high_shift) + S[i].last();
    }
    else {
      return minimum;
    }
  }

private:

  void setEmpty() {
    minimum = 1;
    maximum = 0;
  }

  integer_type low(integer_type n) const {
    return n & low_mask;
  }

  integer_type high(integer_type n) const {
    return n >> high_shift;
  }

  integer_type minimum, maximum;
  subtree_type S[subtrees_number];
  summary_type summary;
};

template<uint8 logM>
class SmallVanEmdeBoasTree {
public:
  using integer_type = detail::IntegerType<1u << logM>;

  SmallVanEmdeBoasTree() = default;

  SmallVanEmdeBoasTree(const SmallVanEmdeBoasTree&) = delete;
  SmallVanEmdeBoasTree(const SmallVanEmdeBoasTree&&) = delete;
  SmallVanEmdeBoasTree& operator=(const SmallVanEmdeBoasTree&) = delete;

  bool find(integer_type n) const {
    return (data & (1u << n)) != 0;
  }

  bool erase(integer_type n) {
    bool result = find(n);
    data &= ~(1u << n);
    return result;
  }

  integer_type first() const {
    assert(!empty());
    return least_significant_one(data);
  }

  integer_type last() const {
    assert(!empty());
    return most_significant_one(data);
  }

  bool insert(integer_type n) {
    bool result = !find(n);
    data |= (1u << n);
    return result;
  }

  bool empty() const {
    return (data == 0);
  }

  integer_type successor(integer_type n) const {
    assert(!empty());
    uint32 mask = (1u << (n + 1)) - 1;
    return least_significant_one(data & ~mask);
  }

  integer_type predecessor(integer_type n) const {
    assert(!empty());
    uint32 mask = (1u << n) - 1;
    return most_significant_one(data & mask);
  }

private:
  integer_type data = 0;
};

template<>
class VanEmdeBoasTree<5> : public SmallVanEmdeBoasTree<5> {
};

template<>
class VanEmdeBoasTree<4> : public SmallVanEmdeBoasTree<4> {
};

template<>
class VanEmdeBoasTree<3> : public SmallVanEmdeBoasTree<3> {
};


template<uint8 logM>
class VanEmdeBoasSet {
public:
  using tree_type = VanEmdeBoasTree<logM>;
  using integer_type = typename tree_type::integer_type;
  using size_type = uint32;
  using value_type = integer_type;

  friend class iterator_helper;

  friend class reverse_iterator_helper;

  VanEmdeBoasSet() :
      tree_(new tree_type()) {}

  VanEmdeBoasSet(const VanEmdeBoasSet&) = delete;
  VanEmdeBoasSet& operator=(const VanEmdeBoasSet&) = delete;

  VanEmdeBoasSet(VanEmdeBoasSet&&) = default;
  VanEmdeBoasSet& operator=(VanEmdeBoasSet&&) = default;

  /**
   * Returns true if n is in set.
   */
  bool find(integer_type n) const {
    if (n >= kEnd)
      outOfRange();

    return tree_->find(n);
  }

  /**
   * Returns true if set is empty.
   */
  bool empty() const {
    return tree_->empty();
  }

  /**
   * Returns number of elements in set.
   */
  size_type size() const {
    return size_;
  }

  /**
   * Returns size in bytes of underlying data structure.
   */
  constexpr static size_type sizeOfTree() {
    return sizeof(tree_type);
  }

  /**
   * Returns maximum allowed value for this tree.
   */
  constexpr static size_type maxValue() {
    return (kEnd - 1);
  }

  /**
   * Removes n from set.
   * Returns true if n was in set.
   */
  bool erase(integer_type n) {
    if (n >= kEnd)
      outOfRange();

    bool result = tree_->erase(n);
    if (result)
      size_--;
    return result;
  }

  /**
   * Inserts n into set.
   * Returns true if n was not already in set.
   */
  bool insert(integer_type n) {
    if (n >= kEnd)
      outOfRange();

    bool result = tree_->insert(n);
    if (result)
      size_++;
    return result;
  }

  /**
   * Returns smallest element in set.
   * If set is empty throws.
   */
  integer_type first() const {
    if (empty())
      illegalOperation();

    return tree_->first();
  }

  /**
   * Returns biggest element in set.
   * If set is empty throws.
   */
  integer_type last() const {
    if (empty())
      illegalOperation();

    return tree_->last();
  }

  /**
   * For given n returns smallest k in set
   * that is bigger than n.
   *
   * If no such k exists, throws.
   */
  integer_type successor(integer_type n) const {
    if (empty() || n >= last())
      illegalOperation();

    return tree_->successor(n);
  }

  /**
   * For given n returns biggest k in set
   * that is smaller than n.
   *
   * If no such k exists, throws.
   */
  integer_type predecessor(integer_type n) const {
    if (empty() || n <= first())
      illegalOperation();

    return tree_->predecessor(n);
  }

  template<class iterator_helper>
  class basic_iterator {
  public:
    using self_type = basic_iterator;
    using value_type = integer_type;
    using reference = const value_type&;
    using pointer = const value_type*;
    using difference_type = int64;
    using iterator_category = std::bidirectional_iterator_tag;
    using container_pointer = const VanEmdeBoasSet*;

    basic_iterator(container_pointer container, value_type position) {
      this->container = container;
      this->position = position;
    }

    self_type& operator++() {
      position = iterator_helper::increment(container, position);
      return *this;
    }

    self_type& operator--() {
      position = iterator_helper::decrement(container, position);
      return *this;
    }

    reference operator*() const {
      return this->position;
    }

    friend bool operator==(const self_type& lhs, const self_type& rhs) {
      return lhs.position == rhs.position;
    }

  private:
    value_type position;
    container_pointer container;
  };

  struct forward_iterator_helper {
    using value_type = integer_type;
    using tree_pointer = const VanEmdeBoasSet*;

    static value_type increment(tree_pointer tree, value_type position) {
      value_type last = tree->last();
      if (position > last)
        tree->illegalOperation();
      else if (position == last)
        return kEnd;
      else
        return tree->successor(position);
    }

    static value_type decrement(tree_pointer tree, value_type position) {
      if (position <= tree->first())
        tree->illegalOperation();
      else
        return tree->predecessor(position);
    }
  };

  struct reverse_iterator_helper {
    using value_type = integer_type;
    using tree_pointer = const VanEmdeBoasSet*;

    static value_type increment(tree_pointer tree, value_type position) {
      value_type first = tree->first();
      if (position == kEnd || position < first)
        tree->illegalOperation();
      else if (position == first)
        return kEnd;
      else
        return tree->predecessor(position);
    }

    static value_type decrement(tree_pointer tree, value_type position) {
      if (position >= tree->last())
        tree->illegalOperation();
      else
        return tree->successor(position);
    }
  };

  using iterator = basic_iterator<forward_iterator_helper>;
  using reverse_iterator = basic_iterator<reverse_iterator_helper>;

  iterator begin() const {
    return empty() ? end() : iterator(this, first());
  }

  iterator end() const {
    return iterator(this, kEnd);
  }

  reverse_iterator rbegin() const {
    return empty() ? rend() : reverse_iterator(this, last());
  }

  reverse_iterator rend() const {
    return reverse_iterator(this, kEnd);
  }

private:
  static constexpr const char kOutOfRange[] = "VanEmdeBoasSet: outOfRange";
  static constexpr const char kIllegalOperation[] = "VanEmdeBoasSet: illegalOperation";

  void outOfRange() const {
    throw std::out_of_range(kOutOfRange);
  }

  void illegalOperation() const {
    throw std::runtime_error(kIllegalOperation);
  }

  static constexpr value_type kEnd = (1u << logM) - 1;

  std::unique_ptr<tree_type> tree_;
  size_type size_ = 0;
};

template <uint8 logM>
constexpr typename VanEmdeBoasSet<logM>::value_type VanEmdeBoasSet<logM>::kEnd;

} // namespace lib
