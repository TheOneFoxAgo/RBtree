#ifndef RBTREE_NODE_HPP
#define RBTREE_NODE_HPP
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
      K key;
      T value;
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
