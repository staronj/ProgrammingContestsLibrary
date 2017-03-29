#pragma once
// Jakub Staroń, 2016-2017

#include "headers.h"

namespace pcl {

/**
 * MaxStack data structure.
 *
 * std::less for oldest max
 * std::less_or_equal for earliest max
 * std::greater for oldest min
 * std::greater_or_equal for earliest min
 */
template <typename Value, typename Comparator = std::less<Value>>
class MaxStack {
public:
  using value_type = Value;
  using reference = const value_type&;
  using comparator_type = Comparator;
  using size_type = uint32;

  /**
   * Constructs new empty stack.
   */
  MaxStack(comparator_type comparator = comparator_type()):
      comparator_(comparator) { }

  /**
   * Pushes new value to stack.
   */
  void push(value_type value) {
    uint32 max_index =
        (empty() || comparator_(max(), value))? size_type(stack_.size()) : stack_.back().second;
    stack_.emplace_back(value, max_index);
  }

  /**
   * Constructs new value from given arguments and pushes it to stack.
   */
  template <typename... Args>
  void emplace(Args&&... args) {
    push(value_type(std::forward<Args>(args)...));
  }

  /**
   * Pops value from stack. If stack is empty behaviour is undefined.
   */
  void pop() {
    stack_.pop_back();
  }

  /**
   * Returns maximum value in stack.
   */
  reference max() const {
    return stack_[stack_.back().second].first;
  }

  /**
   * Returns true if stack is empty.
   */
  bool empty() const {
    return stack_.empty();
  }

  /**
   * Returns size of stack.
   */
  size_type size() const {
    return size_type(stack_.size());
  }

private:
  using entry_type = std::pair<value_type, size_type>;
  using stack_type = std::vector<entry_type>;

  stack_type stack_;
  comparator_type comparator_;
};

} // namespace pcl
