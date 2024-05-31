#include "RPN.hpp"

#include <unistd.h>

double RPN::Execute(std::string str) {
  std::stack<std::string> st;
  double ret;
  std::stringstream ss;
  size_t i = 0;

  while (i < str.size()) {
    while (i < str.size() && str[i] == ' ') ++i;
    if (std::isdigit(str[i])) {
      size_t j = i;
      while (i < str.size() && (std::isdigit(str[i]) || str[i] == '.')) ++i;
      st.push(str.substr(j, i - j));
    } else if (IsOperator(str[i])) {
      Calculate(st, str.substr(i++, 1));
    } else if (i == str.size()) {
      continue;
    } else {
      throw std::invalid_argument(
          "Error: invalid argument");
    }
  }
  if (st.size() != 1)
    throw std::invalid_argument(
        "Error: invalid argument");

  ss << st.top();
  ss >> ret;
  return ret;
}

bool RPN::IsOperator(char c) {
  return c == '+' || c == '/' || c == '-' || c == '*';
}

double RPN::StrToDouble(std::string str) {
  std::stringstream ss(str);
  double ret;

  IsValidDouble(str);
  ss >> ret;
  return ret;
}

void RPN::IsValidDouble(std::string str) {
  int count = 0;

  if (str[count] == '+' || str[count] == '-') count++;
  while (str[count] != '\0' && str[count] != '.') {
    if (!isdigit(str[count]))
      throw std::invalid_argument("Error: invalid argument: 数字以外がるよ");
    count++;
  }
  if (str[count] == '\0')
    return;
  else if (str[++count] == '\0')
    throw std::invalid_argument(
        "Error: invalid argument: .のうしろに何も書いてないよ");
  else {
    while (str[count] != '\0') {
      if (!isdigit(str[count]))
        throw std::invalid_argument(
            "Error: invalid argument: 2回目に数字以外があるよ");
      count++;
    }
  }
}

void RPN::Calculate(std::stack<std::string>& st, std::string ope) {
  double num2, num1, result = 0;

  if (st.size() < 2)
    throw std::invalid_argument("Error: invalid argument");
  num2 = StrToDouble(st.top());
  st.pop();
  num1 = StrToDouble(st.top());
  st.pop();

  if (ope == "+")
    result = num1 + num2;
  else if (ope == "-")
    result = num1 - num2;
  else if (ope == "*")
    result = num1 * num2;
  else if (ope == "/")
    result = num1 / num2;

  st.push(std::to_string(result));
}
