#ifndef RPN_HPP
#define RPN_HPP
#include <cctype>
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>

class RPN {
 public:
  static double Execute(std::string str);

 private:
  static bool IsOperator(char c);
  static void Calculate(std::stack<std::string>& st);
  static double StrToDouble(std::string str);
  static void IsValidDouble(std::string str);
};

#endif  // RPN_HPP
