#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"
#include "data_structures/avl_tree.h"
#include "iterators/random_access_iterator.h"

namespace lib {

/**
 * Implementation of random access list using avl trees.
 *
 *
 * Let n denote the numer of elements in data structure.
 *
 * Memory complexity O(n).
 *
 * Time complexity:
 * * element access O(log n)
 * * insert, erase O(log n)
 * * size, empty O(1)
 * * clear O(n)
 */
template <typename ValueType>
class RandomAccessList {
public:
  using size_type = uint32;
  using difference_type = int32;
  using value_type = ValueType;
  using reference = ValueType&;
  using const_reference = const ValueType&;
  using pointer = ValueType*;
  using const_pointer = const ValueType*;

private:
  struct ListNode : avl::Node<ListNode> {
    using node_pointer = typename avl::Node<ListNode>::node_pointer;

    ListNode(value_type value) :
        value_(std::move(value)) {}

    static size_type size(node_pointer node) {
      return (node != nullptr) ? node->size_ : uint32(0);
    }

    reference value() {
      return value_;
    }

    void update() {
      size_ = size(this->left()) + size(this->right()) + 1;
    }

  private:
    value_type value_;
    size_type size_ = 1;
  };

  using node_pointer = typename ListNode::node_pointer;

  template <typename Type>
  struct iterator_helper {
    using self_type = iterator_helper;
    using container_type = RandomAccessList;
    using container_pointer = const container_type*;
    using value_type = Type;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = RandomAccessList::difference_type;

    friend class RandomAccessList;

    iterator_helper():
        iterator_helper(nullptr, nullptr) { }

    iterator_helper(node_pointer node, container_pointer container):
        node_(node), container_(container) { }

    void next() {
      node_ = avl::next_inorder(node_);
    }

    void prev() {
      if (node_ == nullptr)
        node_ = avl::last(container_->root_);
      else
        node_ = avl::prev_inorder(node_);
    }

    reference value() const {
      return node_->value();
    }

    pointer ptr() const {
      return &value();
    }

    bool equal(const self_type& other) const {
      return node_ == other.node_;
    }

    void advance(difference_type diff) {
      if (diff == 1)
        next();
      else if (diff == -1)
        prev();
      else if (diff != 0) {
        auto index = container_->CalculateIndex(node_);
        node_ = container_->GoAtIndex(index + diff);
      }
    }

    difference_type difference(const self_type& other) const {
      difference_type lhs = container_->CalculateIndex(node_);
      difference_type rhs = container_->CalculateIndex(other.node_);
      return lhs - rhs;
    }

    bool less(const self_type& other) const {
      return difference(other) < 0;
    }

  private:
    node_pointer node_;
    container_pointer container_;
  };

public:
  using iterator = random_access_iterator<iterator_helper<value_type>>;
  using const_iterator = random_access_iterator<iterator_helper<const value_type>>;

  /**
   * Construct new, empty list.
   *
   * Time complexity O(1).
   */
  RandomAccessList() = default;

  /**
   * Constructs new list with elements from range [begin, end).
   *
   * Denote (end - begin) by n.
   * Time complexity O(n log n).
   */
  template<class InputIt>
  RandomAccessList(InputIt begin, InputIt end) {
    assign(begin, end);
  }

  /**
   * Constructs new list with elements from other list.
   *
   * Denote other.size() by n.
   * Time complexity O(n log n).
   */
  RandomAccessList(const RandomAccessList& other) {
    operator=(other);
  }

  /**
   * Constructs new list from rvalue of other list.
   *
   * The other list becomes empty as a result.
   *
   * Time complexity O(1).
   */
  RandomAccessList(RandomAccessList&& other) {
    operator=(std::move(other));
  }

  /**
   * Constructs new list from initializer list.
   *
   * Denote init.size() by n.
   * Time complexity O(n log n).
   */
  RandomAccessList(std::initializer_list<value_type> init) {
    assign(init);
  }

  /**
   * Destroys list.
   *
   * Denote list.size() by n.
   * Time complexity O(n).
   */
  ~RandomAccessList() {
    clear();
  }

  /**
   * Assigns one list into another.
   *
   * Returns reference to lhs.
   *
   * Denote list.size() by n and other.size() by m.
   * Time complexity O(n + m log m).
   */
  RandomAccessList& operator=(RandomAccessList& other) {
    assign(other.begin(), other.end());
    return *this;
  }

  /**
   * Moves one list into another.
   *
   * The other list becomes empty as a result.
   * Returns reference to lhs.
   *
   * Time complexity O(1).
   */
  RandomAccessList& operator=(RandomAccessList&& other) {
    clear();
    root_ = other.root_;
    other.root_ = nullptr;
    return *this;
  }

  /**
   * Assigns elements from initializer list into list.
   *
   * Denote list.size() by n and ilist.size() by m.
   * Time complexity O(n + m log m).
   */
  RandomAccessList& operator=(std::initializer_list<value_type> ilist) {
    assign(ilist);
    return *this;
  }

  /**
   * Assigns elements from range [begin, end) into list.
   *
   * Denote (end - begin) by n.
   * Time complexity O(n log n).
   */
  template< class InputIt >
  void assign(InputIt first, InputIt last) {
    clear();
    for (const auto& value: make_range(first, last))
      push_back(value);
  }

  /**
   * Assigns elements from initializer list into list.
   *
   * Denote list.size() by n and ilist.size() by m.
   * Time complexity O(n + m log m).
   */
  void assign(std::initializer_list<value_type> ilist) {
    assign(ilist.begin(), ilist.end());
  }

