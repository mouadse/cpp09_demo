#include "PmergeMe.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Error" << std::endl;
    return 0;
  }

  PmergeMe sorter;

  if (!sorter.parseInput(argc, argv)) {
    std::cerr << "Error" << std::endl;
    return 1;
  }

  sorter.sortAndTime();

  return 0;
}
