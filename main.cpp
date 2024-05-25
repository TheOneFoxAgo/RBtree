#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include "RBTree.hpp"

void test(const char* testName, bool isSuccess)
{
  std::cout << testName << ": ";
  if (isSuccess)
  {
    std::cout << "\033[32;1mSuccess\033[0m\n";
  }
  else
  {
    std::cout << "\033[31;1mFail\033[0m\n";
  }
}
template < class T >
void testRB(const T& tree)
{
  test("RB test", tree.isRBTree());
}
void testTree()
{
  std::cout << "Basic tests\n";
  demidenko::RBTree< int, int > tree;
  int nodes[]{ 10, 5, 13, 4, 7, 11, 14 };
  for (int i : nodes)
  {
    tree.insert(i, 0);
  }
  testRB(tree);
  test("insert existing(13)", !tree.insert(13, 0));
  test("insert existing(4)", !tree.insert(4, 0));
  testRB(tree);
  test("find existing(4)", tree.find(4));
  test("find not existing(100)", !tree.find(100));

  tree.insert(15, 0);
  tree.insert(16, 0);
  testRB(tree);

  std::cout << "Tree levels: ";
  tree.walkByLevels(std::cout);
  std::cout << '\n';

  int nodesToDelete[]{ 5, 11, 14 };
  for (int i : nodesToDelete)
  {
    std::cout << '(' << i << ')';
    test("erase existing", tree.erase(i));
  }
  testRB(tree);
  test("insert after delete", tree.insert(14, 0));
  testRB(tree);

  std::cout << "copy/move test. If not segfaulted, test is passed\n";
  demidenko::RBTree< int, int > moved(std::move(tree));
  testRB(moved);
  demidenko::RBTree< int, int > copied(moved);
  testRB(copied);
  test("copy is equal", std::equal(moved.begin(), moved.end(), copied.begin()));
  copied = std::move(moved);
  testRB(copied);
  moved = copied;
  testRB(moved);
  test("copy-assigned is equal", std::equal(moved.begin(), moved.end(), copied.begin()));
}
void testInsertBalancing()
{
  std::cout << "Insert balancing test\n";
  demidenko::RBTree< int, int > leftInsertTree;
  for (int i : { 0, 1, -1 })
  {
    leftInsertTree.insert(i, 0);
  }
  demidenko::RBTree< int, int > rightInsertTree(leftInsertTree);
  leftInsertTree.insert(-3, 0);
  rightInsertTree.insert(3, 0);
  testRB(leftInsertTree);
  testRB(rightInsertTree);
  leftInsertTree.insert(-2, 0);
  rightInsertTree.insert(2, 0);
  testRB(leftInsertTree);
  testRB(rightInsertTree);
}
void testEraseBalancing()
{
  std::cout << "Erase balancing test\n";
  demidenko::RBTree< int, int > leftEraseTree;
  for (int i : { 0, 2, -2, -3 })
  {
    leftEraseTree.insert(i, 0);
  }
  leftEraseTree.erase(-3);
  demidenko::RBTree< int, int > backup(leftEraseTree);
  demidenko::RBTree< int, int > rightEraseTree(leftEraseTree);
  leftEraseTree.erase(-2);
  rightEraseTree.erase(2);
  testRB(leftEraseTree);
  testRB(rightEraseTree);

  leftEraseTree = backup;
  rightEraseTree = backup;
  leftEraseTree.insert(1, 0);
  rightEraseTree.insert(-1, 0);
  leftEraseTree.erase(-2);
  rightEraseTree.erase(2);
  testRB(leftEraseTree);
  testRB(rightEraseTree);

  leftEraseTree = backup;
  rightEraseTree = backup;
  leftEraseTree.insert(3, 0);
  rightEraseTree.insert(-3, 0);
  leftEraseTree.erase(-2);
  rightEraseTree.erase(2);
  testRB(leftEraseTree);
  testRB(rightEraseTree);

  leftEraseTree = backup;
  rightEraseTree = backup;
  leftEraseTree.insert(3, 0);
  leftEraseTree.insert(4, 0);
  rightEraseTree.insert(-3, 0);
  rightEraseTree.insert(-4, 0);
  leftEraseTree.erase(-2);
  rightEraseTree.erase(2);
  testRB(leftEraseTree);
  testRB(rightEraseTree);
}
void testDestructorAfterErase()
{
  std::cout << "Destructor + erase test\n";
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
  std::cout << "Erasing nodes\n";
  int nodesToErase[]{ 12726, 2830, 1220 };
  for (int i : nodesToErase)
  {
    tree.erase(i);
  }
  testRB(tree);
  std::cout << "Testing destructor. If not segfaulted, test is successful.\n";
}

void testIterators()
{
  std::cout << "Iterator test\n";
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
  test("equal trees are equal", std::equal(tree.begin(), tree.end(), secondTree.begin()));
  secondTree.insert(0, 0);
  test("not equal trees are not equal", !std::equal(tree.begin(), tree.end(), secondTree.begin()));

  demidenko::RBTree< int, int > shortTree;
  int handfulOfNodes[]{ 10, 5, 13, 4, 7, 11, 14 };
  for (int i : handfulOfNodes)
  {
    shortTree.insert(i, 0);
  }
  std::transform(shortTree.cbegin(), shortTree.cend(), std::ostream_iterator< int >(std::cout, " "), [&](auto pair) {
    return pair.first;
  });
  std::cout << '\n';
  shortTree.begin()->second = 9;
  std::transform(shortTree.cbegin(), shortTree.cend(), std::ostream_iterator< int >(std::cout, " "), [&](auto pair) {
    return pair.second;
  });
  std::cout << '\n';
  // Does not compile:
  // tree.cbegin()->second = 9;
  // const demidenko::RBTree< int, int > consttree;
  // consttree.begin()->second = 9;
}
int main()
{
  testTree();
  std::cout << '\n';
  testInsertBalancing();
  std::cout << '\n';
  testEraseBalancing();
  std::cout << '\n';
  testDestructorAfterErase();
  std::cout << '\n';
  testIterators();
  return 0;
}
