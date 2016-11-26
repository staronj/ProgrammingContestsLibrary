#pragma once
// Jakub Staroń, 2016

#include "headers.h"

namespace lib {

/**
 * MaxQueue data structure.
 *
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

  /**
   * Constructs new empty queue.
   */
  MaxQueue(comparator_type comparator = comparator_type()):
      comparator_(comparator), size_(0) { }

  /**
   * Pushes new value to queue.
   */
  void push(value_type value) {
    size_type count = 1;
    while (!queue_.empty() && comparator_(queue_.back().first, value)) {
      count += queue_.back().second;
      queue_.pop_back();
    }
    queue_.emplace_back(std::move(value), count);
    ++size_;
  }

  /**
   * Constructs new value from given arguments and pushes it to queue.
   */
  template <typename... Args>
  void emplace(Args&&... args) {
    push(value_type(std::forward<Args>(args)...));
  }

  /**
   * Pops value from queue. If queue is empty behaviour is undefined.
   */
  void pop() {
    --queue_.front().second;
    if (queue_.front().second == 0)
      queue_.pop_front();
    --size_;
  }

  /**
   * Returns maximum value in queue.
   */
  reference max() const {
    return queue_.front().first;
  }

  /**
   * Returns true if queue is empty.
   */
  bool empty() const {
    return queue_.empty();
  }

  /**
   * Returns size of queue.
   */
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
