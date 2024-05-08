#ifndef _BINARY_SEARCH_TREE_H
#define _BINARY_SEARCH_TREE_H

#include <functional>
#include <utility>

template < class K, class T, class Compare = std::less< K > >
class BinarySearchTree
{
public:
  BinarySearchTree():
    root_(nullptr),
    compare_({})
  {}
  BinarySearchTree(const BinarySearchTree< K, T, Compare >& scr) = delete;
  BinarySearchTree(BinarySearchTree< K, T, Compare >&& src) noexcept:
    root_(src.root_)
  {
    src.root_ = nullptr;
  }
  BinarySearchTree< K, T, Compare >& operator=(const BinarySearchTree< K, T, Compare >& src) = delete;
  BinarySearchTree< K, T, Compare >& operator=(BinarySearchTree< K, T, Compare >&& src) noexcept
  {
    std::swap(root_, src.root_);
    std::swap(compare_, src.compare_);
    return *this;
  }
  virtual ~BinarySearchTree()
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
          if (compare_(current->key, next->key))
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

  bool searchKeyIterative(const K& key) const
  {
    Node* candidate = searchNodeIterative(key);
    return candidate && key == candidate->key;
  }
  bool insertNode(const K& key, const T& value)
  {
    Node* candidate = searchNodeIterative(key);
    if (!candidate)
    {
      root_ = new Node{ key, value, Color::Black };
    }
    else if (key == candidate->key)
    {
      return false;
    }
    else if (compare_(candidate->key, key))
    {
      candidate->right = new Node{ key, value, Color::Black, candidate };
    }
    else
    {
      candidate->left = new Node{ key, value, Color::Black, candidate };
    }
    return true;
  }
  bool deleteNode(const K& key)
  {
    Node* target = searchNodeIterative(key);
    if (!target || key != target->key)
    {
      return false;
    }
    int nBranches = (target->left != nullptr) + (target->right != nullptr);
    switch (nBranches)
    {
    case 0:
      updateParentNode(target, nullptr);
      break;
    case 1:
    {
      Node* candidate = target->left ? target->left : target->right;
      candidate->p = target->p;
      updateParentNode(target, candidate);
      break;
    }
    case 2:
    {
      Node* candidate = minNode(target->right);
      if (candidate == target->right)
      {
        target->left->p = candidate;
        candidate->left = target->left;
        candidate->p = target->p;
      }
      else
      {
        candidate->p->left = candidate->right;
        if (candidate->right)
        {
          candidate->right->p = candidate->p;
        }

        target->right->p = candidate;
        target->left->p = candidate;

        candidate->right = target->right;
        candidate->left = target->left;
        candidate->p = target->p;
      }
      updateParentNode(target, candidate);
      break;
    }
    }
    delete target;
    return true;
  }
  // void inorderWalkIterative(std::ostream &out) const {
  //   Node *current = min(root_);
  //   while (current) {
  //     out << current->key_ << ' ';
  //     current = successor(current);
  //   }
  // }
  bool isRBTree() const
  {
    if (root_->color != Color::Black)
    {
      return false;
    }
    return isWeakRBTree(root_);
  }

private:
  enum class Color
  {
    Black,
    Red
  };
  struct Node
  {
    K key;
    T value;
    Color color;
    Node* p = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
  };
  Node* searchNodeIterative(const K& key) const
  {
    Node* current = root_;
    bool hasFound = root_ == nullptr;
    while (!hasFound)
    {
      if (key == current->key)
      {
        hasFound = true;
      }
      else if (compare_(key, current->key))
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
  void updateParentNode(Node* oldNode, Node* newNode)
  {
    if (oldNode->p)
    {
      if (compare_(oldNode->key, oldNode->p->key))
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
  Node* successor(Node* node) const
  {
    if (node->right)
    {
      return minNode(node->right);
    }
    else
    {
      Node* candidate = node->p;
      while (candidate && candidate->right == node)
      {
        node = candidate;
        candidate = candidate->p;
      }
      return candidate;
    }
  }
  Node* minNode(Node* node) const
  {
    if (!node)
    {
      return nullptr;
    }
    while (node->left)
    {
      node = node->left;
    }
    return node;
  }
  // returns black height
  int isWeakRBTree(const Node* node) const
  {
    if (!node)
    {
      return 1;
    }
    bool isRed = node->color == Color::Red;
    if (isRed && (colorOf(node->left) == Color::Red || colorOf(node->right) == Color::Red))
    {
      return 0;
    }
    int result = isWeakRBTree(node->right);
    if (!result || result != isWeakRBTree(node->left))
    {
      return 0;
    }
    return result + !isRed;
  }
  Color colorOf(const Node* node) const
  {
    if (node)
    {
      return node->color;
    }
    else
    {
      return Color::Black;
    }
  }

  Node* root_;
  Compare compare_;
};
#endif
