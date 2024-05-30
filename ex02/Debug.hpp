#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <string>

class Debug {
 public:
  // テンプレートクラスとして実装できないので、クラス内部で実装
  template <class T>
  Debug &operator<<(T t) {
    std::cout << t;
    return *this;
  }
  static Debug &cout() {
    static Debug instance;
    return instance;
  }

 private:
  Debug() {}
  ~Debug() {}
};

#endif  // DEBUG_HPP