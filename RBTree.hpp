#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <functional>
#include <utility>

template < class K, class T, class Compare = std::less< K > >
class RBTree
{
public:
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

  bool find(const K& key) const
  {
    Node* candidate = findNode(key);
    return candidate && key == candidate->key;
  }
  bool insert(const K& key, const T& value)
  {
    return insertNode(findNode(key), key, value);
  }
  bool erase(const K& key)
  {
    Node* target = findNode(key);
    if (!target || key != target->key)
    {
      return false;
    }
    eraseNode(target);
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
  Node* insertNode(Node* candidate, const K& key, const T& value)
  {
    if (!candidate)
    {
      root_ = new Node{ key, value, Color::Black };
      return root_;
    }
    else if (key == candidate->key)
    {
      return nullptr;
    }
    Node* newNode = new Node{ key, value, Color::Black, candidate };
    if (compare_(candidate->key, key))
    {
      candidate->right = newNode;
    }
    else
    {
      candidate->left = newNode;
    }
    return newNode;
  }
  void eraseNode(Node* target)
  {
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
      if (candidate != target->right)
      {
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
      updateParentNode(target, candidate);
      break;
    }
    }
    delete target;
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
  Node* findNode(const K& key) const
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
  void rotateRight(Node* node)
  {
    assert(node->left);
    updateParentNode(node, node->left);
    Node* middle = node->left->right;

    node->left->p = node->p;
    node->left->right = node;

    node->p = node->left;
    node->left = middle;

    if (middle)
    {
      middle->p = node;
    }
  }
  void rotateLeft(Node* node)
  {
    assert(node->right);
    updateParentNode(node, node->right);
    Node* middle = node->right->left;

    node->right->p = node->p;
    node->right->left = node;

    node->p = node->right;
    node->right = middle;

    if (middle)
    {
      middle->p = node;
    }
  }

  // returns black height. 0 means error.
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
