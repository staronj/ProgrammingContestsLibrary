#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"
#include "numeric.h"

namespace lib {

template <typename Value, typename Comparator = std::less<Value>>
class RangeMinimumQuery {
public:
  using ptr = std::shared_ptr<RangeMinimumQuery>;
  static constexpr uint32 kMaximumNumberOfLevels = 30;
  using size_type = uint32;
  using index_type = size_type;
  using value_type = Value;
  using reference = const Value&;

  template <typename Iterator>
  RangeMinimumQuery(Iterator begin, Iterator end, Comparator comparator = Comparator()):
      values_(begin, end), comparator_(comparator) {
    calculate();
  }

  index_type minimum(index_type first, index_type last) {
    constexpr const char* kInvalidRange = "RangeMinimumQuery - invalid range!";
    if (first > last)
      throw std::invalid_argument(kInvalidRange);
    else if (first >= values_.size() || last >= values_.size())
      throw std::out_of_range(kInvalidRange);

    const size_type length = last - first + 1;
    const size_type length_log = integer_log2(length);
    const size_type segment_size = (1u << length_log);

    return minimum_index(length_log, first, last + 1 - segment_size);
  }

  size_type size() const {
    return uint32(values_.size());
  }

private:
  void calculate() {
    segments_[0].assign(counting_iterator<uint32>(0), counting_iterator<uint32>(size()));
    const size_type log_size = integer_log2(size());
    for(auto i : range<size_type>(1, log_size + 1))
    {
      const size_type segment_length = (1u << i);
      const size_type segments_count = size() - segment_length + 1;
      segments_[i].reserve(segments_count);
      for (auto j : range<index_type>(0, segments_count))
        segments_[i][j] = minimum_index(i - 1, j, j + segment_length / 2);
    }
  }

  index_type minimum_index(size_type level, index_type first_segment, index_type second_segment) const {
    const index_type first_index = segments_[level][first_segment];
    const index_type second_index = segments_[level][second_segment];
    reference first_value = values_[first_index];
    reference second_value = values_[second_index];

    return (comparator_(first_value, second_value)? first_index : second_index);
  }

  std::vector<Value> values_;
  std::vector<index_type> segments_[kMaximumNumberOfLevels];
  Comparator comparator_;
};

template <typename Value, typename Comparator>
constexpr uint32 RangeMinimumQuery<Value, Comparator>::kMaximumNumberOfLevels;

} // namespace lib
