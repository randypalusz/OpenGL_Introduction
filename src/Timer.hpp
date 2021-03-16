#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

template <typename T>
class Timer {
 public:
  Timer(const T& tick) { currentTick = tick; }
  const T tick(const T& tick, bool updateLastTickOnCall = true) {
    if (updateLastTickOnCall) {
      lastTick = currentTick;
    }
    currentTick = tick;
    deltaTime = currentTick - lastTick;
    return deltaTime;
  }
  T deltaTime;

 private:
  T currentTick;
  T lastTick;
};

class TimePointTimer {
 public:
  TimePointTimer() { currentTick = std::chrono::high_resolution_clock::now(); }
  const size_t tick(bool updateLastTickOnCall = true) {
    if (updateLastTickOnCall) {
      lastTick = currentTick;
    }
    currentTick = std::chrono::high_resolution_clock::now();
    deltaTime =
        std::chrono::duration_cast<std::chrono::microseconds>(currentTick - lastTick)
            .count();
    return deltaTime;
  }
  size_t deltaTime = 0;

 private:
  std::chrono::high_resolution_clock::time_point currentTick;
  std::chrono::high_resolution_clock::time_point lastTick;
};

#endif