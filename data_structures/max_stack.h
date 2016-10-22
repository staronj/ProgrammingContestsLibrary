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
class MaxStack {
public:
  using value_type = Value;
  using reference = const value_type&;
  using comparator_type = Comparator;
  using size_type = uint32;

  MaxStack(comparator_type comparator = comparator_type()):
      comparator_(comparator) { }

  void push(value_type value) {
    uint32 max_index =
        (empty() || comparator_(max(), value))? stack_.size() : stack_.back().second;
    stack_.emplace_back(value, max_index);
  }

  void pop() {
    stack_.pop_back();
  }

  reference max() const {
    return stack_[stack_.back().second].first;
  }

  bool empty() const {
    return stack_.empty();
  }

  size_type size() const {
    return stack_.size();
  }

private:
  using entry_type = std::pair<value_type, size_type>;
  using stack_type = std::vector<entry_type>;

  stack_type stack_;
  comparator_type comparator_;
};

} // namespace lib
