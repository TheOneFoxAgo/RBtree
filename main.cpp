#include "BinarySearchTree.h"
#include <algorithm>
#include <iostream>

void test(bool isSuccess, const char *successMessage, const char *failMessage) {
  if (isSuccess) {
    std::cout << successMessage;
  } else {
    std::cout << failMessage;
  }
}
void print(const BinarySearchTree<int> &tree) {
  tree.output(std::cout);
  std::cout << '\n';
}

void testTree() {
  BinarySearchTree<int> tree;
  int nodes[]{10, 5, 13, 4, 7, 11, 14, 6, 8, 12};
  for (int i : nodes) {
    tree.insertNode(i);
  }
  print(tree);
  test(tree.insertNode(13), "inserted\n", "not inserted\n");
  test(tree.insertNode(4), "inserted\n", "not inserted\n");

  print(tree);

  std::cout << "count: " << tree.getNumberOfNodes() << '\n';
  std::cout << "height: " << tree.getHeight() << '\n';

  test(tree.searchKeyIterative(100), "found\n", "not found\n");
  test(tree.searchKeyIterative(4), "found\n", "not found\n");

  int nodesToDelete[]{5, 11, 14, 15};
  for (int i : nodesToDelete) {
    std::cout << "Trying to delete " << i << ": ";
    test(tree.deleteNode(i), "deleted\n", "not deleted\n");
    print(tree);
    std::cout << "count: " << tree.getNumberOfNodes() << '\n';
  }
  tree.insertNode(14);

  BinarySearchTree<int> moved(std::move(tree));
}

void testDestructorAfterDelete() {
  BinarySearchTree<int> tree;
  int nodes[]{
      22376, 19148, 9064,  4536,  26826, 7510,  16348, 18792, 23507, 6724,
      12960, 11559, 9879,  11023, 18260, 30890, 3200,  2071,  28381, 32676,
      17456, 448,   12726, 13838, 9644,  24207, 2139,  3207,  13422, 2533,
      7402,  10504, 2445,  1443,  2830,  20904, 20377, 2771,  9468,  8436,
      4157,  16521, 13710, 17472, 14788, 12956, 4630,  4888,  1934,  4186,
      26033, 25914, 28231, 27376, 20052, 4866,  28271, 22225, 1220,  32728,
      7044,  12356, 17215, 5488,  6378,  30255, 4676,  7822,  15889, 6895,
      21118, 24465, 20807, 2358,  4513,  31062, 25879, 8793,  13356, 30480,
      413,   14781, 9249,  8973,  29417, 28937, 17829, 10060, 8700,  2880,
      27160, 5242,  11925, 9964,  21235, 15457, 11831, 4317,  23535, 9510,
  };
  for (int i : nodes) {
    tree.insertNode(i);
  }
  std::cout << "Deleting nodes\n";
  int nodesToDelete[]{12726, 2830, 1220};
  for (int i : nodesToDelete) {
    tree.deleteNode(i);
  }
  std::cout << "Testing destructor. If not segfaulted, test is successful.\n";
}

void testWalk() {
  BinarySearchTree<int> tree;
  int nodes[]{
      23, 16, 12, 19, 17, 24, 5,  1, 4,  7, 11, 6,  22, 20, 9,
      27, 10, 2,  13, 15, 25, 18, 3, 14, 8, 29, 26, 30, 21, 28,
  };
  for (int i : nodes) {
    tree.insertNode(i);
  }
  std::cout << "Testing infix walking\n";
  tree.inorderWalk(std::cout);
  std::cout << '\n';
  tree.inorderWalkIterative(std::cout);
  std::cout << '\n';
  BinarySearchTree<int> secondTree;
  for (int i : nodes) {
    secondTree.insertNode(((i + 6) % 30) + 1); // shuffling nodes.
  }
  std::cout << "Are two equal trees equal??? "
            << ((tree.isSimilar(secondTree)) ? "yes" : "no") << '\n';
  secondTree.insertNode(0);
  std::cout << "Are two unequal trees equal??? "
            << ((tree.isSimilar(secondTree)) ? "yes" : "no") << '\n';

  BinarySearchTree<int> thirdTree;
  int otherNodes[]{35, 33, 39, 32, 38, 31, 37, 40, 34, 36, 0};
  for (int i : otherNodes) {
    thirdTree.insertNode(i);
  };
  std::cout << "Are there any similar keys, when there are none?? "
            << ((tree.isIdenticalKey(thirdTree)) ? "yes" : "no") << '\n';
  std::cout << "Are there any similar keys, when there are one?? "
            << ((secondTree.isIdenticalKey(thirdTree)) ? "yes" : "no") << '\n';
  BinarySearchTree<int> forthTree;
  BinarySearchTree<int> fifthTree;
  std::cout << "Are two empty trees equal??? "
            << ((forthTree.isSimilar(fifthTree)) ? "yes" : "no") << '\n';
  std::cout << "Are there any similar keys, when both trees are empty?? "
            << ((forthTree.isIdenticalKey(fifthTree)) ? "yes" : "no") << '\n';
}
int main() {
  testTree();
  testDestructorAfterDelete();
  testWalk();
  std::cout << "Success\n";
  return 0;
}
