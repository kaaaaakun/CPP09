#include <exception>
#include <iostream>

#include "PmergeMe.hpp"

bool CheckArgs(char *argv[]) {
  for (size_t i = 0; argv[i] != NULL; i++) {
    for (size_t j = 0; argv[i][j] != '\0'; j++) {
      if (!std::isdigit(argv[i][j])) return false;
    }
  }
  return true;
}

int main(int argc, char *argv[]) {
  try {
    if (argc < 2 || !CheckArgs(&argv[1])) {
      std::cerr << "だめだよ" << std::endl;
      return EXIT_FAILURE;
    }
    std::vector<int> vec = PmergeMe<std::vector<int> >::Sort(argv);
    std::list<int> lst = PmergeMe<std::list<int> >::Sort(argv);

    std::cout << "- vector : ";
    for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
      std::cout << *it << " ";
    std::cout << std::endl;

    std::cout << "- list   : ";
    for (std::list<int>::iterator it = lst.begin(); it != lst.end(); ++it)
      std::cout << *it << " ";
    std::cout << std::endl;

  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
