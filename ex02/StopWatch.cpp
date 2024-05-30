#include "StopWatch.hpp"

void StopWatch::Start() { start_ = std::clock(); }
void StopWatch::Stop() {
  stop_ = std::clock();
  std::cout << static_cast<double>(stop_ - start_) / CLOCKS_PER_SEC * 1000.0 *
                   1000.0
            << " us" << std::endl;
}
StopWatch &StopWatch::operator=(const StopWatch &obj) {
  start_ = obj.start_;
  stop_ = obj.stop_;
  return *this;
}
