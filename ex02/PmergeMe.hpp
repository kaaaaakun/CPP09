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
  typedef typename Container::iterator Iterator;

  PmergeMe() {}
  ~PmergeMe() {}
  static void RecursiveMergeInsertionSort(size_t chunk_size,
                                          Container &container);
  static void ChunkedMergeSort(size_t chunk_size, Container &container);
  static void ChunkedInsertSort(size_t chunk_size, Container &container);
  static Container ArgvToIntContainer(char *argv[]);
  static Iterator Advance(Iterator it, long long int count);
  static Iterator LowerBound(Iterator first, Iterator last,
                             const Iterator value, size_t chunk_size);
  static void Debug(Container &container, size_t size);

  static size_t CalcJacob(size_t n, size_t max_size);
  static void InsertBack(Container &container, const Iterator &reverse_end_it,
                         const Iterator &reverse_first_it, size_t chunk_size);

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
  Iterator it = container.begin();
  std::cout << "-- chunk_size : " << chunk_size << " -- " << std::endl;
  for (size_t i = 0; i + chunk_size * 2 <= container_size;
       i += chunk_size * 2) {
    Iterator it0 = Advance(it, i);
    Iterator it1 = Advance(it, i + chunk_size - 1);
    Iterator it2 = Advance(it, i + chunk_size * 2 - 1);
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
typename PmergeMe<Container>::Iterator PmergeMe<Container>::LowerBound(
    Iterator first, Iterator last, const Iterator value, size_t chunk_size) {
  Iterator it;
  size_t count, step;
  count = std::distance(first, last) / chunk_size;

  std::cout << count << " : count\n";

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

// 何個目に比較されるのかを計算(累進でヤコブ計算)
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

// ヤコブスタール数の後ろから
template <class Container>
void PmergeMe<Container>::InsertBack(Container &container,
                                     const Iterator &reverse_end_it,
                                     const Iterator &reverse_first_it,
                                     size_t chunk_size) {
  Iterator begin = container.begin();

  for (Iterator it = reverse_first_it; it != reverse_end_it;
       std::advance(it, -chunk_size * 2)) {
    Iterator compared_it_end =
        LowerBound(Advance(begin, chunk_size - 1), reverse_first_it,
                   reverse_first_it, chunk_size);
    std::cout << "ここ reverse_first_it: " << *reverse_first_it
              << ", com: " << *compared_it_end << std::endl;
    if (reverse_first_it != compared_it_end)
      std::rotate(Advance(compared_it_end, 1 - chunk_size),
                  Advance(reverse_first_it, 1 - chunk_size),
                  Advance(reverse_first_it, 1));
  }
}

// 　チャンクを考慮して、インサーションソートを行う
template <class Container>
void PmergeMe<Container>::ChunkedInsertSort(size_t chunk_size,
                                            Container &container) {
  Container insert_container;
  Container inserted_container;
  Iterator begin = container.begin();
  Iterator end = Advance(begin, chunk_size);
  size_t container_size = container.size();
  size_t insert_container_size = 0;

// コンテナを分割する
  for (size_t i = 0; i * chunk_size < container_size; ++i) {
    if (i % 2 == 0) {
      insert_container.insert(insert_container.end(), begin, end);
      ++insert_container_size;
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

  //  // コンテナを分割する
  //  for (size_t i = 0; i + chunk_size*2 < container_size; i += chunk_size*2) {
  //    insert_container.insert(insert_container.end(), Advance(it, i),
  //                            Advance(it, i + chunk_size));
  //    inserted_container.insert(inserted_container.end(), Advance(it, i +
  //    chunk_size),
  //                              Advance(it, i + chunk_size * 2));
  //    ++insert_container_size;
  //  }
  //  if (container_size % (chunk_size * 2) != 0) {
  //    // 余りがある場合
  //    // 余りがchunk_size以下の場合
  //    if (container_size % (chunk_size * 2) <= chunk_size) {
  //      inserted_container.insert(insert_container.end(),
  //      Advance(it, insert_container_size * chunk_size * 2),
  //                              container.end());
  //    } else { // 余りがchunk_sizeより大きい場合
  //      insert_container.insert(insert_container.end(),
  //      Advance(it,insert_container_size * chunk_size * 2),
  //                              Advance(it, insert_container_size * chunk_size
  //                              * 2 + chunk_size));
  //      inserted_container.insert(inserted_container.end(),
  //      Advance(it, insert_container_size * chunk_size * 2 + chunk_size),
  //                              container.end());
  //                              ++ insert_container_size;
  //    }
  //  }
  //
  std::cout << insert_container_size << " : insert_container_size\n";
  std::cout << "insert_container   : ";
  Debug(insert_container, chunk_size);
  std::cout << "inserted_container : ";
  Debug(inserted_container, chunk_size);

  // 　比較対象
  for (size_t i = 0; i < insert_container_size; ++i) {
    size_t jaco = CalcJacob(i, insert_container_size - 1);
    std::cout << "test : " << i << " : " << jaco << " ";
    Debug(inserted_container, chunk_size);
    Iterator insert_it = Advance(insert_container.begin(), jaco * chunk_size);
    Iterator inserted_it =
        Advance(inserted_container.begin(), jaco * chunk_size);
    if (jaco == 0) {
      inserted_container.insert(inserted_container.begin(), insert_it,
                                Advance(insert_it, chunk_size));
    } else {
      Iterator it =
          LowerBound(Advance(inserted_container.begin(), chunk_size - 1),
                     Advance(inserted_it, chunk_size - 1 + chunk_size * i),
                     Advance(insert_it, chunk_size - 1), chunk_size);
      std::cout << "--\n";
      std::cout << "it : " << *it << " : \n";
      std::cout << "Advance(inserted_container.begin(), chunk_size - 1) : "
                << *Advance(inserted_container.begin(), chunk_size - 1)
                << " : \n";
      std::cout << "Advance(inserted_it, chunk_size - 1) : "
                << *Advance(inserted_it, chunk_size - 1) << " : \n";
      std::cout << "Advance(insert_it, chunk_size - 1) : "
                << *Advance(insert_it, chunk_size - 1) << " : ";
      std::cout << std::endl;
      std::cout << "--\n";
      inserted_container.insert(Advance(it, 1 - chunk_size), insert_it,
                                Advance(insert_it, chunk_size));
    }
  }
  container = inserted_container;
  std::cout << "inserted_container : ";
  Debug(inserted_container, chunk_size);
  std::cout << "--------------------------\n\n";

  // // 必要なヤコブスタール数の配列をgetする
  // // ヤコブスタールに則ったコンテナを作成する
  // // インサートソートを作成する
  // std::cout << "-- chunk_size : " << chunk_size << " -- " << std::endl;
  // size_t container_size = container.size();
  // size_t elem_num = container_size / chunk_size * 2;
  // Iterator jacob_container;
  // for (size_t i = 0; i < elem_num; ++i) {
  //   jacob_container.push_back(Jacobsthal(i));
  // }
  // size_t jacob_nbr;
  // size_t jacob_progressive_nbr = 0;
  // size_t reverse_first_point = 0;
  // Iterator begin = container.begin();
  // Iterator reverse_first_it;
  // Iterator reverse_end_it = Advance(begin, chunk_size - 1);

  // for (size_t i = 1;; ++i) {
  //   // ヤコブスタール数は２倍の値かつ累進にする
  //   jacob_nbr = Jacobsthal(i);
  //   jacob_progressive_nbr += jacob_nbr;
  //   if (jacob_nbr == 0) continue;
  //   reverse_first_point =
  //       jacob_progressive_nbr * chunk_size * 2 + chunk_size - 1;
  //   // 範囲内にあるか確かめる(なかったら、納める)
  //   while (container_size < reverse_first_point)
  //     reverse_first_it -= chunk_size * 2;
  //   // 挿入する側のコンテナを別のコンテナに移動させる。
  //   // TODO : ここで、コンテナを移動させる
  //   reverse_first_it = Advance(begin, reverse_first_point);
  //   InsertBack(container, reverse_end_it, reverse_first_it, chunk_size);
  //   Debug(container, chunk_size);
  //   reverse_end_it = reverse_first_it;
  // }
}

// std::advance でイテレーターを返すようにを使いやすく
template <class Container>
typename PmergeMe<Container>::Iterator PmergeMe<Container>::Advance(
    Iterator it, long long int count) {
  std::advance(it, count);
  return it;
}

// デバック用
template <class Container>
void PmergeMe<Container>::Debug(Container &container, size_t size) {
  std::cout << "debug : ";
  size_t count = 0;
  for (Iterator it = container.begin(); it != container.end(); ++it) {
    if (count % size == 0 && count != 0)
      std::cout << "\033[0m"
                << "\033[32m"
                << ", "
                << "\033[m";
    else
      std::cout << " ";
    std::cout << "\033[4m" << *it;
    count++;
  }
  std::cout << "\033[0m" << std::endl;
}
