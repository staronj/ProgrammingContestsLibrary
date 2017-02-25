#pragma once
// Jakub Staroń, 2016

#include "io.h"
#include "headers.h"

namespace lib {
namespace avl {

enum class side_type : uint8 {
  left = 0,
  root = 1,
  right = 2,
};

template<typename DerivedNode>
struct Node {
  using node_pointer = DerivedNode*;
  using height_type = uint8;

  Node() = default;
  ~Node() = default;

  node_pointer left() const {
    return left_;
  }

  node_pointer right() const {
    return right_;
  }

  node_pointer parent() const {
    return parent_;
  }

  height_type height() const {
    return height_;
  }

  /**
   * Returns side of node in the context of parent.
   */
  side_type side() const {
    return side_;
  }

  template<typename L>
  friend L* rotate_right(L* A);

  template<typename L>
  friend L* rotate_left(L* A);

  template<typename L>
  friend L* balance(L* node);

  template<typename L>
  friend L* cut(L* node);

  template<typename L>
  friend L* link(L* where, L* what, side_type side);

  template<typename L>
  friend void update_height(L* node);

private:
  node_pointer left_ = nullptr;
  node_pointer right_ = nullptr;
  node_pointer parent_ = nullptr;

  // We want node to be as small as possible
  // so one can try to use bitfields here.
  // It's ineffective because of aligning.
  height_type height_ = 1;
  side_type side_ = side_type::root;
};

/**
 * Cuts subtree and returns pointer to cutted subtree.
 *
 * Returns given node.
 */
template<typename T>
T* cut(T* node) {
  if (node == nullptr)
    return nullptr;

  auto side = node->side();
  if (side != side_type::root) {
    if (side == side_type::left)
      node->parent_->left_ = nullptr;
    else
      node->parent_->right_ = nullptr;

    node->parent_ = nullptr;
    node->side_ = side_type::root;
  }

  return node;
}

/**
 * Links given node to other given node on given site.
 *
 * Where could be null. In this case link just returns what.
 *
 * Returns linked subtree.
 */
template<typename T>
T* link(T* where, T* what, side_type side) {
  if (where == nullptr)
    return what;
  else if (what == nullptr)
    return where;

  assert(what->parent_ == nullptr);
  if (side == side_type::left) {
    assert(where->left() == nullptr);
    where->left_ = what;
    what->parent_ = where;
    what->side_ = side_type::left;
  }
  else if (side == side_type::right) {
    assert(where->right() == nullptr);
    where->right_ = what;
    what->parent_ = where;
    what->side_ = side_type::right;
  }
  else {
    assert(false);
  }
  return what;
}

/**
 * Returns height of subtree of given node
 * or 0 if node is null.
 */
template<typename T>
typename Node<T>::height_type height(T* node) {
  return (node != nullptr) ? node->height() : typename Node<T>::height_type(0);
}

/**
 * Returns balance factor of node.
 *
 * Node should not be null.
 */
template<typename T>
typename Node<T>::height_type balance_factor(T* node) {
  return height(node->right()) - height(node->left());
}

/**
 * Updates height of given node.
 *
 * Node should not be null.
 */
template<typename T>
void update_height(T* node) {
  using height_type = typename Node<T>::height_type;
  auto left = height(node->left());
  auto right = height(node->right());
  node->height_ = std::max(left, right) + height_type(1);
}

/**
 * Performs right rotate on node.
 *
 * Returns new subtree root.
 *
 *     A       B
 *    /       / \
 *   B   ->  C  A
 *  / \        /
 * C  D       D
 */
template<typename T>
T* rotate_right(T* A) {
  auto B = A->left_;
  auto D = B->right_;
  A->left_ = D;
  B->right_ = A;

  B->parent_ = A->parent_;
  B->side_ = A->side_;

  A->parent_ = B;
  A->side_ = side_type::right;

  if (D != nullptr) {
    D->parent_ = A;
    D->side_ = side_type::left;
  }

  update_height(A);
  A->update();
  update_height(B);
  B->update();

  return B;
}

/**
 * Performs left rotatr on node.
 *
 * Returns new subtree root.
 *
 *  A          B
 *   \        / \
 *   B   ->  A  D
 *  / \       \
 * C  D       C
 */
template<typename T>
T* rotate_left(T* A) {
  auto B = A->right_;
  auto C = B->left_;
  A->right_ = C;
  B->left_ = A;

  B->parent_ = A->parent_;
  B->side_ = A->side_;

  A->parent_ = B;
  A->side_ = side_type::left;

  if (C != nullptr) {
    C->parent_ = A;
    C->side_ = side_type::right;
  }


  update_height(A);
  A->update();
  update_height(B);
  B->update();
  return B;
}

/**
 * Returns first node in tree (left superson).
 *
 * Node should not be null.
 */
template<typename T>
T* first(T* node) {
  assert(node != nullptr);
  while (node->left() != nullptr)
    node = node->left();
  return node;
}

/**
 * Returns last node in tree (right superson).
 *
 * Node should not be null.
 */
template<typename T>
T* last(T* node) {
  assert(node != nullptr);
  while (node->right() != nullptr)
    node = node->right();
  return node;
}

/**
 * Performs binary search on tree.
 * Returns first element for which predicate is true
 * or nullptr if is always false.
 *
 * predicate must be monotonic,
 * i.e. must be constantly false on (possibly empty) prefix
 * and then constantly true on remaining suffix.
 *
 * predicate must take T* type and return boolean value
 */
template<typename T, typename Predicate>
T* find(T* root, Predicate predicate) {
  while (root != nullptr && !predicate(root))
    root = root->right();

  if (root == nullptr)
    return nullptr;

  auto left = find(root->left(), predicate);
  return (left != nullptr) ? left : root;
}

/**
 * Balances given node.
 *
 * Returns new subtree root.
 */
template<typename T>
T* balance(T* node) {
  auto side = node->side();

  if (balance_factor(node) == 2) {
    if (balance_factor(node->right_) < 0)
      node->right_ = rotate_right(node->right_);
    node = rotate_left(node);
  }
  else if (balance_factor(node) == -2) {
    if (balance_factor(node->left_) > 0)
      node->left_ = rotate_left(node->left_);
    node = rotate_right(node);
  }

  if (side == side_type::left) {
    node->parent()->left_ = node;
  }
  else if (side == side_type::right) {
    node->parent()->right_ = node;
  }

  return node;
}

/**
 * Balances tree starting in given node and traversing
 * up to the root. Returns new tree root.
 *
 * If given node is null returns nullptr.
 */
template<typename T>
T* balance_to_root(T* node) {
  T* prev = nullptr;
  while (node != nullptr) {
    update_height(node);
    node->update();
    prev = node = balance(node);
    node = node->parent();
  }
  return prev;
}

/**
 * Performs insert on tree.
 * Inserts new_node before the 'where' node.
 *
 * If 'where' node is null then inserts new_node as
 * last element of the tree.
 *
 * New node should be pointer to newly created node.
 */
template<typename T>
T* insert(T* root, T* where, T* new_node) {
  if (root == nullptr)
    return new_node;
  
  if (where == nullptr) {
    where = last(root);
    link(where, new_node, side_type::right);
  }
  else {
    if (where->left() == nullptr) {
      link(where, new_node, side_type::left);
    }
    else {
      where = last(where->left());
      link(where, new_node, side_type::right);
    }
  }
  return balance_to_root(new_node);
}

/**
 * Merges two trees, ie creates tree with all nodes from
 * left tree being before all nodes from right tree.
 *
 * Left and right could be nulls.
 */
template<typename T>
T* merge_trees(T* left, T* right) {
  if (right == nullptr)
    return left;
  else if (left == nullptr)
    return right;

  auto minimum = first(right);
  auto minimum_parent = minimum->parent();
  minimum = cut(minimum);

  if (minimum->right() != nullptr) {
    auto minimum_right = cut(minimum->right());
    minimum_parent = link(minimum_parent, minimum_right, side_type::left);
  }

  right = balance_to_root(minimum_parent);

  link(minimum, left, side_type::left);
  link(minimum, right, side_type::right);

  return minimum;
}

/**
 * Removes given node from tree.
 *
 * Returns new tree root.
 */
template<typename T>
T* erase(T* node) {
  assert(node != nullptr);

  auto side = node->side();
  auto parent = node->parent();
  auto left = node->left();
  auto right = node->right();

  node = cut(node);
  left = cut(left);
  right = cut(right);

  delete node;

  auto merged = merge_trees(left, right);
  parent = link(parent, merged, side);

  return balance_to_root(parent);
}

/**
 * Returns node following given node in pre order.
 */
template<typename T>
T* next_inorder(T* node) {
  if (node->right() != nullptr) {
    return first(node->right());
  }
  else {
    decltype(node) prev;
    do {
      prev = node;
      node = node->parent();
    } while (node != nullptr && prev->side() != side_type::left);
    return node;
  }
}

/**
 * Returns node preceding given node in pre order.
 */
template<typename T>
T* prev_inorder(T* node) {
  if (node->left() != nullptr) {
    return last(node->left());
  }
  else {
    decltype(node) prev;
    do {
      prev = node;
      node = node->parent();
    } while (node != nullptr && prev->side() != side_type::right);
    return node;
  }
}

/**
 * Destroys tree of root in given node.
 *
 * Node should not be null.
 */
template<typename T>
void destroy_tree(T*& root) {
  using node_pointer = T*;

  assert(root != nullptr);
  assert(root->side() == side_type::root);

  /**
   * For given subtree returns first node in
   * pre order in that subtree.
   */
  auto first_preorder = [](node_pointer n) {
    n = first(n);
    while (n->right() != nullptr) {
      n = n->right();
      n = first(n);
    }
    return n;
  };

  /**
   * Returns node following given node in post order.
   */
  auto next_postorder = [&first_preorder](node_pointer n) {
    auto side = n->side();
    if (side == side_type::left) {
      n = n->parent();
      if (n->right() != nullptr) {
        n = n->right();
        n = first_preorder(n);
      }

      return n;
    }
    else
      return n->parent();
  };

  root = first_preorder(root);
  while (root != nullptr) {
    auto tmp = root;
    root = next_postorder(root);

    delete tmp;
  }
  assert(root == nullptr);
}


struct DummyNode : Node<DummyNode> {
  using value_type = uint32;

  DummyNode(value_type value) :
      value_(value) {}

  void update();

  uint32 value() const {
    return value_;
  }

  friend uint32 size(DummyNode::node_pointer node);

private:
  uint32 size_ = 1;
  value_type value_;
};

/**
 * Returns size of subtree of given node
 * or 0 if node is null.
 */
uint32 size(DummyNode::node_pointer node) {
  return (node != nullptr) ? node->size_ : uint32(0);
}

void DummyNode::update() {
  size_ = size(left()) + size(right()) + 1;
}

/**
 * Returns node which is lower_bound for given value.
 */
DummyNode::node_pointer lower_bound(DummyNode::node_pointer root, DummyNode::value_type value) {
  return find(root, [value](DummyNode::node_pointer node) {
    return node->value() >= value;
  });
}

DummyNode::node_pointer insert(DummyNode::node_pointer root, DummyNode::value_type k) {
  auto found = find(root, [k](DummyNode::node_pointer n) {
    return n->value() > k;
  });
  return insert(root, found, new DummyNode(k));
}

} // namespace avl
} // namespace lib
