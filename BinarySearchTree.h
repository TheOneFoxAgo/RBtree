#ifndef _BINARY_SEARCH_TREE_H
#define _BINARY_SEARCH_TREE_H

#include "QueueArray.h"
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
    return *this;
  }
  virtual ~BinarySearchTree() {
    Node *current = root_;
    if (current) {
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
      Node *candidate = min(target->right_);
      if (candidate == target->right_) {
        target->left_->p_ = candidate;
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
  int getHeight() const { return getHeight(root_) - 1; }
  void inorderWalkIterative(std::ostream &out) const {
    Node *current = min(root_);
    while (current) {
      out << current->key_ << ' ';
      current = successor(current);
    }
  }
  void inorderWalk(std::ostream &out) const { inorderWalk(root_, out); }
  void walkByLevels(std::ostream &out) const {
    if (!root_) {
      return;
    }
    QueueArray<Node *> queue(1 << getHeight());
    queue.enQueue(root_);
    while (!queue.isEmpty()) {
      Node *current = queue.deQueue();
      out << current->key_ << ' ';
      if (current->left_) {
        queue.enQueue(current->left_);
      }
      if (current->right_) {
        queue.enQueue(current->right_);
      }
    }
  }
  bool isSimilar(const BinarySearchTree<T> &other) const {
    Node *current = min(root_);
    Node *otherCurrent = min(other.root_);
    while (current && otherCurrent) {
      if (current->key_ == otherCurrent->key_) {
        current = successor(current);
        otherCurrent = successor(otherCurrent);
      } else {
        return false;
      }
    }
    return !(current || otherCurrent);
  }
  bool isIdenticalKey(const BinarySearchTree<T> &other) const {
    Node *current = min(root_);
    Node *otherCurrent = min(other.root_);
    while (current && otherCurrent) {
      if (current->key_ == otherCurrent->key_) {
        return true;
      } else if (current->key_ > otherCurrent->key_) {
        otherCurrent = successor(otherCurrent);
      } else {
        current = successor(current);
      }
    }
    return false;
  }

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
  void inorderWalk(Node *node, std::ostream &out) const {
    if (!node) {
      return;
    }
    inorderWalk(node->left_, out);
    out << node->key_ << ' ';
    inorderWalk(node->right_, out);
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
  Node *successor(Node *node) const {
    if (node->right_) {
      return min(node->right_);
    } else {
      Node *candidate = node->p_;
      while (candidate && candidate->right_ == node) {
        node = candidate;
        candidate = candidate->p_;
      }
      return candidate;
    }
  }
  Node *min(Node *node) const {
    if (!node) {
      return nullptr;
    }
    while (node->left_) {
      node = node->left_;
    }
    return node;
  }

  Node *root_;
};
#endif
