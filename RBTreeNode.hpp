#ifndef RBTREE_NODE_HPP
#define RBTREE_NODE_HPP

#include <utility>

namespace demidenko
{
  namespace detail
  {
    enum class Color
    {
      Black,
      Red
    };
    template < class K, class T >
    struct Node
    {
      std::pair< const K, T > value;
      Color color;
      Node* p = nullptr;
      Node* left = nullptr;
      Node* right = nullptr;
      Node* child(bool isLeft)
      {
        return isLeft ? left : right;
      }
    };

  }
}
#endif
