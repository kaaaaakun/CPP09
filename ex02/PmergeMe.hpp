#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>

#include "Debug.hpp"

#define YELLOW "\x1b[38;5;228m"
#define DEFAULT "\x1b[0m"
#define GREEN "\x1b[38;5;40m"
#define GRAY "\x1b[38;5;238m"
#define ORANGE "\x1b[38;5;208m"

template <class Container>
class PmergeMe {
 private:
  typedef typename Container::iterator Iterator;

  PmergeMe() {}
  ~PmergeMe() {}
  static void RecursiveMergeInsertionSort(size_t chunk_size,
                                          Container &container);
  static void ChunkedMergeSort(size_t chunk_size, Container &container);
  static void ChunkedInsertSort(size_t chunk_size, Container &container);

  static void SplitContainer(size_t chunk_size, Container &container,
                             Container &insert_container,
                             Container &inserted_container,
                             size_t &insert_container_size);
  static void InsertIntegrate(size_t chunk_size, Container &container,
                              Container &insert_container,
                              Container &inserted_container,
                              size_t insert_container_size);

  static Container ArgvToIntContainer(char *argv[]);

  static Iterator Advance(Iterator it, long long int count);
  static Iterator LowerBound(Iterator first, Iterator last,
                             const Iterator value, size_t chunk_size);
  static void DebugContainer(Container &container, size_t size);
  static void DebugContainer(Container &container, size_t size, Iterator it1,
                             Iterator it2);
  static size_t CalcJacob(size_t n, size_t max_size);

 public:
  static Container Sort(char *argv[]);
};

size_t Jacobsthal(size_t n);

#endif  // PMERGEME_HPP

// ソートするメインの関数
template <class Container>
Container PmergeMe<Container>::Sort(char *argv[]) {
  Container container;
  container = ArgvToIntContainer(argv);
  RecursiveMergeInsertionSort(1, container);
  return container;
}

// 再帰でソートするメインの関数
template <class Container>
void PmergeMe<Container>::RecursiveMergeInsertionSort(size_t chunk_size,
                                                      Container &container) {
  size_t container_size = container.size();

  Debug::cout() << YELLOW << "\n-- chunk_size : " << chunk_size << " --"
                << DEFAULT << "\n";
  ChunkedMergeSort(chunk_size, container);
  if (chunk_size * 4 <= container_size)
    RecursiveMergeInsertionSort(chunk_size * 2, container);
  else
    return;
  Debug::cout() << YELLOW << "\n-- chunk_size : " << chunk_size << " --"
                << DEFAULT << "\n";
  Debug::cout() << "main_container     : ";
  DebugContainer(container, chunk_size);
  ChunkedInsertSort(chunk_size, container);
}

// チャンクごとにマージソートを行う関数
template <class Container>
void PmergeMe<Container>::ChunkedMergeSort(size_t chunk_size,
                                           Container &container) {
  size_t container_size = container.size();
  Iterator it = container.begin();

  Debug::cout() << "before:";
  DebugContainer(container, chunk_size);
  for (size_t i = 0; i + chunk_size * 2 <= container_size;
       i += chunk_size * 2) {
    Iterator it0 = Advance(it, i);
    Iterator it1 = Advance(it, i + chunk_size - 1);
    Iterator it2 = Advance(it, i + chunk_size * 2 - 1);
    Debug::cout() << "比較  :";
    DebugContainer(container, chunk_size, it1, it2);
    if (*it2 < *it1) {
      std::advance(it1, 1);
      std::advance(it2, 1);
      std::rotate(it0, it1, it2);
    }
  }
  Debug::cout() << "after :";
  DebugContainer(container, chunk_size);
}

// reserveが可能な場合にreserveを行う関数
template <class Container>
void reserve_if_possible(Container &container,
                         typename Container::size_type size) {
  // Do nothing for the general case
  (void)container;
  (void)size;
}

// reserveが可能な場合にreserveを行う関数
template <class T>
void reserve_if_possible(std::vector<T> &container,
                         typename std::vector<T>::size_type size) {
  container.reserve(size);
}

// チャンクを考慮して、インサーションソートを行う関数
template <class Container>
void PmergeMe<Container>::ChunkedInsertSort(size_t chunk_size,
                                            Container &container) {
  Container insert_container;
  Container inserted_container;
  size_t insert_container_size = 0;

  reserve_if_possible(inserted_container, container.size());
  SplitContainer(chunk_size, container, insert_container, inserted_container,
                 insert_container_size);

  Debug::cout() << "insert_container   : ";
  DebugContainer(insert_container, chunk_size);
  Debug::cout() << "inserted_container : ";
  DebugContainer(inserted_container, chunk_size);
  Debug::cout() << "\n";

  InsertIntegrate(chunk_size, container, insert_container, inserted_container,
                  insert_container_size);
}

// コンテナを挿入するコンテナと挿入されるコンテナに分割する関数
template <class Container>
void PmergeMe<Container>::SplitContainer(size_t chunk_size,
                                         Container &container,
                                         Container &insert_container,
                                         Container &inserted_container,
                                         size_t &insert_container_size) {
  Iterator begin = container.begin();
  Iterator end = Advance(begin, chunk_size);
  size_t container_size = container.size();

  // コンテナを分割する
  for (size_t i = 0; i * chunk_size < container_size; ++i) {
    if (i % 2 == 0) {
      if (end == container.end() && chunk_size != 1)
        inserted_container.insert(inserted_container.end(), begin, end);
      else {
        insert_container.insert(insert_container.end(), begin, end);
        ++insert_container_size;
      }
    } else {
      inserted_container.insert(inserted_container.end(), begin, end);
    }
    if (end == container.end()) break;
    begin = end;

    if (container_size < (i + 2) * chunk_size) {
      end = container.end();
    } else {
      std::advance(end, chunk_size);
    }
  }
}

