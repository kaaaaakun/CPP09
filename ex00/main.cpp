#include "BitcoinExchange.hpp"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Invalid argument" << std::endl;
    std::cerr << "usage: ./btc input.txt" << std::endl;
    return EXIT_FAILURE;
  }
  try {
    BitcoinExchange bitcoinExcenger(DEFAULT_CSV_DATA_FILE);
    bitcoinExcenger.Exchange(argv[1]);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
