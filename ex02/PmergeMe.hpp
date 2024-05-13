#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>

class StopWatch {
 public:
  StopWatch() : start_(0), stop_(0) {}
  void Start() { start_ = std::clock(); }
  void Stop() {
    stop_ = std::clock();
    std::cout << "Time: "
              << static_cast<double>(stop_ - start_) / CLOCKS_PER_SEC * 1000.0
              << " ms" << std::endl;
  }

 private:
  std::clock_t start_;
  std::clock_t stop_;
};


template <class Container>
class PmergeMe {
 private:
  PmergeMe() {}
  ~PmergeMe() {}
  static void RecursiveMergeInsertionSort(size_t chunk_size,
                                          Container &container);
  static void ChunkedMergeSort(size_t chunk_size, Container &container);
  static void ChunkedInsertSort(size_t chunk_size, Container &container);
  static Container ArgvToIntContainer(char *argv[]);
  static typename Container::iterator Advance(typename Container::iterator it,
                                              long long int count);

  static void Debug(Container &container);

 public:
  static Container Sort(char *argv[]);
};

#endif  // PMERGEME_HPP

// std::advance を使いやすく
template <class Container>
typename Container::iterator PmergeMe<Container>::Advance(
    typename Container::iterator it, long long int count) {
  std::advance(it, count);
  return it;
}

// ソートするメインの関数
template <class Container>
Container PmergeMe<Container>::Sort(char *argv[]) {
  Container container;
  container = ArgvToIntContainer(argv);
  RecursiveMergeInsertionSort(1, container);
  return container;
}

// ダブルポインタから型の変換を行う
template <class Container>
Container PmergeMe<Container>::ArgvToIntContainer(char *argv[]) {
  Container container;

  for (int i = 1; argv[i] != NULL; ++i) container.push_back(std::atoi(argv[i]));

  return container;
}

// チャンクごとのマージソートを行う
template <class Container>
void PmergeMe<Container>::ChunkedMergeSort(size_t chunk_size,
                                           Container &container) {
  size_t container_size = container.size();
  typename Container::iterator it = container.begin();

  // std::cout << "-- chunk_size : " << chunk_size << " -- " << std::endl;
  for (size_t i = 0; i + chunk_size * 2 <= container_size;
       i += chunk_size * 2) {
    typename Container::iterator it0 = Advance(it, i);
    typename Container::iterator it1 = Advance(it, i + chunk_size - 1);
    typename Container::iterator it2 = Advance(it, i + chunk_size * 2 - 1);
    // std::cout << "比較 : " << *it1 << " : " << *it2 << std::endl;
    //  ペアの値を昇順にする
    if (*it2 < *it1) {
      std::advance(it1, 1);
      std::advance(it2, 1);
      std::rotate(it0, it1, it2);
    }
  }
}

// 　チャンクを考慮して、インサーションソートを行う
template <class Container>
void PmergeMe<Container>::ChunkedInsertSort(size_t chunk_size,
                                            Container &container) {
  size_t container_size = container.size();
  typename Container::iterator it = container.begin();

  // std::cout << "-- chunk_size : " << chunk_size << " -- " << std::endl;
  for (size_t i = chunk_size * 3 - 1; i + chunk_size <= container_size;
       i += chunk_size * 2) {
    typename Container::iterator it0 = Advance(it, i);  // 挿入される値
    for (size_t j = chunk_size - 1; j + chunk_size <= container_size && i != j;
         j += chunk_size) {
      typename Container::iterator it1 = Advance(it, j);
      // std::cout << "比較 : " << *it1 << " : " << *it0 << std::endl;
      if (*it0 < *it1) {
        std::advance(it0, 1);  // it0を1つ進める
        typename Container::iterator it2 = Advance(it0, -chunk_size);
        // std::cout << "rotate it2:" << *it2 << std::endl;
        // std::cout << "入れ替え" << std::endl;

        std::advance(it1, -chunk_size + 1);
        std::rotate(it1, it2, it0);
        break;
      }
    }
  }
}

// 再帰でソートする
template <class Container>
void PmergeMe<Container>::RecursiveMergeInsertionSort(size_t chunk_size,
                                                      Container &container) {
  size_t container_size = container.size();

  ChunkedMergeSort(chunk_size, container);
  Debug(container);
  if (chunk_size * 4 <= container_size)
    RecursiveMergeInsertionSort(chunk_size * 2, container);
  else
    return;
  ChunkedInsertSort(chunk_size, container);
  Debug(container);
}

// デバック用
template <class Container>
void PmergeMe<Container>::Debug(Container &container) {
  return;
  std::cout << "debug:";
  for (typename Container::iterator it = container.begin();
       it != container.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}
