#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other)
    : _vecStorage(other._vecStorage), _deqStorage(other._deqStorage) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &other) {
  if (this != &other) {
    _vecStorage = other._vecStorage;
    _deqStorage = other._deqStorage;
  }
  return *this;
}

PmergeMe::~PmergeMe() {}

bool PmergeMe::isValidNumber(const char *str) {

  if (!str || *str == '\0')
    return false;

  for (size_t i = 0; str[i] != '\0'; i++) {
    if (str[i] < '0' || str[i] > '9') {
      return false;
    }
  }

  char *endptr;
  long val = std::strtol(str, &endptr, 10);

  if (*endptr != '\0' || endptr == str) {
    return false;
  }

  if (val < 0 || val > INT_MAX) {
    return false;
  }

  return true;
}

bool PmergeMe::parseInput(int argc, char **argv) {

  if (argc < 2) {
    return false;
  }
  for (int i = 1; i < argc; ++i) {
    if (!isValidNumber(argv[i])) {
      return false;
    }
    int num = std::atoi(argv[i]);
    _vecStorage.push_back(num);
    _deqStorage.push_back(num);
  }
  return true;
}
void PmergeMe::sortAndTime() {
  displaySequence(_vecStorage, "Before");

  std::clock_t vecStart = std::clock();
  mergeInsertSortVector();
  std::clock_t vecEnd = std::clock();
  double vecTime =
      static_cast<double>(vecEnd - vecStart) / CLOCKS_PER_SEC * 1000000;

  std::clock_t deqStart = std::clock();
  mergeInsertSortDeque();
  std::clock_t deqEnd = std::clock();
  double deqTime =
      static_cast<double>(deqEnd - deqStart) / CLOCKS_PER_SEC * 1000000;

  displaySequence(_vecStorage, "After");

  std::cout << "Time to process a range of " << _vecStorage.size()
            << " elements with std::vector : " << std::fixed
            << std::setprecision(5) << vecTime << " us" << std::endl;

  std::cout << "Time to process a range of " << _deqStorage.size()
            << " elements with std::deque  : " << std::fixed
            << std::setprecision(5) << deqTime << " us" << std::endl;
}

std::vector<int> PmergeMe::generateJacobsthalSequence(size_t limit) {
  std::vector<int> sequence;

  sequence.reserve(15);

  sequence.push_back(0);
  sequence.push_back(1);

  while (true) {
    // J(n) = J(n-1) + 2 * J(n-2)
    int nextValue = sequence.back() + 2 * *(sequence.end() - 2);

    if (nextValue >= static_cast<int>(limit))
      break;

    sequence.push_back(nextValue);
  }

  return sequence;
}

std::vector<int> PmergeMe::calculateInsertionOrder(size_t pendingCount) {
  std::vector<int> jacobsthalNums =
      generateJacobsthalSequence(pendingCount);

  std::vector<int> sequence;
  if (pendingCount < 2)
    return sequence;
  sequence.push_back(1);
  size_t lastProcessed = 1;

  for (size_t i = 3; i < jacobsthalNums.size(); i++) {
    int nextJacobsthal = jacobsthalNums[i];

    int upperLimit =
        std::min(nextJacobsthal, static_cast<int>(pendingCount) - 1);
    for (int idx = upperLimit; idx > static_cast<int>(lastProcessed); idx--) {
      sequence.push_back(idx);
    }
    lastProcessed = nextJacobsthal;
  }

  for (int idx = static_cast<int>(pendingCount) - 1;
       idx > static_cast<int>(lastProcessed); idx--) {
    sequence.push_back(idx);
  }

  return sequence;
}


// bool PmergeMe::isSorted() const {
//   for (size_t i = 1; i < _vecStorage.size(); ++i) {
//     if (_vecStorage[i - 1] > _vecStorage[i]) {
//       return false;
//     }
//   }
//   for (size_t i = 1; i < _deqStorage.size(); ++i) {
//     if (_deqStorage[i - 1] > _deqStorage[i]) {
//       return false;
//     }
//   }

//   return true;
// }

void PmergeMe::mergeInsertSortVector() {
  _vecStorage = runMergeInsertionSort(_vecStorage);
}

std::vector<int> PmergeMe::runMergeInsertionSort(std::vector<int> elements) {
  if (elements.size() <= 1) {
    return elements;
  }

  int unpairedElement = -1;
  bool hasUnpaired = (elements.size() % 2 != 0);
  if (hasUnpaired) {
    unpairedElement = elements.back();
    elements.pop_back();
  }

  std::vector<std::pair<int, int> > pairs;
  pairs.reserve(elements.size() / 2);

  for (size_t i = 0; i < elements.size(); i += 2) {
    int candidateA = elements[i];
    int candidateB = elements[i + 1];

    if (candidateA > candidateB) {
      pairs.push_back(std::make_pair(candidateA, candidateB));
    } else {
      pairs.push_back(std::make_pair(candidateB, candidateA));
    }
  }

  std::vector<int> largerElements;
  largerElements.reserve(pairs.size());
  for (size_t i = 0; i < pairs.size(); i++) {
    largerElements.push_back(pairs[i].first);
  }

  std::vector<int> sortedLarger = runMergeInsertionSort(largerElements);

  std::vector<int> smallerElements;
  smallerElements.reserve(pairs.size());
  for (size_t i = 0; i < sortedLarger.size(); i++) {
    for (size_t j = 0; j < pairs.size(); j++) {
      if (pairs[j].first == sortedLarger[i]) {
        smallerElements.push_back(pairs[j].second);
        pairs[j].first = -1;
        break;
      }
    }
  }

  std::vector<int> sortedSequence = sortedLarger;
  std::vector<size_t> positionOf(sortedLarger.size());
  for (size_t i = 0; i < positionOf.size(); ++i) {
    positionOf[i] = i;
  }
  sortedSequence.insert(sortedSequence.begin(), smallerElements[0]);
  for (size_t i = 0; i < positionOf.size(); ++i) {
    ++positionOf[i];
  }

  std::vector<int> insertionOrder =
      calculateInsertionOrder(smallerElements.size());

  for (size_t i = 0; i < insertionOrder.size(); i++) {
    int idx = insertionOrder[i];

    if (idx <= 0 || idx >= (int)smallerElements.size())
      continue;

    int valueToInsert = smallerElements[idx];
    size_t searchBound = positionOf[idx];
    std::vector<int>::iterator position =
        std::lower_bound(sortedSequence.begin(),
                         sortedSequence.begin() + searchBound, valueToInsert);

    size_t insertPos = static_cast<size_t>(position - sortedSequence.begin());
    sortedSequence.insert(position, valueToInsert);
    for (size_t k = 0; k < positionOf.size(); ++k) {
      if (positionOf[k] >= insertPos) {
        ++positionOf[k];
      }
    }
  }

  if (hasUnpaired) {
    std::vector<int>::iterator position = std::lower_bound(
        sortedSequence.begin(), sortedSequence.end(), unpairedElement);
    sortedSequence.insert(position, unpairedElement);
  }

  return sortedSequence;
}

