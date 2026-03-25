# 1 "counters.cppm"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 489 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "counters.cppm" 2
export module counters;
import "/usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/cinttypes";
import "/usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/functional";
import "/usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/memory";
import "/usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/utility";
export import "/usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/unordered_map";
export class MCounter;
export class FCounter;
export class GCounter;
export class CounterSet;

class Prescaler {
  uint64_t p;
  uint64_t r;
  uint64_t prev_r, sum_w;

public:
  Prescaler(uint64_t p) noexcept : p(p), r(0), prev_r(0), sum_w(0) {}



  uint64_t advance(uint64_t t) noexcept {


    uint64_t w;
    prev_r = r;
    sum_w = 0;
    if (t <= r) {
      r -= t;
      w = 0;
    }
    else {
      uint64_t s = t - r - 1;
      if (p < (18446744073709551615UL)) {
        uint64_t m = p + 1;
        r = p - s % m;
        w = 1 + s / m;
      }
      else {
        r = p - s;
        w = 1;
      }
    }
    return w;
  }



  uint64_t next(uint64_t w) noexcept {
    sum_w += w;

    return (sum_w - 1) * (p + 1) + prev_r + 1;
  }
};

class Counter {
protected:
  Prescaler prescaler;
  uint64_t counter;

public:
  Counter(uint64_t presc) noexcept : prescaler(presc), counter(0) {};

  virtual ~Counter() noexcept {};

  uint64_t value() const noexcept {
    return counter;
  }



  virtual void advance(uint64_t, std::function<void(uint64_t)>) = 0;
};

class VMCounter : public Counter {
protected:
  uint64_t mod;

  virtual void change_mod() noexcept = 0;

public:
  VMCounter(uint64_t presc, uint64_t mod) noexcept : Counter(presc), mod(mod) {};

  void advance(uint64_t impulses, std::function<void(uint64_t)> add_event) override final {
    uint64_t t = prescaler.advance(impulses);
    uint64_t s;
    while (t > (s = mod - counter)) {
      t -= s + 1;
      add_event(prescaler.next(s + 1));
      change_mod();
      counter = 0;
    }
    counter += t;
  }
};

class MCounter : public VMCounter {
  void change_mod() noexcept override {};

public:
  MCounter(uint64_t presc, uint64_t mod) noexcept : VMCounter(presc, mod) {};
};

class GCounter : public VMCounter {
  void change_mod() noexcept override {
    mod = mod == 999999999999 ? 9 : 10 * mod + 9;
  };

public:
  GCounter(uint64_t presc) noexcept : VMCounter(presc, 9) {};
};

class FCounter : public Counter {
  uint64_t f0, f1;
  bool flag;

public:
  FCounter(uint64_t presc) noexcept : Counter(presc), f0(0), f1(1), flag(true) {};

  void advance(uint64_t impulses, std::function<void(uint64_t)> add_event) override final {
    if (counter < (18446744073709551615UL)) {
      uint64_t t = prescaler.advance(impulses);
      while (flag && t >= f1 - counter) {
        t -= f1 - counter;
        add_event(prescaler.next(f1 - counter));
        counter = f1;
        if (f1 <= (18446744073709551615UL) - f0) {
          uint64_t f2 = f1 + f0;
          f0 = f1;
          f1 = f2;
        }
        else {
          flag = false;
        }
      }
      if (counter < (18446744073709551615UL) - t)
        counter += t;
      else
        counter = (18446744073709551615UL);
    }
  }
};

class CounterSet {
  std::unordered_map<uint64_t, std::unique_ptr<Counter>> counter_map;

public:
  template <typename T, typename... P>
  void addCounter(uint64_t counter, P... params) {




    if (!counter_map.insert(std::make_pair(counter, std::make_unique<T>(params...))).second)
      throw std::invalid_argument("");
  }

  void delCounter(uint64_t counter) {
    auto it = counter_map.find(counter);
    if (it == counter_map.end())
      throw std::invalid_argument("");
    counter_map.erase(it);
  }

  uint64_t getCounter(uint64_t counter) const {
    auto it = counter_map.find(counter);
    if (it == counter_map.end())
      throw std::invalid_argument("");
    return it->second->value();
  }


  void advanceCounters(uint64_t pulses, std::function<void(uint64_t, uint64_t)> add_event) const {
    for (auto const &counter : counter_map)
      counter.second->advance(pulses, [&](uint64_t t) {add_event(counter.first, t);});
  }
};
