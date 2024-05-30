#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>
#include <iostream>

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

#endif  // STOPWATCH_HPP