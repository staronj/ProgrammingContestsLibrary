#pragma once
// Jakub Staroń, 2016

#include "headers.h"

namespace lib {

template <typename Value>
class Indexer {
public:
  using value_type = Value;
  using reference = const value_type&;
  using id_type = uint32;

  Indexer():
      value_to_id_() { }

  id_type get_id(const value_type& value) {
    auto it = value_to_id_.find(value);
    if (it == value_to_id_.end()) {
      id_type id = value_to_id_.size();
      it = value_to_id_.emplace(value, id).first;
      id_to_value_.push_back(it);
    }
    return it->second;
  }

  reference get_value(id_type id) {
    try {
      return id_to_value_.at(id)->first;
    }
    catch (const std::out_of_range&) {
      throw std::out_of_range("Indexer - id out of range");
    }
  }

  size_t size() const {
    return value_to_id_.size();
  }

private:
  using map_type = std::unordered_map<value_type, id_type>;
  map_type value_to_id_;
  std::vector<typename map_type::const_iterator> id_to_value_;
};

} // namespace lib
