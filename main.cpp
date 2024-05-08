#include <iostream>
#include "BinarySearchTree.h"

void test(bool isSuccess, const char* successMessage, const char* failMessage)
{
  if (isSuccess)
  {
    std::cout << successMessage;
  }
  else
  {
    std::cout << failMessage;
  }
}
template < class T >
void testRB(const T& tree)
{
  test(tree.isRBTree(), "Tree is still RBTree\n", "Tree is not RBTree\n");
}
void testTree()
{
  BinarySearchTree< int, int > tree;
  int nodes[]{ 10, 5, 13, 4, 7, 11, 14 };
  for (int i : nodes)
  {
    tree.insertNode(i, 0);
  }
  testRB(tree);
  test(tree.insertNode(13, 0), "inserted\n", "not inserted\n");
  test(tree.insertNode(4, 0), "inserted\n", "not inserted\n");
  testRB(tree);
  test(tree.searchKeyIterative(100), "found\n", "not found\n");
  test(tree.searchKeyIterative(4), "found\n", "not found\n");

  int nodesToDelete[]{ 5, 11, 14, 15 };
  for (int i : nodesToDelete)
  {
    std::cout << "Trying to delete " << i << ": ";
    test(tree.deleteNode(i), "deleted\n", "not deleted\n");
  }
  testRB(tree);
  tree.insertNode(14, 0);
  testRB(tree);

  BinarySearchTree< int, int > moved(std::move(tree));
}

void testDestructorAfterDelete()
{
  BinarySearchTree< int, int > tree;
  int nodes[]{
    22376, 19148, 9064,  4536,  26826, 7510,  16348, 18792, 23507, 6724,  12960, 11559, 9879,  11023, 18260,
    30890, 3200,  2071,  28381, 32676, 17456, 448,   12726, 13838, 9644,  24207, 2139,  3207,  13422, 2533,
    7402,  10504, 2445,  1443,  2830,  20904, 20377, 2771,  9468,  8436,  4157,  16521, 13710, 17472, 14788,
    12956, 4630,  4888,  1934,  4186,  26033, 25914, 28231, 27376, 20052, 4866,  28271, 22225, 1220,  32728,
    7044,  12356, 17215, 5488,  6378,  30255, 4676,  7822,  15889, 6895,  21118, 24465, 20807, 2358,  4513,
    31062, 25879, 8793,  13356, 30480, 413,   14781, 9249,  8973,  29417, 28937, 17829, 10060, 8700,  2880,
    27160, 5242,  11925, 9964,  21235, 15457, 11831, 4317,  23535, 9510,
  };
  for (int i : nodes)
  {
    tree.insertNode(i, 0);
  }
  testRB(tree);
  std::cout << "Deleting nodes\n";
  int nodesToDelete[]{ 12726, 2830, 1220 };
  for (int i : nodesToDelete)
  {
    tree.deleteNode(i);
  }
  testRB(tree);
  std::cout << "Testing destructor. If not segfaulted, test is successful.\n";
}

void testWalk()
{
  BinarySearchTree< int, int > tree;
  int nodes[]{
    23, 16, 12, 19, 17, 24, 5, 1, 4, 7, 11, 6, 22, 20, 9, 27, 10, 2, 13, 15, 25, 18, 3, 14, 8, 29, 26, 30, 21, 28,
  };
  for (int i : nodes)
  {
    tree.insertNode(i, 0);
  }
  testRB(tree);
  BinarySearchTree< int, int > secondTree;
  for (int i : nodes)
  {
    secondTree.insertNode(((i + 6) % 30) + 1, 0); // shuffling nodes.
  }
  testRB(secondTree);
  // std::cout << "Are two equal trees equal??? "
  //           << ((tree.isSimilar(secondTree)) ? "yes" : "no") << '\n';
  // secondTree.insertNode(0);
  // std::cout << "Are two unequal trees equal??? "
  //           << ((tree.isSimilar(secondTree)) ? "yes" : "no") << '\n';

  BinarySearchTree< int, int > thirdTree;
  int otherNodes[]{ 35, 33, 39, 32, 38, 31, 37, 40, 34, 36, 0 };
  for (int i : otherNodes)
  {
    thirdTree.insertNode(i, 0);
  };
  testRB(thirdTree);
  BinarySearchTree< int, int > forthTree;
  BinarySearchTree< int, int > fifthTree;
  // std::cout << "Are two empty trees equal??? "
  //           << ((forthTree.isSimilar(fifthTree)) ? "yes" : "no") << '\n';
}
int main()
{
  testTree();
  testDestructorAfterDelete();
  testWalk();
  std::cout << "Success\n";
  return 0;
}
