#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"

namespace lib {

namespace utility {
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

inline uint32 trailing_zeros_count(uint32 n) {
  return __builtin_ctz(n);
}

inline uint32 leading_zeros_count(uint32 n) {
  return __builtin_clz(n);
}

} // namespace utility


using namespace utility;

template<uint8 logM>
class VanEmdeBoasTree {
public:
  using integer_type = typename IntegerHelper<logM>::integer_type;

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
    set_empty();
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
    } else if (minimum == n)
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
    } else if (minimum == maximum) {
      set_empty();
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
    } else {
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
    } else if (summary.first() < high(n)) {
      integer_type i = summary.predecessor(high(n));
      return (i << high_shift) + S[i].last();
    } else {
      return minimum;
    }
  }

private:

  void set_empty() {
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

template<>
class VanEmdeBoasTree<5> {
public:
  using integer_type = uint32;

  VanEmdeBoasTree() {
    data = 0;
  }

  VanEmdeBoasTree(const VanEmdeBoasTree&) = delete;
  VanEmdeBoasTree(const VanEmdeBoasTree&&) = delete;
  VanEmdeBoasTree& operator=(const VanEmdeBoasTree&) = delete;

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
    return trailing_zeros_count(data);
  }

  integer_type last() const {
    assert(!empty());
    return 31 - leading_zeros_count(data);
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
    uint32 shiftedData = (data >> (n + 1));
    return n + 1 + trailing_zeros_count(shiftedData);
  }

  integer_type predecessor(integer_type n) const {
    assert(!empty());
    uint32 shiftedData = (data << (32 - n));
    return n - 1 - leading_zeros_count(shiftedData);
  }

  integer_type data;
};

template<>
class VanEmdeBoasTree<4> {
public:
  using integer_type = uint16;

  VanEmdeBoasTree() {
    data = 0;
  }

  VanEmdeBoasTree(const VanEmdeBoasTree&) = delete;
  VanEmdeBoasTree(const VanEmdeBoasTree&&) = delete;
  VanEmdeBoasTree& operator=(const VanEmdeBoasTree&) = delete;

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
    return trailing_zeros_count(data);
  }

  integer_type last() const {
    assert(!empty());
    return 31 - leading_zeros_count(data);
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
    uint32 shiftedData = (data >> (n + 1));
    return n + 1 + trailing_zeros_count(shiftedData);
  }

  integer_type predecessor(integer_type n) const {
    assert(!empty());
    uint32 shiftedData = (data << (32 - n));
    return n - 1 - leading_zeros_count(shiftedData);
  }

  integer_type data;
};

template<>
class VanEmdeBoasTree<3> {
public:
  using integer_type = uint8;

  VanEmdeBoasTree() {
    data = 0;
  }

  VanEmdeBoasTree(const VanEmdeBoasTree&) = delete;
  VanEmdeBoasTree(const VanEmdeBoasTree&&) = delete;
  VanEmdeBoasTree& operator=(const VanEmdeBoasTree&) = delete;

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
    uint8 n = 0;
    while (!find(n)) n++;
    return n;
  }

  integer_type last() const {
    assert(!empty());
    uint8 n = 7;
    while (!find(n)) n--;
    return n;
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
    do {
      n++;
    } while (!find(n));
    return n;
  }

  integer_type predecessor(integer_type n) const {
    assert(!empty());
    do {
      n--;
    } while (!find(n));
    return n;
  }

  integer_type data;
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

  VanEmdeBoasSet():
      tree_(new tree_type()) { }

  VanEmdeBoasSet(const VanEmdeBoasSet &) = delete;
  VanEmdeBoasSet &operator=(const VanEmdeBoasSet &) = delete;


  /**
   * Returns true if n is in set.
   */
  bool find(integer_type n) const {
    if (n >= end_const)
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
    return (end_const - 1);
  }

  /**
   * Removes n from set.
   * Returns true if n was in set.
   */
  bool erase(integer_type n) {
    if (n >= end_const)
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
    if (n >= end_const)
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
    using iterator_category = std::bidirectional_iterator_tag;
    using container_pointer = const VanEmdeBoasSet*;

    basic_iterator(container_pointer container, value_type position) {
      this->container = container;
      this->position = position;
    }

    self_type operator++() {
      position = iterator_helper::increment(container, position);
      return *this;
    }

    self_type operator--() {
      position = iterator_helper::decrement(container, position);
      return *this;
    }

    value_type operator*() const {
      return this->position;
    }

    friend bool operator==(const self_type &lhs, const self_type &rhs) {
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
        return end_const;
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
      if (position == end_const || position < first)
        tree->illegalOperation();
      else if (position == first)
        return end_const;
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
    return empty()? end() : iterator(this, first());
  }

  iterator end() const {
    return iterator(this, end_const);
  }

  reverse_iterator rbegin() const {
    return empty()? rend() : reverse_iterator(this, last());
  }

  reverse_iterator rend() const {
    return reverse_iterator(this, end_const);
  }

private:
  void outOfRange() const {
    throw std::out_of_range("VanEmdeBoasSet: outOfRange");
  }

  void illegalOperation() const {
    throw std::runtime_error("VanEmdeBoasSet: illegalOperation");
  }

  static constexpr value_type end_const = (1u << logM) - 1;

  std::unique_ptr<tree_type> tree_;
  size_type size_ = 0;
};

} // namespace lib
