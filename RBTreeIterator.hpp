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
    using value_type = std::conditional_t< CONST, const std::pair< K, T >, std::pair< K, T > >;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::bidirectional_iterator_tag;

    RBTreeIterator(const RBTreeIterator&) = default;
    ~RBTreeIterator() = default;
    RBTreeIterator< K, T, Compare, CONST >& operator=(const RBTreeIterator< K, T, Compare, CONST >&) = default;
    RBTreeIterator< K, T, Compare, CONST >& operator++()
    {
      node_ = successor(node_);
      updateOut();
      return *this;
    }
    RBTreeIterator< K, T, Compare, CONST > operator++(int)
    {
      RBTreeIterator< K, T, Compare, CONST > temp(*this);
      ++*this;
      updateOut();
      return temp;
    }
    RBTreeIterator< K, T, Compare, CONST >& operator--()
    {
      node_ = predesessor(node_);
      updateOut();
      return *this;
    }
    RBTreeIterator< K, T, Compare, CONST > operator--(int)
    {
      RBTreeIterator< K, T, Compare, CONST > temp(*this);
      --*this;
      updateOut();
      return temp;
    }

    std::enable_if_t< !CONST, value_type >& operator*()
    {
      return out_;
    }
    const value_type& operator*() const
    {
      return out_;
    }
    std::enable_if_t< !CONST, value_type >* operator->()
    {
      return std::addressof(out_);
    }
    const value_type* operator->() const
    {
      return std::addressof(out_);
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
      node_(node)
    {
      updateOut();
    };
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
    void updateOut()
    {
      if (node_)
      {
        out_.first = node_->key;
        out_.second = node_->value;
      }
    }
    Node* node_;
    value_type out_;
  };
}

#endif
