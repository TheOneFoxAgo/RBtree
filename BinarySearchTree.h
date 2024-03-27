#ifndef _BINARY_SEARCH_TREE_H
#define _BINARY_SEARCH_TREE_H

#include <iostream>

template <class T> class BinarySearchTree {
public:
  BinarySearchTree() : root_(nullptr) {}
  BinarySearchTree(const BinarySearchTree<T> &scr) = delete;
  BinarySearchTree(BinarySearchTree<T> &&src) noexcept : root_(src.root_) {
    src.root_ = nullptr;
  }
  BinarySearchTree<T> &operator=(const BinarySearchTree<T> &src) = delete;
  BinarySearchTree<T> &operator=(BinarySearchTree<T> &&src) noexcept {
    std::swap(root_, src.root_);
  }
  virtual ~BinarySearchTree() {
    Node *current = root_;
    while (true) {
      if (current->left_) {
        current = current->left_;
      } else if (current->right_) {
        current = current->right_;
      } else {
        Node *next = current->p_;
        if (!next) {
          delete current;
          break;
        }
        if (next->key_ > current->key_) {
          next->left_ = nullptr;
        } else {
          next->right_ = nullptr;
        }
        delete current;
        current = next;
      }
    }
  }

  bool searchKeyIterative(const T &key) const {
    Node *candidate = searchNodeIterative(key);
    return candidate && key == candidate->key_;
  }
  bool insertNode(const T &key) {
    Node *candidate = searchNodeIterative(key);
    if (!candidate) {
      root_ = new Node(key);
    } else if (key == candidate->key_) {
      return false;
    } else if (key > candidate->key_) {
      candidate->right_ = new Node(key, candidate);
    } else {
      candidate->left_ = new Node(key, candidate);
    }
    return true;
  }
  bool deleteNode(const T &key) {
    Node *target = searchNodeIterative(key);
    if (!target || key != target->key_) {
      return false;
    }
    int nBranches = (target->left_ != nullptr) + (target->right_ != nullptr);
    switch (nBranches) {
    case 0:
      updateNodeParent(target, nullptr);
      break;
    case 1: {
      Node *candidate = target->left_ ? target->left_ : target->right_;
      candidate->p_ = target->p_;
      updateNodeParent(target, candidate);
      break;
    }
    case 2: {
      Node *candidate = target->right_;
      while (candidate->left_) {
        candidate = candidate->left_;
      }
      if (candidate == target->right_) {
        candidate->left_ = target->left_;
        candidate->p_ = target->p_;
      } else {
        candidate->p_->left_ = candidate->right_;
        if (candidate->right_) {
          candidate->right_->p_ = candidate->p_;
        }

        target->right_->p_ = candidate;
        target->left_->p_ = candidate;

        candidate->right_ = target->right_;
        candidate->left_ = target->left_;
        candidate->p_ = target->p_;
      }
      updateNodeParent(target, candidate);
      break;
    }
    }
    delete target;
    return true;
  }
  void output(std::ostream &out) const { output(out, root_); }
  int getNumberOfNodes() const { return getNumberOfNodes(root_); }
  int getHeight() const { return getHeight(root_); }
  void inorderWalkIterative() const;
  void inorderWalk() const;
  void walkByLevels() const;
  bool isSimilar(const BinarySearchTree<T> &other) const;
  bool isIdenticalKey(const BinarySearchTree<T> &other) const;

private:
  struct Node {
    T key_;
    Node *left_;
    Node *right_;
    Node *p_;
    Node(T key, Node *p = nullptr, Node *left = nullptr, Node *right = nullptr)
        : key_(key), left_(left), right_(right), p_(p) {}
  };
  Node *searchNodeIterative(const T &key) const {
    Node *current = root_;
    bool hasFound = root_ == nullptr;
    while (!hasFound) {
      if (key == current->key_) {
        hasFound = true;
      } else if (key < current->key_) {
        if (current->left_) {
          current = current->left_;
        } else {
          hasFound = true;
        }
      } else {
        if (current->right_) {
          current = current->right_;
        } else {
          hasFound = true;
        }
      }
    }
    return current;
  }
  void output(std::ostream &out, Node *root) const {
    out << '(';
    out << root->key_;
    if (root->left_) {
      output(out, root->left_);
    }
    if (root->right_) {
      output(out, root->right_);
    }
    out << ')';
  }
  int getNumberOfNodes(const Node *node) const {
    if (node) {
      return 1 + getNumberOfNodes(node->left_) + getNumberOfNodes(node->right_);
    } else {
      return 0;
    }
  }
  int getHeight(const Node *node) const {
    if (!node) {
      return 0;
    } else {
      int leftHeight = node->left_ ? getHeight(node->left_) : 0;
      int rightHeight = node->right_ ? getHeight(node->right_) : 0;
      return std::max(leftHeight, rightHeight) + 1;
    }
  }
  void inorderWalk(Node *node) const;

  void debug(const Node *node) const {
    std::cout << node << ": " << node->key_ << ", left: " << node->left_
              << ", right: " << node->right_ << '\n';
  }
  void updateNodeParent(Node *target, Node *candidate) {
    if (target->p_) {
      if (target->p_->key_ > target->key_) {
        target->p_->left_ = candidate;
      } else {
        target->p_->right_ = candidate;
      }
    } else {
      root_ = candidate;
    }
  }

  Node *root_;
};
#endif
