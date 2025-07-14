constexpr unsigned short MAX_LEVEL = 4;
const int LEVEL_MAP[MAX_LEVEL+1] = {0, 1, 2, 3, 4};

struct Assistance {
  
  unsigned short level = 0;
  bool changed = false;

  void loop() {
    changed = false;
  }

  void increment() {
    if (level == MAX_LEVEL) return;

    ++level;
    changed = true;
  }

  void decrement() {
    if (level == 0) return;

    --level;
    changed = true;
  }

  int map(int toLow, int toHigh) {
    return ::map(LEVEL_MAP[level], LEVEL_MAP[0], LEVEL_MAP[MAX_LEVEL], toLow, toHigh);
  }
};