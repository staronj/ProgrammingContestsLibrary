#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"
#include "numeric.h"

namespace lib {

class BitArray {
  using block_type = uint64;
  using data_pointer = std::unique_ptr<block_type []>;
  using size_type = size_t;

  static constexpr size_type block_size = sizeof(block_type) * 8;

public:

  /**
   * Creates new empty table.
   */
  BitArray(): size_(0) { }

  /**
   * Creates table and resizes to given size.
   */
  BitArray(size_type size, bool value): BitArray() {
    assign(size, value);
  }

  BitArray(BitArray&& other): BitArray() {
    *this = std::move(other);
  }

  BitArray(const BitArray& other): BitArray() {
    *this = other;
  }

  BitArray& operator=(BitArray&& other) {
    if (this == &other)
      return *this;

    clear();
    size_ = other.size_;
    other.size_ = 0;
    blocks_ = std::move(other.blocks_);
    return *this;
  }

  BitArray& operator=(const BitArray& other) {
    if (this == &other)
      return *this;

    clear();
    construct(other.size_);
    if (size_ > 0)
      std::memcpy(blocks_.get(), other.blocks_.get(), sizeof(block_type) * block_count());
    return *this;
  }

  /**
   * Sets value of given bit.
   */
  void set(size_type index, bool value) {
    block_type mask = block_type(1uLL << (index % block_size));
    size_type block_index = index / block_size;

    if (value)
      blocks_[block_index] |= mask;
    else
      blocks_[block_index] &= ~mask;
  }

  /**
   * Gets value of given bit.
   */
  bool get(size_type index) const {
    return (blocks_[index / block_size] & (1uLL << (index % block_size))) != 0uLL;
  }

  /**
   * Returns true if bit_table is in uninitialized (empty) state.
   */
  bool empty() const {
    return !bool(blocks_);
  }

  /**
   * Returns number of bits stored.
   */
  size_type size() const {
    return size_;
  }

  /**
   * Sets all bits to zero.
   */
  void reset(bool value) {
    if (empty())
      return;

    uint8 byte_value = value? ~uint8(0) : uint8(0);
    std::memset(blocks_.get(), byte_value, sizeof(block_type) * block_count());
    trim_last_block();
  }

  /**
   * Frees memory. Table is now in raw state.
   */
  void clear() {
    if (size_ > 0) {
      size_ = 0;
      blocks_.reset();
    }
  }

  /**
   * If new_size is different than size releases
   * memory and allocates new for given number of bits.
   * Bits are initialized to given value.
   */
  void assign(size_type new_size, bool value) {
    if (new_size != size_) {
      clear();
      construct(new_size);
    }
    reset(value);
  }

  /**
   * Returns number of bits set to true.
   */
  size_type count() const {
    size_type result = 0;
    for (auto i: range(size_type(0), block_count()))
      result += pop_count(blocks_[i]);
    return result;
  }

  /**
   * Flips all bits.
   */
  void flip() {
    if (empty())
      return;

    for (auto i: range(size_type(0), block_count()))
      blocks_[i] = ~blocks_[i];
    trim_last_block();
  }

  /**
   * Returns array copy with flipped bits.
   */
  BitArray operator~() const {
    BitArray tmp(*this);
    tmp.flip();
    return tmp;
  }

  /**
   * Returns true if any bit is set to true.
   */
  bool any() const {
    if (empty())
      return false;

    for (auto i: range(size_type(0), block_count()))
      if (blocks_[i] != 0)
        return true;
    return false;
  }

  /**
   * Returns true if all bits are set to false.
   */
  bool none() const {
    return !any();
  }

  /**
   * Returns true if all bits are set to true.
   */
  bool all() const {
    if (empty())
      return true;

    for (auto i: range(size_type(0), block_count() - 1))
      if (blocks_[i] == 0)
        return false;

    size_type bits_in_last_block = size_ % block_size;
    block_type last_block_mask = (1uLL << bits_in_last_block) - 1;
    return (blocks_[block_count() - 1] | ~last_block_mask) == ~block_type(0u);
  }

  friend bool operator==(const BitArray& lhs, const BitArray& rhs) {
    if (lhs.size() != rhs.size())
      return false;

    return std::memcmp(lhs.blocks_.get(), rhs.blocks_.get(), sizeof(block_type) * lhs.block_count()) == 0;
  }

  /**
   * Returns bit or on two BitArrays.
   */
  friend BitArray operator|(const BitArray& lhs, const BitArray& rhs) {
    if (lhs.size() != rhs.size())
      throw std::logic_error("BitArrays not of the same size!");
    BitArray result(lhs.size(), false);
    for (auto i: range(size_type(0), result.block_count()))
      result.blocks_[i] = lhs.blocks_[i] | rhs.blocks_[i];

    return result;
  }

  /**
   * Returns bit and on two BitArrays.
   */
  friend BitArray operator&(const BitArray& lhs, const BitArray& rhs) {
    if (lhs.size() != rhs.size())
      throw std::logic_error("BitArrays not of the same size!");
    BitArray result(lhs.size(), false);
    for (auto i: range(size_type(0), result.block_count()))
      result.blocks_[i] = lhs.blocks_[i] & rhs.blocks_[i];

    return result;
  }

private:
  void construct(size_type new_size) {
    size_ = new_size;
    if (size_ > 0)
      blocks_.reset(new block_type[block_count()]);
  }

  void trim_last_block() {
    size_type bits_in_last_block = size_ % block_size;
    block_type last_block_mask = (1uLL << bits_in_last_block) - 1;
    blocks_[block_count() - 1] &= last_block_mask;
  }

  size_type block_count() const {
    return (size_ + block_size - 1) / block_size;
  }

  data_pointer blocks_;
  size_type size_;
};

} // namespace lib
