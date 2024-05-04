#include "littlebits.h"
#include <print>
CONFIGS(MyConfigs, One, Two, Three, Four)
CONFIGS(MoreConfigs, One, Two, Three, Four, Five, Six, Seven)
CONFIGS(EvenMoreConfigs, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten)

int main() {
  std::println("Number of configs: {}", MyConfigs::size);
  std::println("Number of configs: {}", MoreConfigs::size);
  std::println("Number of configs: {}", EvenMoreConfigs::size);
  return 0;
}