  /**
   * Returns reference to the first element of the list.
   *
   * List must not be empty.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  reference front() {
    assert(!empty());
    return avl::first(root_)->value();
  }

  /**
   * Returns const reference to the first element of the list.
   *
   * List must not be empty.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  const_reference front() const {
    assert(!empty());
    return avl::first(root_)->value();
  }

  /**
   * Returns reference to the last element of the list.
   *
   * List must not be empty.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  reference back() {
    assert(!empty());
    return avl::last(root_)->value();
  }

  /**
   * Returns const reference to the last element of the list.
   *
   * List must not be empty.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  const_reference back() const {
    assert(!empty());
    return avl::last(root_)->value();
  }

  /**
   * Returns reference to element at position pos.
   *
   * Denote list.size() by n.
   *
   * pos must satisfy 0 <= pos < n.
   * Time complexity O(log n).
   */
  reference operator[](size_type pos) {
    auto node = GoAtIndex(pos);
    return node->value();
  }

  /**
   * Returns const reference to element at position pos.
   *
   * Denote list.size() by n.
   *
   * pos must satisfy 0 <= pos < n.
   * Time complexity O(log n).
   */
  const_reference operator[](size_type pos) const {
    auto node = GoAtIndex(pos);
    return node->value();
  }

  /**
   * Returns iterator to the first element of the list.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  iterator begin() {
    if (empty())
      return end();
    else
      return iterator(avl::first(root_), this);
  }

  /**
   * Returns const iterator to the first element of the list.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  const_iterator begin() const {
    if (empty())
      return end();
    else
      return const_iterator(avl::first(root_), this);
  }

  /**
   * Returns iterator to the first position after last element of the list.
   *
   * Time complexity O(1).
   */
  iterator end() {
    return iterator(nullptr, this);
  }

  /**
   * Returns const iterator to the first position after last element of the list.
   *
   * Time complexity O(1).
   */
  const_iterator end() const {
    return const_iterator(nullptr, this);
  }

  /**
   * Checks if the list has no elements.
   *
   * Time complexity O(1).
   */
  bool empty() const {
    return (root_ == nullptr);
  }

  /**
   * Returns the number of elements stored in list.
   *
   * Time complexity O(1).
   */
  size_type size() const {
    return (root_ != nullptr)? ListNode::size(root_) : 0;
  }

  /**
   * Removes all elements stored in list.
   *
   * List becomes empty.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  void clear() {
    if (root_ != nullptr)
      avl::destroy_tree(root_);
  }

  /**
   * Inserts new node before node pointed by iterator pos.
   *
   * Returns iterator to newly created list node.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  iterator insert(iterator pos, value_type value) {
    auto where = pos.getHelper().node_;
    auto new_node = new ListNode(std::move(value));
    root_ = avl::insert(root_, where, new_node);
    return iterator(new_node, this);
  }

  /**
   * Inserts new node before node pointed by iterator pos.
   *
   * Returns iterator to newly created list node.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  template<class... Args>
  iterator emplace(iterator pos, Args&&... args) {
    return insert(pos, value_type(std::forward<Args>(args)...));
  }

  /**
   * Removes node pointed by iterator pos.
   *
   * Returns iterator to subsequent node.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  iterator erase(iterator pos) {
    assert(pos != end());
    auto next = std::next(pos);
    auto node = pos.getHelper().node_;
    root_ = avl::erase(node);
    return next;
  }

  /**
   * Inserts node at the end of the list.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  void push_back(value_type value) {
    insert(end(), value);
  }

  /**
   * Inserts node at the end of the list.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  template<class... Args>
  void emplace_back(Args&&... args) {
    push_back(value_type(std::forward<Args>(args)...));
  }

  /**
   * Removes the last node of the list.
   *
   * List must not be empty.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  void pop_back() {
    erase(std::prev(end()));
  }

  /**
   * Inserts node at the begin of the list.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  void push_front(value_type value) {
    insert(begin(), value);
  }

  /**
   * Inserts node at the begin of the list.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  template<class... Args>
  void emplace_front(Args&&... args) {
    push_front(value_type(std::forward<Args>(args)...));
  }

  /**
   * Removes the first node of the list.
   *
   * List must not be empty.
   *
   * Denote list.size() by n.
   * Time complexity O(log n).
   */
  void pop_front() {
    erase(begin());
  }

  /**
   * Swappes two lists.
   *
   * Time complexity O(1).
   */
  void swap(RandomAccessList& other) {
    std::swap(root_, other.root_);
  }

private:
  node_pointer GoAtIndex(size_type index) const {
    assert(index <= size());
    if (index == size())
      return nullptr;

    auto node = root_;
    while (index > 0) {
      auto size = ListNode::size(node->left());
      if (index < size) {
        node = node->left();
      }
      else if (index > size) {
        node = node->right();
        index -= size + 1;
      }
      else {
        return node;
      }
    }
    return avl::first(node);
  }

  size_type CalculateIndex(node_pointer node) const {
    if (node == nullptr)
      return size();

    size_type result = ListNode::size(node->left());
    while (node->side() != avl::side_type::root) {
      auto side = node->side();
      node = node->parent();
      if (side == avl::side_type::right)
        result += ListNode::size(node->left()) + 1;
    }
    return result;
  }

  node_pointer root_ = nullptr;
};

} // namespace lib
