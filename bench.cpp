#include <algorithm>
#include <chrono>
#include <fstream>
#include <ios>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include "RBTree.hpp"

const int CYCLES = 1'000;
const int FILLS_PER_CYCLE = 10'000;
int main(int argc, char** argv)
{
  if (argc != 4)
  {
    return 1;
  }
  std::ofstream fill{ argv[1], std::ios_base::app };
  std::ofstream search{ argv[2], std::ios_base::app };
  std::vector< int > randomNumbers(CYCLES * FILLS_PER_CYCLE);
  std::iota(randomNumbers.begin(), randomNumbers.end(), 0);
  if (*argv[3] == 'r')
  {
    std::shuffle(randomNumbers.begin(), randomNumbers.end(), std::random_device());
  }
  demidenko::RBTree< int, int > tree;
  unsigned long long luckyNumber = 0;
  for (int i = 0; i < CYCLES; ++i)
  {
    auto start = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < FILLS_PER_CYCLE; ++j)
    {
      int index = i * FILLS_PER_CYCLE + j;
      tree.insert(randomNumbers[index], 0);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast< std::chrono::nanoseconds >(end - start).count();
    fill << i * FILLS_PER_CYCLE << ' ' << duration / static_cast< double >(FILLS_PER_CYCLE) << '\n';
    start = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < CYCLES; ++j)
    {
      luckyNumber += tree.find(j * FILLS_PER_CYCLE + i);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast< std::chrono::nanoseconds >(end - start).count();
    search << (i + 1) * FILLS_PER_CYCLE << ' ' << duration / static_cast< double >(CYCLES) << '\n';
  }
  std::cout << "Lucky number: " << luckyNumber << '\n';
}
