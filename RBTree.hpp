#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <cassert>
#include <functional>
#include <ostream>
#include <queue>
#include <utility>
#include "RBTreeIterator.hpp"
#include "RBTreeNode.hpp"

namespace demidenko
{
  template < class K, class T, class Compare = std::less< K > >
  class RBTree
  {
  public:
    using Iterator = RBTreeIterator< K, T, Compare, false >;
    using ConstIterator = RBTreeIterator< K, T, Compare, true >;
    RBTree():
      root_(nullptr),
      compare_({})
    {}
    RBTree(const RBTree< K, T, Compare >& scr) = delete;
    RBTree(RBTree< K, T, Compare >&& src) noexcept:
      root_(src.root_)
    {
      src.root_ = nullptr;
    }
    RBTree< K, T, Compare >& operator=(const RBTree< K, T, Compare >& src) = delete;
    RBTree< K, T, Compare >& operator=(RBTree< K, T, Compare >&& src) noexcept
    {
      std::swap(root_, src.root_);
      std::swap(compare_, src.compare_);
      return *this;
    }
    virtual ~RBTree()
    {
      Node* current = root_;
      if (current)
      {
        while (true)
        {
          if (current->left)
          {
            current = current->left;
          }
          else if (current->right)
          {
            current = current->right;
          }
          else
          {
            Node* next = current->p;
            if (!next)
            {
              delete current;
              break;
            }
            if (compare_(current->value.first, next->value.first))
            {
              next->left = nullptr;
            }
            else
            {
              next->right = nullptr;
            }
            delete current;
            current = next;
          }
        }
      }
    }

    bool find(const K& key) const
    {
      Node* candidate = findNode(key);
      return candidate && key == candidate->value.first;
    }
    bool insert(const K& key, const T& value)
    {
      return insertNode(findNode(key), key, value);
    }
    bool erase(const K& key)
    {
      Node* target = findNode(key);
      if (!target || key != target->value.first)
      {
        return false;
      }
      eraseNode(target);
      return true;
    }
    bool isRBTree() const
    {
      if (root_->color != Color::Black)
      {
        return false;
      }
      return isWeakRBTree(root_);
    }
    void walkByLevels(std::ostream& out) const
    {
      if (!root_)
      {
        return;
      }
      std::queue< Node* > queue;
      queue.push(root_);
      while (!queue.empty())
      {
        Node* current = queue.front();
        queue.pop();
        out << current->value.first << ' ';
        if (current->left)
        {
          queue.push(current->left);
        }
        if (current->right)
        {
          queue.push(current->right);
        }
      }
    }
    Iterator begin()
    {
      return Iterator(minNode(root_));
    }
    ConstIterator begin() const
    {
      return ConstIterator(minNode(root_));
    }
    ConstIterator cbegin() const
    {
      return ConstIterator(minNode(root_));
    }
    Iterator end()
    {
      return Iterator(nullptr);
    }
    ConstIterator end() const
    {
      return ConstIterator(nullptr);
    }
    ConstIterator cend() const
    {
      return ConstIterator(nullptr);
    }

