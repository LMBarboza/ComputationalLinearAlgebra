#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
  std::chrono::high_resolution_clock::time_point startTime;
  std::chrono::high_resolution_clock::time_point endTime;

public:
  void start(void);
  void end(void);
  void reset(void);
  long long getTime(void);
};

#endif
