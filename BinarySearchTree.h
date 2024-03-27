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
    bool isLeft = true;
    while (true) {
      if (current->left_) {
        current = current->left_;
        isLeft = true;
      } else if (current->right_) {
        current = current->right_;
        isLeft = false;
      } else {
        Node *next = current->p_;
        delete current;
        current = next;
        if (!current) {
          break;
        }
        if (isLeft) {
          next->left_ = nullptr;
        } else {
          next->right_ = nullptr;
        }
      }
    }
  }

  bool searchKeyIterative(const T &key) const {
    Node *candidate = *searchNodeIterative(key);
    return candidate && key == candidate->key_;
  }
  bool insertNode(const T &key) {
    Node *candidate = *searchNodeIterative(key);
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
    Node **pTarget = const_cast<Node **>(searchNodeIterative(key));
    Node *target = *pTarget;
    if (!target || key != target->key_) {
      return false;
    }
    Node *targetLeft = target->left_;
    Node *targetRight = target->right_;
    Node *targetParent = target->p_;
    delete target;
    bool isDone = false;
    while (!isDone) {
      int nBranches = (targetLeft != nullptr) + (targetRight != nullptr);
      switch (nBranches) {
      case 0:
        isDone = true;
        break;
      case 1: {
        Node *candidate = targetLeft ? targetLeft : targetRight;
        candidate->p_ = targetParent;
        *pTarget = candidate;
        isDone = true;
        break;
      }
      case 2: {
        Node **pNewTarget = nullptr;
        Node *newTarget = *pTarget;
        while (newTarget->left_) {
          pNewTarget = &newTarget->left_;
          newTarget = *pTarget;
        }
        *pTarget = newTarget;
        std::swap(newTarget->left_, targetLeft);
        std::swap(newTarget->p_, targetParent);
        if (pNewTarget) {
          std::swap(newTarget->right_, targetRight);
          pTarget = pNewTarget;
        } else {
          isDone = true;
        }
        break;
      }
      }
    }
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
  Node *const *searchNodeIterative(const T &key) const {
    Node *const *pCurrent = &root_;
    bool hasFound = root_ == nullptr;
    while (!hasFound) {
      Node *current = *pCurrent;
      if (key == current->key_) {
        hasFound = true;
      } else if (key < current->key_) {
        if (current->left_) {
          pCurrent = &current->left_;
        } else {
          hasFound = true;
        }
      } else {
        if (current->right_) {
          pCurrent = &current->right_;
        } else {
          hasFound = true;
        }
      }
    }
    return pCurrent;
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

  Node *root_;
};
#endif
