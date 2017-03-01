#pragma once
// Jakub Staroń, 2017

#include "headers.h"
#include "data_structures/random_access_list.h"


namespace lib {

/**
 * Data structure representing set of linear functions.
 *
 * Operations:
 * * insert(a, b) - inserts new linear function ax + b
 *   into the data structure
 * * evaluate(x) - computes max {a * x + b : a, b in set}
 *   (or min if one pass std::greater instead of std::less)
 * * find(x) - returns linear function that gives best result
 *
 * Note that for every value k inserted in this data structure
 * k * k should fit in value_type.
 */
template <typename ValueType, typename Comparator = std::less<ValueType>>
class Huller {
public:
  using value_type = ValueType;
  using function_type = std::pair<value_type, value_type>;
  using comparator_type = Comparator;
  using set_type = RandomAccessList<function_type>;
  using size_type = typename set_type::size_type;
  using difference_type = typename set_type::difference_type;

  /**
   * Constructs new empty Huller.
   */
  Huller(comparator_type comparator = comparator_type()):
    comparator_(comparator) { }

  /**
   * Inserts new linear function into set.
   */
  void insert(function_type function) {
    if (empty()) {
      set_.push_back(function);
      return;
    }

    // Find lower_bound on slopes.
    auto it = set_.find([&function, this](const function_type& other) {
      return !comparator_(other.first, function.first);
    });

    // Here we are dealing with function having the same slope.
    // If y-intercept of existing function is smaller than
    // the inserted, then we need to remove it.
    // In the other case we are under the existing function
    // and we do nothing.
    if (it != set_.end() && it->first == function.first) {
      if (comparator_(it->second, function.second))
        it = set_.erase(it);
      else
        return;
    }

    // Now we know, that if only it != begin(), then
    // it[-1]->first < function.first < it->first
    // We need to check if it[-1] and it[0] are not covering
    // inserted function.
    if (it != set_.end() && it != set_.begin() && isUnder(it[-1], function, it[0]))
      return;

    it = set_.insert(it, function);

    // Now we must perform cleanup on the right and on the left
    // of out newly inserted function.
    while (set_.end() - it >= 3 && isUnder(it[0], it[1], it[2]))
      set_.erase(it + 1);

    while (it - set_.begin() >= 2 && isUnder(it[-2], it[-1], it[0]))
      set_.erase(it - 1);
  }

  /**
   * Insert new linear function into set.
   *
   * Equivalent to insert(std::make_pair(a, b));   */
  void insert(value_type a, value_type b) {
    insert(std::make_pair(std::move(a), std::move(b)));
  }

  /**
   * Returns best result for given x.
   */
  value_type evaluate(const value_type& x) const {
    return evaluateOn(x, find(x));
  }

  /**
   * Returns linear function that produces best result for given x.
   *
   * If more than one function gives the best value
   * returns unspecified one of them.
   */
  const function_type& find(const value_type& x) const {
    assert(!empty());
    const auto size = set_.size();
    if (size == 1)
      return set_.front();

    size_type first = 0;
    size_type last = size - 1;
    while (first != last) {
      auto middle = (first + last) / 2;

      auto middle_it = set_.iterator_to(middle);

      const auto& f1 = evaluateOn(x, *middle_it);
      const auto& f2 = evaluateOn(x, *(middle_it + 1));
      if (comparator_(f1, f2))
        first = middle + 1;
      else
        last = middle;
    }
    return set_[first];
  }

  /**
   * Checks if there is at least one function in set.
   */
  bool empty() const {
    return set_.empty();
  }

  size_type elements() const {
    return set_.size();
  }

private:
  /**
   * Calculates a * x + b.
   */
  static value_type evaluateOn(const value_type& x, const function_type& function) {
    return function.first * x + function.second;
  }

  /**
   * Returns true if second linear function lies under
   * the first and third.
   */
  static bool isUnder(const function_type& first, const function_type& second, const function_type& third) {
    auto crossProduct = [](const function_type& x, const function_type& y) {
      return x.first * y.second - x.second * y.first;
    };

    return crossProduct(first, third) <= crossProduct(first, second) + crossProduct(second, third);
  }

  RandomAccessList<function_type> set_;
  comparator_type comparator_;
};

} // namespace lib
