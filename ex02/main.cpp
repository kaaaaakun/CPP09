#include <exception>
#include <iostream>

#include "PmergeMe.hpp"

bool CheckArgs(char *argv[]) {
  for (size_t i = 0; argv[i] != NULL; ++i) {
    bool sign_flag = false;
    bool nbr_flag = false;
    for (size_t j = 0; argv[i][j] != '\0'; ++j) {
      if ((argv[i][j] == '+' || argv[i][j] == '-') && sign_flag == false)
        sign_flag = true;
      else if (std::isdigit(argv[i][j]) == 0)
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
  for (size_t i = 1; argv[i] != NULL; ++i) std::cout << argv[i] << " ";
  std::cout << std::endl;
}

template <typename Container>
void PutSorted(Container &container) {
  std::cout << "Sorted: ";
  for (typename Container::iterator it = container.begin();
       it != container.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  try {
    if (argc < 2 || !CheckArgs(&argv[1])) {
      std::cerr << "だめだよ" << std::endl;
      std::cerr << "Usage: ./PmergeMe [int] [int] ..." << std::endl;
      return EXIT_FAILURE;
    }
    StopWatch sw;

    PutInput(argv);
    std::vector<int> vec = PmergeMe<std::vector<int> >::Sort(argv);
    PutSorted(vec);

    std::cout << "number of elements : " << vec.size() << std::endl;

    sw.Start();
    std::vector<int> vec2 = PmergeMe<std::vector<int> >::Sort(argv);
    std::cout << "vector: processing time : ";
    sw.Stop();

    sw.Start();
    std::list<int> lst = PmergeMe<std::list<int> >::Sort(argv);
    std::cout << "list  : processing time : ";
    sw.Stop();

    // PutSorted(vec2);
    // PutSorted(lst);

  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
