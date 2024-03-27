#include "BinarySearchTree.h"
#include <iostream>

void testTree() {
  BinarySearchTree<int> tree;
  int nodes[] = {10, 5, 12, 4, 7, 11, 13, 6, 8};
  for (int i : nodes) {
    tree.insertNode(i);
  }
  if (tree.insertNode(13)) {
    std::cout << "inserted\n";
  } else {
    std::cout << "not inserted\n";
  }

  tree.output(std::cout);
  std::cout << '\n';
  std::cout << "count: " << tree.getNumberOfNodes() << '\n';
  std::cout << "height: " << tree.getHeight() << '\n';

  if (!tree.searchKeyIterative(100)) {
    std::cout << "not found\n";
  }

  // tree.deleteNode(5);
  // tree.output(std::cout);

  if (!tree.deleteNode(100)) {
    std::cout << "not found\n";
  }
}

int main() {
  testTree();

  return 0;
}
