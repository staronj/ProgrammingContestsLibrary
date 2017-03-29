#pragma once
// Jakub Staroń, 2016-2017

#include "headers.h"

namespace pcl {

/**
 * Data structure for assigning consecutive identificators for values.
 *
 * Example:
 * <pre>
 * Indexer<std::string> indexer;
 * indeger.getID("Ala"); // returns 0
 * indeger.getID("ma"); // returns 1
 * indeger.getID("kota"); // returns 2
 * indeger.getID("Ala"); // returns 0
 * indeger.getValue(1); // returns "ma"
 * </pre>
 */
template <typename Value>
class Indexer {
public:
  using value_type = Value;
  using reference = const value_type&;
  using id_type = uint32;

  Indexer():
      value_to_id_() { }

  /**
   * Returns id of value.
   *
   * If value was already in set returns id of this value.
   * If value is not in set assign new id to value and returns it.
   */
  id_type getID(const value_type &value) {
    auto it = value_to_id_.find(value);
    if (it == value_to_id_.end()) {
      id_type id = value_to_id_.size();
      it = value_to_id_.emplace(value, id).first;
      id_to_value_.push_back(it);
    }
    return it->second;
  }

  /**
   * Returns value associated with given id.
   */
  reference getValue(id_type id) {
    try {
      return id_to_value_.at(id)->first;
    }
    catch (const std::out_of_range&) {
      throw std::out_of_range("Indexer - id out of range");
    }
  }

  /**
   * Returns number of stored (id, value) pairs.
   */
  size_t size() const {
    return value_to_id_.size();
  }

private:
  using map_type = std::unordered_map<value_type, id_type>;
  map_type value_to_id_;
  std::vector<typename map_type::const_iterator> id_to_value_;
};

} // namespace pcl
