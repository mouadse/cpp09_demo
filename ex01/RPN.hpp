#ifndef RPN_HPP
#define RPN_HPP
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>

class RPN {
private:
  std::stack<int> myStack;
  bool isNumber(const char c);
  bool isOperator(const char c);
  void performOperation(const char op);

public:
  RPN();
  RPN(const RPN &other);
  RPN &operator=(const RPN &other);
  ~RPN();
  int calc(const std::string &expression);
};

#endif
