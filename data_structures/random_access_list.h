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

  RandomAccessList() = default;

  template<class InputIt>
  RandomAccessList(InputIt first, InputIt last) {
    assign(first, last);
  }

  RandomAccessList(const RandomAccessList& other) {
    operator=(other);
  }

  RandomAccessList(RandomAccessList&& other) {
    operator=(std::move(other));
  }

  RandomAccessList(std::initializer_list<value_type> init) {
    assign(init);
  }

  ~RandomAccessList() {
    clear();
  }

  RandomAccessList& operator=(RandomAccessList& other) {
    assign(other.begin(), other.end());
    return *this;
  }

  RandomAccessList& operator=(RandomAccessList&& other) {
    clear();
    root_ = other.root_;
    other.root_ = nullptr;
    return *this;
  }

  RandomAccessList& operator=(std::initializer_list<value_type> ilist) {
    assign(ilist);
    return *this;
  }

  template< class InputIt >
  void assign(InputIt first, InputIt last) {
    clear();
    for (const auto& value: make_range(first, last))
      push_back(value);
  }

  void assign(std::initializer_list<value_type> ilist) {
    assign(ilist.begin(), ilist.end());
  }

  reference front() {
    assert(!empty());
    return avl::first(root_)->value();
  }

  const_reference front() const {
    assert(!empty());
    return avl::first(root_)->value();
  }

  reference back() {
    assert(!empty());
    return avl::last(root_)->value();
  }

  const_reference back() const {
    assert(!empty());
    return avl::last(root_)->value();
  }

  reference operator[](size_type pos) {
    auto node = GoAtIndex(pos);
    return node->value();
  }

  const_reference operator[](size_type pos) const {
    auto node = GoAtIndex(pos);
    return node->value();
  }

  iterator begin() {
    if (empty())
      return end();
    else
      return iterator(avl::first(root_), this);
  }

  const_iterator begin() const {
    if (empty())
      return end();
    else
      return const_iterator(avl::first(root_), this);
  }

  iterator end() {
    return iterator(nullptr, this);
  }

  const_iterator end() const {
    return const_iterator(nullptr, this);
  }

  bool empty() const {
    return (root_ == nullptr);
  }

  size_type size() const {
    return (root_ != nullptr)? ListNode::size(root_) : 0;
  }

  void clear() {
    if (root_ != nullptr)
      avl::destroy_tree(root_);
  }

  iterator insert(iterator pos, value_type value) {
    auto where = pos.getHelper().node_;
    auto new_node = new ListNode(std::move(value));
    root_ = avl::insert(root_, where, new_node);
    return iterator(new_node, this);
  }

  template<class... Args>
  iterator emplace(iterator pos, Args&&... args) {
    return insert(pos, value_type(std::forward<Args>(args)...));
  }

  iterator erase(iterator pos) {
    assert(pos != end());
    auto next = std::next(pos);
    auto node = pos.getHelper().node_;
    root_ = avl::erase(node);
    return next;
  }

  void push_back(value_type value) {
    insert(end(), value);
  }

  template<class... Args>
  void emplace_back(Args&&... args) {
    push_back(value_type(std::forward<Args>(args)...));
  }

  void pop_back() {
    erase(std::prev(end()));
  }

  void push_front(value_type value) {
    insert(begin(), value);
  }

  template<class... Args>
  void emplace_front(Args&&... args) {
    push_front(value_type(std::forward<Args>(args)...));
  }

  void pop_front() {
    erase(begin());
  }

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
