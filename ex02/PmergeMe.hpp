#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>

template <class Container>
class PmergeMe {
 private:
  PmergeMe() {}
  ~PmergeMe() {}
  static void RecursiveMergeInsertionSort(size_t chunk_size,
                                          Container &container);
  void ChunkedMergeSort(size_t chunk_size, Container &container);
  void ChunkedInsertSort(size_t chunk_size, Container &container);
  static Container ArgvToIntContainer(char *argv[]);

 public:
  static Container Sort(char *argv[]);
};

#endif  // PMERGEME_HPP

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

template <class Container>
void PmergeMe<Container>::ChunkedMergeSort(size_t chunk_size,
                                           Container &container) {
  std::cout << "chunk size : " << chunk_size << std::endl;
  size_t container_size = container.size();

  typename Container::iterator it = container.begin();
  // int tmp;
  //  forで比較して、入れ替えを行う
  for (size_t i = 0; i + chunk_size * 2 < container_size; i += chunk_size * 2) {
    // イテレータを移動して、正しい比較を行う
    typename Container::iterator it0 = it;  // 挿入場所
    typename Container::iterator it1 = it;  // 最初のイテレーター
    typename Container::iterator it2 = it;  // 次のイテレーター
    std::advance(it0, i);
    std::advance(it1, i + chunk_size - 1);
    std::advance(it2, i + chunk_size * 2 - 1);
    std::cout << "it0: " << *it0 << " ";
    std::cout << "it1: " << *it1 << " it2: " << *it2 << std::endl;
    if (*it2 < *it1) {
      std::cout << "rotate it2:" << *it2 << std::endl;
      std::advance(it1, 1);  // it1を1つ進める
      std::advance(it2, 1);  // it1を1つ進める
      std::rotate(it0, it1, it2);
    }
  }
}

template <class Container>
void PmergeMe<Container>::ChunkedInsertSort(size_t chunk_size,
                                            Container &container) {
  (void)chunk_size;
  (void)container;
}

// 再帰でソートする
template <class Container>
void PmergeMe<Container>::RecursiveMergeInsertionSort(size_t chunk_size,
                                                      Container &container) {
  size_t container_size = container.size();
  std::cout << "------------------------------------------------" << std::endl;
  // PmergeMe(chunk_size,container);
  std::cout << "chunk size : " << chunk_size << std::endl;

  typename Container::iterator it = container.begin();
  // int tmp;
  //  forで比較して、入れ替えを行う
  for (size_t i = 0; i + chunk_size * 2 < container_size; i += chunk_size * 2) {
    // イテレータを移動して、正しい比較を行う
    typename Container::iterator it0 = it;  // 挿入場所
    typename Container::iterator it1 = it;  // 最初のイテレーター
    typename Container::iterator it2 = it;  // 次のイテレーター
    std::advance(it0, i);
    std::advance(it1, i + chunk_size - 1);
    std::advance(it2, i + chunk_size * 2 - 1);
    std::cout << "it0: " << *it0 << " ";
    std::cout << "it1: " << *it1 << " it2: " << *it2 << std::endl;
    if (*it2 < *it1) {
      std::cout << "rotate" << std::endl;
      std::advance(it1, 1);  // it1を1つ進める
      std::advance(it2, 1);  // it1を1つ進める
      std::rotate(it0, it1, it2);
    }
  }

  // 再帰処理
  // chunk_size*2がcontainerのサイズを上回ってたら、再帰
  std::cout << "debug:";
  for (typename Container::iterator it = container.begin();
       it != container.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
  if (chunk_size * 4 < container_size)
    RecursiveMergeInsertionSort(chunk_size * 2, container);
  else
    return;

  // インサートソート
  std::cout << "------------------------------------------------" << std::endl;
  std::cout << "chunk size : " << chunk_size << std::endl;
  for (size_t i = chunk_size * 2; i + chunk_size <= container_size;
       i += chunk_size * 2) {
    typename Container::iterator it0 = it;  // 比較対象になるイテレーター
    std::advance(it0, i + chunk_size - 1);
    std::cout << "it0: " << *it0 << std::endl;
    for (size_t j = 0; j + chunk_size <= container_size; j += chunk_size) {
      // イテレータを移動して、正しい比較を行う
      typename Container::iterator it1 = it;  // 挿入場所
      std::advance(it1, j + chunk_size - 1);
      std::cout << "-it1: " << *it1 << std::endl;
      if (it0 == it1)
          break ;
      else if (*it0 < *it1) {
        std::advance(it0, 1);                    // it0を1つ進める
        typename Container::iterator it2 = it0;  // 挿入場所
        std::advance(it2, -chunk_size);
        std::cout << "rotate it2:" << *it2 << std::endl;

        std::advance(it1, -chunk_size+1);

        std::rotate(it1, it2, it0);
        break;
      }
    }
    std::cout << "debug:";
    for (typename Container::iterator it = container.begin();
         it != container.end(); ++it)
      std::cout << *it << " ";
    std::cout << std::endl;
  }
  std::cout << "debug:";
  for (typename Container::iterator it = container.begin();
       it != container.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}
