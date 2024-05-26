#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
#include "RBTree.hpp"

const int SKIP = 10'000;
int main(int argc, char** argv)
{
  if (argc != 3)
  {
    return 1;
  }
  std::ofstream fill{ argv[1], std::ios_base::app };
  std::ofstream search{ argv[2], std::ios_base::app };
  demidenko::RBTree< int, int > tree;
  unsigned long long luckyNumber = 0;
  using InputIt = std::istream_iterator< int >;
  int current = -1;
  for (auto i = InputIt{ std::cin }; i != InputIt{}; ++i, ++current)
  {
    auto start = std::chrono::high_resolution_clock::now();
    tree.insert(*i, 0);
    if (current % SKIP != 0)
    {
      continue;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast< std::chrono::nanoseconds >(end - start).count();
    fill << current << ' ' << duration << '\n';
    start = std::chrono::high_resolution_clock::now();
    luckyNumber += tree.find(std::rand());
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast< std::chrono::nanoseconds >(end - start).count();
    search << current << ' ' << duration << '\n';
  }
  std::cout << "Lucky number: " << luckyNumber << '\n';
}