  private:
    using Color = detail::Color;
    using Node = detail::Node< K, T >;
    Node* insertNode(Node* candidate, const K& key, const T& value)
    {
      if (!candidate)
      {
        root_ = new Node{ std::make_pair(key, value), Color::Black };
        return root_;
      }
      else if (key == candidate->value.first)
      {
        return nullptr;
      }
      Node* newNode = new Node{ std::make_pair(key, value), Color::Red, candidate };
      if (compare_(candidate->value.first, key))
      {
        candidate->right = newNode;
      }
      else
      {
        candidate->left = newNode;
      }
      insertFixup(newNode);
      return newNode;
    }
    void insertFixup(Node* target)
    {
      while (colorOf(target->p) == Color::Red)
      {
        target = partialInsertFixup(target, target->p == target->p->p->left);
      }
      root_->color = Color::Black;
    }
    Node* partialInsertFixup(Node* target, bool isParentLeft)
    {
      Node* grandpa = target->p->p;
      Node* uncle = grandpa->child(!isParentLeft);
      if (colorOf(uncle) == Color::Red)
      {
        target->p->color = Color::Black;
        uncle->color = Color::Black;
        grandpa->color = Color::Red;
        target = grandpa;
      }
      else
      {
        if ((target == target->p->right) == isParentLeft)
        {
          target = target->p;
          rotate(target, isParentLeft);
        }
        target->p->color = Color::Black;
        target->p->p->color = Color::Red;
        rotate(target->p->p, !isParentLeft);
      }
      return target;
    }
    void eraseNode(Node* target)
    {
      assert(target);
      Color erasedColor = target->color;
      Node* brokenNode = target->p;
      bool isLeftBroken = target == root_ || target == target->p->left;
      int nBranches = (target->left != nullptr) + (target->right != nullptr);
      switch (nBranches)
      {
      case 0:
        updateParentNode(target, nullptr);
        break;
      case 1:
      {
        Node* candidate = target->child(target->left);
        candidate->p = target->p;
        updateParentNode(target, candidate);
        break;
      }
      case 2:
      {
        Node* candidate = minNode(target->right);
        erasedColor = candidate->color;
        brokenNode = candidate;
        isLeftBroken = false;
        if (candidate != target->right)
        {
          isLeftBroken = true;
          candidate->p->left = candidate->right;
          if (candidate->right)
          {
            candidate->right->p = candidate->p;
          }
          target->right->p = candidate;
          candidate->right = target->right;
        }
        target->left->p = candidate;
        candidate->left = target->left;
        candidate->p = target->p;
        candidate->color = target->color;
        updateParentNode(target, candidate);
        break;
      }
      }
      delete target;
      if (erasedColor == Color::Black)
      {
        eraseFixup(brokenNode, isLeftBroken);
      }
    }
    void eraseFixup(Node* target, bool isLeftBroken)
    {
      assert(target);
      Node* child = target->child(isLeftBroken);
      Node* brother = target->child(!isLeftBroken);
      while (target && colorOf(child) == Color::Black)
      {
        if (colorOf(brother) == Color::Red)
        {
          brother->color = Color::Black;
          target->color = Color::Red;
          rotate(target, isLeftBroken);
          brother = target->child(!isLeftBroken);
        }
        assert(brother);
        if (colorOf(brother->left) == Color::Black && colorOf(brother->right) == Color::Black)
        {
          brother->color = Color::Red;
          isLeftBroken = target == root_ || target == target->p->left;
          target = target->p;
          child = target->child(isLeftBroken);
          brother = target->child(!isLeftBroken);
        }
        else
        {
          if (colorOf(brother->right) == Color::Black)
          {
            brother->left->color = Color::Black;
            brother->color = Color::Red;
            rotate(brother, !isLeftBroken);
            brother = target->child(!isLeftBroken);
          }
          brother->color = target->color;
          target->color = Color::Black;
          assert(brother->right);
          brother->right->color = Color::Black;
          rotate(target, isLeftBroken);
          target = nullptr;
        }
      }
      if (target)
      {
        target->child(isLeftBroken)->color = Color::Black;
      }
      else if (root_)
      {
        root_->color = Color::Black;
      }
    }
    void updateParentNode(Node* oldNode, Node* newNode)
    {
      if (oldNode->p)
      {
        if (compare_(oldNode->value.first, oldNode->p->value.first))
        {
          oldNode->p->left = newNode;
        }
        else
        {
          oldNode->p->right = newNode;
        }
      }
      else
      {
        root_ = newNode;
      }
    }
    Node* findNode(const K& key) const
    {
      Node* current = root_;
      bool hasFound = root_ == nullptr;
      while (!hasFound)
      {
        if (key == current->value.first)
        {
          hasFound = true;
        }
        else if (compare_(key, current->value.first))
        {
          if (current->left)
          {
            current = current->left;
          }
          else
          {
            hasFound = true;
          }
        }
        else
        {
          if (current->right)
          {
            current = current->right;
          }
          else
          {
            hasFound = true;
          }
        }
      }
      return current;
    }
    Node* minNode(Node* target) const
    {
      if (!target)
      {
        return nullptr;
      }
      while (target->left)
      {
        target = target->left;
      }
      return target;
    }
    void rotate(Node* target, bool isLeft)
    {
      if (isLeft)
      {
        rotateLeft(target);
      }
      else
      {
        rotateRight(target);
      }
    }
    void rotateRight(Node* target)
    {
      assert(target->left);
      updateParentNode(target, target->left);
      Node* middle = target->left->right;

      target->left->p = target->p;
      target->left->right = target;

      target->p = target->left;
      target->left = middle;

      if (middle)
      {
        middle->p = target;
      }
    }
    void rotateLeft(Node* target)
    {
      assert(target->right);
      updateParentNode(target, target->right);
      Node* middle = target->right->left;

      target->right->p = target->p;
      target->right->left = target;

      target->p = target->right;
      target->right = middle;

      if (middle)
      {
        middle->p = target;
      }
    }
    // returns black height. 0 means error.
    int isWeakRBTree(const Node* target) const
    {
      if (!target)
      {
        return 1;
      }
      bool isRed = target->color == Color::Red;
      if (isRed && (colorOf(target->left) == Color::Red || colorOf(target->right) == Color::Red))
      {
        return 0;
      }
      int result = isWeakRBTree(target->right);
      if (!result || result != isWeakRBTree(target->left))
      {
        return 0;
      }
      return result + !isRed;
    }
    Color colorOf(const Node* target) const
    {
      if (target)
      {
        return target->color;
      }
      else
      {
        return Color::Black;
      }
    }

    Node* root_;
    Compare compare_;
  };
}
#endif
