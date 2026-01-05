#include "RPN.hpp"
#include <iostream>

int main(int argc, char **argv) {

  if (2 != argc) {
    std::cerr << "Error" << std::endl;
    return 1;
  }

  try {
    RPN calc;
    int res = calc.calc(argv[1]);
    std::cout << res << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error" << std::endl;
    return 1;
  }

  return 0;
}
