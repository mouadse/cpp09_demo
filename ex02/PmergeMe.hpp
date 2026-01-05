#ifndef PMERGEME_HPP
#define PMERGEME_HPP
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

class PmergeMe {
private:
  std::vector<int> _vecStorage;
  std::deque<int> _deqStorage;

  bool isValidNumber(const char *str);
  std::vector<int> runMergeInsertionSort(std::vector<int> elements);
  void mergeInsertSortVector();
  void mergeInsertSortDeque();
  std::vector<int> generateJacobsthalSequence(size_t limit);
  std::vector<int> calculateInsertionOrder(size_t pendingCount);

  std::deque<int> runMergeInsertionSortDeque(std::deque<int> elements);
  std::deque<int> generateJacobsthalSequenceDeque(size_t limit);
  std::deque<int> calculateInsertionOrderDeque(size_t pendingCount);

  template <typename T>
  void displaySequence(const T &container, const std::string &header) {
    std::cout << header << ":";

    for (size_t i = 0; i < container.size(); i++) {
      std::cout << " " << container[i];
    }

    std::cout << std::endl;
  }

public:
  PmergeMe();
  PmergeMe(const PmergeMe &other);
  PmergeMe &operator=(const PmergeMe &other);
  ~PmergeMe();

  bool parseInput(int argc, char **argv);
  // bool isSorted() const;
  void sortAndTime();
};

#endif
