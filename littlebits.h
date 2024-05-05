#pragma once
#include <array>
#include <atomic>
#include <print>
#include <ranges>
#include <type_traits>
namespace {
struct Base {};
} // namespace
#define COUNT_ARGS(...) (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
#define CONFIGS(name, ...)                                                                         \
  struct name final : public Base {                                                                \
    enum _configs { __VA_ARGS__ };                                                                 \
    inline const static size_t size = COUNT_ARGS(__VA_ARGS__);                                     \
  };

template <typename EnumProvider, typename Type,
          typename = std::enable_if_t<std::is_base_of_v<Base, EnumProvider>>>
class littlebits {
public:
  littlebits() : configs{} {}
  explicit littlebits(std::array<Type, EnumProvider::size> values) {
    std::ranges::copy(values, configs.begin());
  }
  template <int Config> Type get() { return configs[Config].load(); }
  template <int Config> auto &atomic() { return configs[Config]; }
  template <int Config> auto set(Type &&value) { configs[Config].store(std::forward<Type>(value)); }

private:
  std::array<std::atomic<Type>, EnumProvider::size> configs;
};