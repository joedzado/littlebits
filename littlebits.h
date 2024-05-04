#pragma once
#define COUNT_ARGS(...) (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
#define CONFIGS(name, ...)                                                     \
  struct name {                                                                \
    enum _configs { __VA_ARGS__ };                                             \
    inline const static size_t size = COUNT_ARGS(__VA_ARGS__);                 \
  };