void PmergeMe::mergeInsertSortDeque() {
  _deqStorage = runMergeInsertionSortDeque(_deqStorage);
}

std::deque<int> PmergeMe::generateJacobsthalSequenceDeque(size_t limit) {
  std::deque<int> sequence;

  sequence.push_back(0);
  sequence.push_back(1);

  while (true) {
    // J(n) = J(n-1) + 2 * J(n-2)
    int nextValue = sequence.back() + 2 * *(sequence.end() - 2);

    if (nextValue >= static_cast<int>(limit))
      break;

    sequence.push_back(nextValue);
  }

  return sequence;
}

std::deque<int> PmergeMe::calculateInsertionOrderDeque(size_t pendingCount) {
  std::deque<int> jacobsthalNums =
      generateJacobsthalSequenceDeque(pendingCount);

  std::deque<int> sequence;
  if (pendingCount < 2)
    return sequence;
  sequence.push_back(1);
  size_t lastProcessed = 1;

  for (size_t i = 3; i < jacobsthalNums.size(); i++) {
    int nextJacobsthal = jacobsthalNums[i];

    int upperLimit =
        std::min(nextJacobsthal, static_cast<int>(pendingCount) - 1);
    for (int idx = upperLimit; idx > static_cast<int>(lastProcessed); idx--) {
      sequence.push_back(idx);
    }
    lastProcessed = nextJacobsthal;
  }

  for (int idx = static_cast<int>(pendingCount) - 1;
       idx > static_cast<int>(lastProcessed); idx--) {
    sequence.push_back(idx);
  }

  return sequence;
}

std::deque<int> PmergeMe::runMergeInsertionSortDeque(std::deque<int> elements) {
  if (elements.size() <= 1) {
    return elements;
  }

  int unpairedElement = -1;
  bool hasUnpaired = (elements.size() % 2 != 0);
  if (hasUnpaired) {
    unpairedElement = elements.back();
    elements.pop_back();
  }

  std::deque<std::pair<int, int> > pairs;

  for (size_t i = 0; i < elements.size(); i += 2) {
    int candidateA = elements[i];
    int candidateB = elements[i + 1];

    if (candidateA > candidateB) {
      pairs.push_back(std::make_pair(candidateA, candidateB));
    } else {
      pairs.push_back(std::make_pair(candidateB, candidateA));
    }
  }

  std::deque<int> largerElements;
  for (size_t i = 0; i < pairs.size(); i++) {
    largerElements.push_back(pairs[i].first);
  }

  std::deque<int> sortedLarger = runMergeInsertionSortDeque(largerElements);

  std::deque<int> smallerElements;
  for (size_t i = 0; i < sortedLarger.size(); i++) {
    for (size_t j = 0; j < pairs.size(); j++) {
      if (pairs[j].first == sortedLarger[i]) {
        smallerElements.push_back(pairs[j].second);
        pairs[j].first = -1;
        break;
      }
    }
  }

  std::deque<int> sortedSequence = sortedLarger;
  std::vector<size_t> positionOf(sortedLarger.size());
  for (size_t i = 0; i < positionOf.size(); ++i) {
    positionOf[i] = i;
  }
  sortedSequence.push_front(smallerElements[0]);
  for (size_t i = 0; i < positionOf.size(); ++i) {
    ++positionOf[i];
  }

  std::deque<int> insertionOrder =
      calculateInsertionOrderDeque(smallerElements.size());

  for (size_t i = 0; i < insertionOrder.size(); i++) {
    int idx = insertionOrder[i];

    if (idx <= 0 || idx >= (int)smallerElements.size())
      continue;

    int valueToInsert = smallerElements[idx];
    size_t searchBound = positionOf[idx];
    std::deque<int>::iterator position =
        std::lower_bound(sortedSequence.begin(),
                         sortedSequence.begin() + searchBound, valueToInsert);

    size_t insertPos = static_cast<size_t>(position - sortedSequence.begin());
    sortedSequence.insert(position, valueToInsert);
    for (size_t k = 0; k < positionOf.size(); ++k) {
      if (positionOf[k] >= insertPos) {
        ++positionOf[k];
      }
    }
  }

  if (hasUnpaired) {
    std::deque<int>::iterator position = std::lower_bound(
        sortedSequence.begin(), sortedSequence.end(), unpairedElement);
    sortedSequence.insert(position, unpairedElement);
  }

  return sortedSequence;
}
