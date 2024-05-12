#ifndef RBTREE_ITERATOR_HPP
#define RBTREE_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <type_traits>
#include "RBTreeNode.hpp"

namespace demidenko
{
  template < class K, class T, class Compare >
  class RBTree;

  template < class K, class T, class Compare, bool CONST >
  class RBTreeIterator
  {
    friend class RBTree< K, T, Compare >;

  public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::conditional_t< CONST, const std::pair< const K, T >, std::pair< const K, T > >;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::bidirectional_iterator_tag;

    RBTreeIterator(const RBTreeIterator&) = default;
    ~RBTreeIterator() = default;
    RBTreeIterator< K, T, Compare, CONST >& operator=(const RBTreeIterator< K, T, Compare, CONST >&) = default;
    RBTreeIterator< K, T, Compare, CONST >& operator++()
    {
      node_ = successor(node_);
      return *this;
    }
    RBTreeIterator< K, T, Compare, CONST > operator++(int)
    {
      RBTreeIterator< K, T, Compare, CONST > temp(*this);
      ++*this;
      return temp;
    }
    RBTreeIterator< K, T, Compare, CONST >& operator--()
    {
      node_ = predesessor(node_);
      return *this;
    }
    RBTreeIterator< K, T, Compare, CONST > operator--(int)
    {
      RBTreeIterator< K, T, Compare, CONST > temp(*this);
      --*this;
      return temp;
    }

    template < class = std::enable_if< !CONST > >
    value_type& operator*()
    {
      return node_->value;
    }
    const value_type& operator*() const
    {
      return node_->value;
    }
    template < class = std::enable_if< !CONST > >
    value_type* operator->()
    {
      return std::addressof(node_->value);
    }
    const value_type* operator->() const
    {
      return std::addressof(node_->value);
    }

    bool operator==(const RBTreeIterator< K, T, Compare, CONST >& other) const
    {
      return node_ == other.node_;
    }
    bool operator!=(const RBTreeIterator< K, T, Compare, CONST >& other) const
    {
      return node_ != other.node_;
    }

  private:
    using Node = detail::Node< K, T >;
    explicit RBTreeIterator(Node* node):
      node_(node){};
    Node* successor(Node* target) const
    {
      if (target->right)
      {
        target = target->right;
        while (target->left)
        {
          target = target->left;
        }
        return target;
      }
      else
      {
        Node* candidate = target->p;
        while (candidate && candidate->right == target)
        {
          target = candidate;
          candidate = candidate->p;
        }
        return candidate;
      }
    }
    Node* predesessor(Node* target) const
    {
      if (target->left)
      {
        target = target->left;
        while (target->right)
        {
          target = target->right;
        }
        return target;
      }
      else
      {
        Node* candidate = target->p;
        while (candidate && candidate->left == target)
        {
          target = candidate;
          candidate = candidate->p;
        }
        return candidate;
      }
    }
    Node* node_;
  };
}

#endif