// 分割したコンテナを結合していく
// insert_containerをinserted_containerに挿入していく
// その時ヤコブスタール数列とチャンクを考慮して挿入していく
template <class Container>
void PmergeMe<Container>::InsertIntegrate(size_t chunk_size,
                                          Container &container,
                                          Container &insert_container,
                                          Container &inserted_container,
                                          size_t insert_container_size) {
  for (size_t i = 0; i < insert_container_size; ++i) {
    size_t jaco = CalcJacob(i, insert_container_size - 1);
    Iterator insert_it = Advance(insert_container.begin(), jaco * chunk_size);
    if (jaco == 0) {
      inserted_container.insert(inserted_container.begin(), insert_it,
                                Advance(insert_it, chunk_size));
      DebugContainer(inserted_container, chunk_size, inserted_container.begin(),
                     Advance(inserted_container.begin(), chunk_size));
    } else {
      Iterator inserted_it =
          Advance(inserted_container.begin(), jaco * chunk_size);
      Iterator it =
          LowerBound(Advance(inserted_container.begin(), chunk_size - 1),
                     Advance(inserted_it, chunk_size - 1 + chunk_size * i),
                     Advance(insert_it, chunk_size - 1), chunk_size);
      inserted_container.insert(Advance(it, 1 - chunk_size), insert_it,
                                Advance(insert_it, chunk_size));
      DebugContainer(inserted_container, chunk_size,
                     Advance(it, 1 - chunk_size),
                     Advance(inserted_it, chunk_size * i));
    }
  }
  container = inserted_container;
}

// utils系 ------------------------------------------------------------------ //

// ダブルポインタからコンテナに変換する関数
template <class Container>
Container PmergeMe<Container>::ArgvToIntContainer(char *argv[]) {
  Container container;
  for (int i = 1; argv[i] != NULL; ++i) container.push_back(std::atoi(argv[i]));
  return container;
}

// n番がヤコブスタール数列の何番目かを計算する
template <class Container>
size_t PmergeMe<Container>::CalcJacob(size_t n, size_t max_size) {
  size_t i = 0;
  size_t jacob_nbr = 0;
  size_t jacob_progressive_nbr = 0;
  size_t diff;

  if (n == 0) return 0;
  // ヤコブの合計値がnを超えてないか確認
  while (jacob_progressive_nbr + jacob_nbr < n) {
    jacob_progressive_nbr += jacob_nbr;
    jacob_nbr = Jacobsthal(i) * 2;
    ++i;
  }
  // max_sizeに合わせて変更を行う
  if (jacob_progressive_nbr + jacob_nbr > max_size)
    jacob_nbr = max_size - jacob_progressive_nbr;
  diff = n - jacob_progressive_nbr;
  return ++jacob_progressive_nbr + jacob_nbr - diff;
}

// https://en.cppreference.com/w/cpp/algorithm/lower_bound の実装を参考に
// chunk_sizeを考慮してlower_bound(二分探索)を行う関数
template <class Container>
typename PmergeMe<Container>::Iterator PmergeMe<Container>::LowerBound(
    Iterator first, Iterator last, const Iterator value, size_t chunk_size) {
  Iterator it;
  size_t count, step;
  count = std::distance(first, last) / chunk_size;

  while (0 < count) {
    it = first;
    step = count / 2;
    std::advance(it, step * chunk_size);
    Debug::cout() << "比較  :" << std::setw(3) << *value << ":" << std::setw(3)
                  << *it << "\n";
    if (*value > *it) {
      first = Advance(it, chunk_size);
      count -= step + 1;
    } else {
      count = step;
    }
  }
  return first;
}

// std::advanceが参照ではなくiteratorを返すように変更した関数
template <class Container>
typename PmergeMe<Container>::Iterator PmergeMe<Container>::Advance(
    Iterator it, long long int count) {
  std::advance(it, count);
  return it;
}

// コンテナのデバック用
template <class Container>
void PmergeMe<Container>::DebugContainer(Container &container, size_t size) {
  Debug::cout() << GRAY << "debug : [";
  size_t count = 0;
  for (Iterator it = container.begin(); it != container.end(); ++it) {
    if (count % size == 0 && count != 0)
      Debug::cout() << "]" << GREEN << "." << GRAY << "[";
    else if (count != 0)
      Debug::cout() << " ";
    Debug::cout() << DEFAULT << *it << GRAY;
    count++;
  }
  Debug::cout() << "]\n" << DEFAULT;
}

template <class Container>
void PmergeMe<Container>::DebugContainer(Container &container, size_t size,
                                         Iterator it1, Iterator it2) {
  size_t count = 0;
  if (it1 == it2) std::advance(it2, -size);

  Debug::cout() << GRAY << "debug : [";
  for (Iterator it = container.begin(); it != container.end(); ++it) {
    if (count % size == 0 && count != 0)
      Debug::cout() << GRAY << "]" << GREEN << "." << GRAY << "[";
    else if (count != 0)
      Debug::cout() << " ";
    if (it == it1)
      Debug::cout() << YELLOW << *it;
    else if (it == it2)
      Debug::cout() << ORANGE << *it;
    else
      Debug::cout() << *it;
    count++;
  }
  Debug::cout() << GRAY << "]\n" << DEFAULT;
}