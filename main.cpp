#include <algorithm>
#include <iostream>
#include <iterator>
#include "RBTree.hpp"

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
  demidenko::RBTree< int, int > tree;
  int nodes[]{ 10, 5, 13, 4, 7, 11, 14 };
  for (int i : nodes)
  {
    tree.insert(i, 0);
  }
  testRB(tree);
  test(tree.insert(13, 0), "inserted\n", "not inserted\n");
  test(tree.insert(4, 0), "inserted\n", "not inserted\n");
  testRB(tree);
  test(tree.find(100), "found\n", "not found\n");
  test(tree.find(4), "found\n", "not found\n");

  tree.insert(15, 0);
  tree.insert(16, 0);
  testRB(tree);
  tree.walkByLevels(std::cout);
  std::cout << '\n';

  int nodesToDelete[]{ 5, 11, 14, 15 };
  for (int i : nodesToDelete)
  {
    std::cout << "Trying to delete " << i << ": ";
    test(tree.erase(i), "deleted\n", "not deleted\n");
  }
  testRB(tree);
  tree.insert(14, 0);
  testRB(tree);

  demidenko::RBTree< int, int > moved(std::move(tree));
}

void testDestructorAfterDelete()
{
  demidenko::RBTree< int, int > tree;
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
    tree.insert(i, 0);
  }
  testRB(tree);
  std::cout << "Deleting nodes\n";
  int nodesToDelete[]{ 12726, 2830, 1220 };
  for (int i : nodesToDelete)
  {
    tree.erase(i);
  }
  testRB(tree);
  std::cout << "Testing destructor. If not segfaulted, test is successful.\n";
}

void testWalk()
{
  demidenko::RBTree< int, int > tree;
  int nodes[]{
    23, 16, 12, 19, 17, 24, 5, 1, 4, 7, 11, 6, 22, 20, 9, 27, 10, 2, 13, 15, 25, 18, 3, 14, 8, 29, 26, 30, 21, 28,
  };
  for (int i : nodes)
  {
    tree.insert(i, 0);
  }
  testRB(tree);
  demidenko::RBTree< int, int > secondTree;
  for (int i : nodes)
  {
    secondTree.insert(((i + 6) % 30) + 1, 0); // shuffling nodes.
  }
  testRB(secondTree);
  // std::cout << "Are two equal trees equal??? "
  //           << ((tree.isSimilar(secondTree)) ? "yes" : "no") << '\n';
  // secondTree.insertNode(0);
  // std::cout << "Are two unequal trees equal??? "
  //           << ((tree.isSimilar(secondTree)) ? "yes" : "no") << '\n';

  demidenko::RBTree< int, int > thirdTree;
  int otherNodes[]{ 35, 33, 39, 32, 38, 31, 37, 40, 34, 36, 0 };
  for (int i : otherNodes)
  {
    thirdTree.insert(i, 0);
  };
  testRB(thirdTree);
  demidenko::RBTree< int, int > forthTree;
  demidenko::RBTree< int, int > fifthTree;
  // std::cout << "Are two empty trees equal??? "
  //           << ((forthTree.isSimilar(fifthTree)) ? "yes" : "no") << '\n';
}
void testIterators()
{
  demidenko::RBTree< int, int > tree;
  int nodes[]{ 10, 5, 13, 4, 7, 11, 14 };
  for (int i : nodes)
  {
    tree.insert(i, 0);
  }
  std::transform(tree.cbegin(), tree.cend(), std::ostream_iterator< int >(std::cout, " "), [&](auto pair) {
    return pair.first;
  });
  std::cout << '\n';
  tree.begin()->second = 9;
  std::transform(tree.cbegin(), tree.cend(), std::ostream_iterator< int >(std::cout, " "), [&](auto pair) {
    return pair.second;
  });
  std::cout << '\n';
  // tree.cbegin()->second = 9;
  // const demidenko::RBTree< int, int > consttree;
  // consttree.begin()->second = 9;
}
int main()
{
  testTree();
  testDestructorAfterDelete();
  testWalk();
  testIterators();
  std::cout << "Success\n";
  return 0;
}
