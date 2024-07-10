#include "../include/timer.h"
#include <chrono>

void Timer::start(void) {
  startTime = std::chrono::high_resolution_clock::now();
}

void Timer::end(void) { endTime = std::chrono::high_resolution_clock::now(); }

void Timer::reset(void) {
  startTime = std::chrono::high_resolution_clock::time_point();
  endTime = std::chrono::high_resolution_clock::time_point();
}

long long Timer::getTime(void) {
  std::chrono::milliseconds duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                            startTime);
  return duration.count();
}
