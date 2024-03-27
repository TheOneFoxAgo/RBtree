#include "BinarySearchTree.h"
#include <iostream>

void testTree() {
  BinarySearchTree<int> tree;

  tree.insertNode(15);
  tree.insertNode(23);
  if (tree.insertNode(13)) {
    std::cout << "inserted\n";
  }

  tree.output(std::cout); // (15(13)(23))
  std::cout << '\n';
  std::cout << "count: " << tree.getNumberOfNodes() << '\n';
  std::cout << "height: " << tree.getHeight() << '\n';

  if (!tree.searchKeyIterative(100)) {
    std::cout << "not found\n";
  }

  if (!tree.deleteNode(100)) {
    std::cout << "not found\n";
  }
}

int main() {
  testTree();

  return 0;
}
