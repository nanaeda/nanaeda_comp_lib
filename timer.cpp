#include <chrono>

class Timer {
private:
  std::chrono::system_clock::time_point begin_time;

public:
  Timer()
  {
    reset();
  }

  void reset()
  {
    begin_time = std::chrono::system_clock::now();
  }

  double get_elapsed() 
  {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    int64_t dur = std::chrono::duration_cast<std::chrono::nanoseconds>(now - begin_time).count();
    return dur * 1e-9;
  }
};
