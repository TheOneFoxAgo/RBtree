#ifndef _BINARY_SEARCH_TREE_H
#define _BINARY_SEARCH_TREE_H

#include <iostream>

template <class T> class BinarySearchTree {
public:
  BinarySearchTree();
  BinarySearchTree(const BinarySearchTree<T> &scr) = delete;
  BinarySearchTree(BinarySearchTree<T> &&scr) noexcept;
  BinarySearchTree<T> &operator=(const BinarySearchTree<T> &src) = delete;
  BinarySearchTree<T> &operator=(BinarySearchTree<T> &&src) noexcept;
  virtual ~BinarySearchTree();

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
      candidate->left = new Node(key, candidate);
    }
    return true;
  }

  bool deleteNode(const T &key) {
    Node **pTarget = searchNodeIterative(key);
    Node *target = *pTarget;
    if (!*target || key != target->key_) {
      return false;
    }
    Node *targetLeft = target->left_;
    Node *targetRight = target->right_;
    Node *targetParent = target->p_;
    delete target;
    bool isDone = false;
    while (!isDone) {
      int nBranches = targetLeft != nullptr + targetRight != nullptr;
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

  void output(std::ostream &out) const;

  int getNumberOfNodes() const;

  int getHeight() const;

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
  Node **searchNodeIterative(const T &key) const {
    Node **pCurrent = &root_;
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

  void output(std::ostream &out, Node *root) const;

  int getNumberOfNodes(const Node *node) const;

  int getHeight(const Node *node) const;

  void inorderWalk(Node *node) const;
  Node *root_;
};

#endif
