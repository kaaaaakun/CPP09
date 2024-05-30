#include <iomanip>

#include "RPN.hpp"

int main(int argc, char *argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Invalid argument" << std::endl;
      std::cerr << "usage: ./RPN \"8 9 * 9 - 9 - 9 - 4 - 1 +\"" << std::endl;
      return EXIT_FAILURE;
    }
    double ret = RPN::Execute(argv[1]);
    std::cout << std::fixed << std::setprecision(0) << ret << std::endl;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
