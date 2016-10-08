#pragma once
// Jakub Staroń, 2016

#include "headers.h"

namespace lib {

/**
 * std::less for oldest max
 * std::less_or_equal for earliest max
 * std::greater for oldest min
 * std::greater_or_equal for earliest min
 */
template <typename Value, typename Comparator = std::less<Value>>
class MaxQueue {
public:
  using value_type = Value;
  using reference = const value_type&;
  using comparator_type = Comparator;
  using size_type = uint32;

  MaxQueue(comparator_type comparator = comparator_type()):
      comparator_(comparator), size_(0) { }

  void push(value_type value) {
    size_type count = 1;
    while (!queue_.empty() && comparator_(queue_.back().first, value)) {
      count += queue_.back().second;
      queue_.pop_back();
    }
    queue_.emplace_back(std::move(value), count);
    ++size_;
  }

  void pop() {
    --queue_.front().second;
    if (queue_.front().second == 0)
      queue_.pop_front();
    --size_;
  }

  reference max() const {
    return queue_.front().first;
  }

  bool empty() const {
    return queue_.empty();
  }

  size_type size() const {
    return size_;
  }

private:
  using entry_type = std::pair<value_type, size_type>;
  using queue_type = std::deque<entry_type>;

  queue_type queue_;
  comparator_type comparator_;
  size_type size_;
};

} // namespace lib
