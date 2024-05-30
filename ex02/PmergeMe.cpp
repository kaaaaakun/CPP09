#include "PmergeMe.hpp"

// ヤコブスタール数を計算する関数
size_t Jacobsthal(size_t n) {
  if (n == 0) return 0;
  if (n == 1) return 1;
  return Jacobsthal(n - 1) + 2 * Jacobsthal(n - 2);
}