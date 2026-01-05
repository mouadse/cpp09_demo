#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN &other) : myStack(other.myStack) {}

RPN &RPN::operator=(const RPN &other) {
  if (this != &other) {
    myStack = other.myStack;
  }
  return *this;
}

RPN::~RPN() {}

bool RPN::isNumber(const char c) { return (c >= '0' && c <= '9'); }
bool RPN::isOperator(const char c) {
  return (c == '+' || c == '-' || c == '*' || c == '/');
}

void RPN::performOperation(const char op) {
  if (myStack.size() < 2)
    throw std::runtime_error("Error: Not enough operands in the stack");
  int b = myStack.top();
  myStack.pop();
  int a = myStack.top();
  myStack.pop();

  int res;
  switch (op) {
  case '+':
    res = a + b;
    break;
  case '-':
    res = a - b;
    break;
  case '*':
    res = a * b;
    break;
  case '/':
    if (b == 0)
      throw std::runtime_error("Error");
    res = a / b;
    break;
  default:
    throw std::runtime_error("Error");
  }

  myStack.push(res);
}

int RPN::calc(const std::string &expression) {
  std::istringstream ss(expression);
  std::string token;

  while (ss >> token) {

    if (token.empty()) {
      continue;
    }

    if (token.length() != 1) {
      throw std::runtime_error("Error");
    }

    if (token.length() == 1 && isNumber(token[0])) {
      myStack.push(token[0] - '0');
    } else if (token.length() == 1 && isOperator(token[0])) {
      performOperation(token[0]);
    } else {
      throw std::runtime_error("Error");
    }
  }

  if (myStack.size() != 1) {
    throw std::runtime_error(
        "Error: The stack should contain exactly one element after evaluation");
  }

  return myStack.top();
}