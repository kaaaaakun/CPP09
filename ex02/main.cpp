#include <deque>
#include <exception>
#include <iostream>

#include "PmergeMe.hpp"
#include "StopWatch.hpp"

bool CheckArgs(char *argv[]) {
  for (size_t i = 0; argv[i] != NULL; ++i) {
    bool sign_flag = false;
    bool nbr_flag = false;
    for (size_t j = 0; argv[i][j] != '\0'; ++j) {
      if ((argv[i][j] == '+' || argv[i][j] == '-') && sign_flag == false) {
        sign_flag = true;
        if (argv[i][j] == '-') return false;
      } else if (std::isdigit(argv[i][j]) == 0)
        return false;
      else {
        sign_flag = true;
        nbr_flag = true;
      }
    }
    if (nbr_flag == false) return false;
  }
  return true;
}

void PutInput(char *argv[]) {
  std::cout << "Input : ";
  for (size_t i = 1; argv[i] != NULL; ++i) {
    std::cout << argv[i] << " ";
    if (i == 20) {
      std::cout << "......" << std::endl;
      return;
    }
  }
  std::cout << std::endl;
}

template <typename Container>
void PutSorted(Container &container) {
  std::cout << "Sorted: ";
  for (typename Container::iterator it = container.begin();
       it != container.end(); ++it) {
    std::cout << *it << " ";
    if (std::distance(container.begin(), it) == 20) {
      std::cout << "......" << std::endl;
      return;
    }
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  try {
    if (argc < 2 || !CheckArgs(&argv[1])) {
      std::cerr << "ERROR" << std::endl;
      std::cerr << "Usage: ./PmergeMe [int] [int] ..." << std::endl;
      return EXIT_FAILURE;
    }
    StopWatch sw;

    PutInput(argv);
    std::vector<int> vec = PmergeMe<std::vector<int> >::Sort(argv);
    PutSorted(vec);

    std::cout << "number of elements : " << vec.size() << std::endl;

#ifndef DEBUG
    sw.Start();
    std::vector<int> vec2 = PmergeMe<std::vector<int> >::Sort(argv);
    std::cout << "vector: processing time : ";
    sw.Stop();

    sw.Start();
    std::list<int> lst = PmergeMe<std::list<int> >::Sort(argv);
    std::cout << "list  : processing time : ";
    sw.Stop();

    sw.Start();
    std::deque<int> dequ = PmergeMe<std::deque<int> >::Sort(argv);
    std::cout << "dequ  : processing time : ";
    sw.Stop();

    PutSorted(vec2);
    PutSorted(lst);
#endif

  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
