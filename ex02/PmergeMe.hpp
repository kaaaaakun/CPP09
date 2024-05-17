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
  ~StopWatch() {}
  void Start();
  void Stop();

 private:
  std::clock_t start_;
  std::clock_t stop_;

  StopWatch(const StopWatch &obj) : start_(obj.start_), stop_(obj.stop_) {}
  StopWatch &operator=(const StopWatch &obj);
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
  static typename Container::iterator LowerBound(
      typename Container::iterator first, typename Container::iterator last,
      const typename Container::iterator value, size_t chunk_size);
  static void Debug(Container &container, size_t size);

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

// 再帰でソートする
template <class Container>
void PmergeMe<Container>::RecursiveMergeInsertionSort(size_t chunk_size,
                                                      Container &container) {
  size_t container_size = container.size();
  ChunkedMergeSort(chunk_size, container);
  Debug(container, chunk_size);
  if (chunk_size * 4 <= container_size)
    RecursiveMergeInsertionSort(chunk_size * 2, container);
  else
    return;
  ChunkedInsertSort(chunk_size, container);
  Debug(container, chunk_size);
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
  std::cout << "-- chunk_size : " << chunk_size << " -- " << std::endl;
  for (size_t i = 0; i + chunk_size * 2 <= container_size;
       i += chunk_size * 2) {
    typename Container::iterator it0 = Advance(it, i);
    typename Container::iterator it1 = Advance(it, i + chunk_size - 1);
    typename Container::iterator it2 = Advance(it, i + chunk_size * 2 - 1);
    std::cout << "比較 : " << *it1 << " : " << *it2 << std::endl;
    if (*it2 < *it1) {
      std::advance(it1, 1);
      std::advance(it2, 1);
      std::rotate(it0, it1, it2);
    }
  }
}

// https://en.cppreference.com/w/cpp/algorithm/lower_bound の実装を参考に
template <class Container>
typename Container::iterator PmergeMe<Container>::LowerBound(
    typename Container::iterator first, typename Container::iterator last,
    const typename Container::iterator value, size_t chunk_size) {
  typename Container::iterator it;
  size_t count, step;
  count = std::distance(first, last) / chunk_size;

  while (0 < count) {
    it = first;
    step = count / 2;
    std::advance(it, step * chunk_size);
    std::cout << "比較 : " << *value << " : " << *it << std::endl;
    if (*value > *it) {
      first = Advance(it, chunk_size);
      count -= step + 1;
    } else {
      count = step;
    }
  }
  return first;
}

// 　チャンクを考慮して、インサーションソートを行う
template <class Container>
void PmergeMe<Container>::ChunkedInsertSort(size_t chunk_size,
                                            Container &container) {
  size_t container_size = container.size();
  typename Container::iterator begin = container.begin();
  std::cout << "-- chunk_size : " << chunk_size << " -- " << std::endl;
  for (size_t i = chunk_size * 3 - 1; i < container_size; i += chunk_size * 2) {
    typename Container::iterator inserted_it_end = Advance(begin, i);
    typename Container::iterator compared_it_end =
        LowerBound(Advance(begin, chunk_size - 1), inserted_it_end,
                   inserted_it_end, chunk_size);
    std::cout << "ここ inserted_it_end: " << *inserted_it_end
              << ", com: " << *compared_it_end << std::endl;
    if (inserted_it_end != compared_it_end)
      std::rotate(Advance(compared_it_end, 1-chunk_size),
                  Advance(inserted_it_end, 1 - chunk_size),
                  Advance(inserted_it_end, 1));
    Debug(container, chunk_size);
  }
}

// std::advance でイテレーターを返すようにを使いやすく
template <class Container>
typename Container::iterator PmergeMe<Container>::Advance(
    typename Container::iterator it, long long int count) {
  std::advance(it, count);
  return it;
}

// デバック用
template <class Container>
void PmergeMe<Container>::Debug(Container &container, size_t size) {
  std::cout << "debug : ";
  size_t count = 1;
  for (typename Container::iterator it = container.begin();
       it != container.end(); ++it) {
    std::cout << *it << " ";
    if (count % size == 0) std::cout << ",";
    count++;
  }
  std::cout << std::endl;
}
