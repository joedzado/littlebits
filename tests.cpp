#include "littlebits.h"
#include <cassert>
#include <chrono>
#include <format>
#include <future>
#include <print>
#include <random>

using namespace std::chrono_literals;
CONFIGS(MyConfigs, One, Two, Three, Four)
CONFIGS(MoreConfigs, One, Two, Three, Four, Five, Six, Seven)

void booltest() {
  std::println("Boolean test.");
  assert(MyConfigs::size == 4);
  assert(MoreConfigs::size == 7);

  littlebits<MyConfigs, bool> configs;
  assert(!configs.get<MyConfigs::One>());
  assert(!configs.get<MyConfigs::Two>());
  assert(!configs.get<MyConfigs::Three>());
  assert(!configs.get<MyConfigs::Four>());

  littlebits<MoreConfigs, bool> moreConfigs({true, false, true, false, true, false, true});
  assert(moreConfigs.get<MoreConfigs::One>());
  assert(!moreConfigs.get<MoreConfigs::Two>());
  assert(moreConfigs.get<MoreConfigs::Three>());
  assert(!moreConfigs.get<MoreConfigs::Four>());
  assert(moreConfigs.get<MoreConfigs::Five>());
  assert(!moreConfigs.get<MoreConfigs::Six>());
  assert(moreConfigs.get<MoreConfigs::Seven>());

  auto &atom = moreConfigs.atomic<MoreConfigs::One>();

  auto a2 = std::async(std::launch::async, [&atom]() {
    std::this_thread::sleep_for(250ms);
    atom.store(false);
    atom.notify_all();
  });
  std::atomic<bool> expected = true;
  atom.wait(true);

  // Done waiting, it's false.
  assert(false == moreConfigs.get<MoreConfigs::One>());

  auto job = [&atom]() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(1, 2);
    auto duration = std::chrono::milliseconds(dis(gen));
    std::this_thread::sleep_for(duration);
    bool expected = false;
    while (!atom.compare_exchange_strong(expected, true)) {
      expected = false;
    }
    std::println("Thread {} set the bool.", duration);
    atom.store(false);
  };
  std::thread t1(job);
  std::thread t2(job);
  t1.join();
  t2.join();
}
void inttest() {
  std::println("Integer test.");
  littlebits<MyConfigs, int> configs;
  assert(configs.get<MyConfigs::One>() == 0);
  assert(configs.get<MyConfigs::Two>() == 0);
  assert(configs.get<MyConfigs::Three>() == 0);

  littlebits<MyConfigs, int> moreConfigs({1, 2, 3, 4});
  assert(moreConfigs.get<MyConfigs::One>() == 1);
  assert(moreConfigs.get<MyConfigs::Two>() == 2);
  assert(moreConfigs.get<MyConfigs::Three>() == 3);
  assert(moreConfigs.get<MyConfigs::Four>() == 4);
}

int main() {
  booltest();
  inttest();
  return 0;
}