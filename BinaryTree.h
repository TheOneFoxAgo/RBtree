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
      candidate->left = new Node(key, candidate);
    }
    return true;
  }

  bool deleteNode(const T &key) {
    Node *candidate = searchNodeIterative(key);
    if (!candidate || key != candidate->key_) {
      return false;
    }
    int nBranches = candidate->left_ != nullptr + candidate->right_ != nullptr;
    switch (nBranches) {
    case 0:
      delete candidate;
      break;
    case 1: {
      Node *newCandidate = nullptr;
      if (candidate->left_) {
        newCandidate = candidate->left_;
      } else {
        newCandidate = candidate->right_;
      }
      if (candidate->p_) {
        if (candidate->p_->left_ == candidate) {
          candidate->p_->left_ = newCandidate;
        } else {
          candidate->p_->right_ = newCandidate;
        }
      } else {
        root_ = newCandidate;
      }
      delete candidate;
    }
    }
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
  Node *searchNodeIterative(const T &key) const {
    Node *current = root_;
    bool hasFound = false;
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

  void output(std::ostream &out, Node *root) const;

  int getNumberOfNodes(const Node *node) const;

  int getHeight(const Node *node) const;

  void inorderWalk(Node *node) const;
  Node *root_;
};

#endif
