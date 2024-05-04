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
CONFIGS(EvenMoreConfigs, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten)

int main() {
  assert(MyConfigs::size == 4);
  std::println("Number of configs: {}", MyConfigs::size);
  assert(MoreConfigs::size == 7);
  std::println("Number of configs: {}", MoreConfigs::size);
  assert(EvenMoreConfigs::size == 10);
  std::println("Number of configs: {}", EvenMoreConfigs::size);

  littlebits<MyConfigs> configs;
  assert(!configs.get<MyConfigs::One>());
  assert(!configs.get<MyConfigs::Two>());
  assert(!configs.get<MyConfigs::Three>());
  assert(!configs.get<MyConfigs::Four>());

  littlebits<MoreConfigs> moreConfigs({true, false, true, false, true, false, true});
  assert(moreConfigs.get<MoreConfigs::One>());
  assert(!moreConfigs.get<MoreConfigs::Two>());
  assert(moreConfigs.get<MoreConfigs::Three>());
  assert(!moreConfigs.get<MoreConfigs::Four>());
  assert(moreConfigs.get<MoreConfigs::Five>());
  assert(!moreConfigs.get<MoreConfigs::Six>());
  assert(moreConfigs.get<MoreConfigs::Seven>());

  auto &atom = moreConfigs.atomic<MoreConfigs::One>();
  auto time = std::chrono::system_clock::now();
  auto a2 = std::async(std::launch::async, [&atom, &time]() {
    std::this_thread::sleep_for(250ms);
    atom.store(false);
    time = std::chrono::system_clock::now();
    atom.notify_all();
  });
  std::atomic<bool> expected = true;
  atom.wait(true);

  auto delta = std::chrono::system_clock::now() - time;
  std::println("After thread time: {}", delta);

  std::println("Thread complete.");

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
  return 0;
}