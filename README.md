# LittleBits Project

## Overview

The LittleBits project provides a lightweight and efficient C++ library designed for managing configuration settings. It leverages modern C++ features, such as templates, `std::array`, and `std::atomic`, to ensure type safety, thread safety, and ease of use. The library is particularly suited for applications that require dynamic configuration management with minimal overhead.

## Features

- **Type Safety**: Utilizes templates to ensure that configuration values are of the correct type.
- **Thread Safety**: Uses `std::atomic` for thread-safe read and write operations on configuration values.
- **Efficiency**: Designed with performance in mind, making minimal use of resources.
- **Ease of Use**: Provides a simple and intuitive API for managing configuration settings.

## Requirements

- C++17 or higher
- A modern C++ compiler (GCC, Clang, MSVC)

## Installation

To include LittleBits in your project, simply add the `littlebits.h` header file to your project's include path. There is no need to compile any source files or link against any libraries, as LittleBits is a header-only library.

## Usage
Usage
Defining Configurations
```cpp
#include "littlebits.h"

CONFIGS(MyConfig,
    Config1, // First configuration label
    Config2  // Second configuration label
);
```
Configurations are defined using the CONFIGS macro. Under the hood this creates a an enum whose size we store. You can then use your configs to initialize an instance of littlebits to keep track of your preferred type of configs.

### Initializing the Configuration Manager
littlebits is templated and requires two template parameters: the enum provider (the struct defined using CONFIGS) and the type of the configuration values (e.g. bool, int, etc).
```cpp
// Will have an int for each item in "MyConfigs"
littlebits<MyConfig, int> configs;
```

### Setting and Getting Configuration Values
Configuration values can be set and retrieved using the set and get member functions, respectively. These functions are templated on the configuration enum value.
```cpp
// Setting a configuration value
configs.set<MyConfig::Config1>(42);

// Getting a configuration value
int value = configs.get<MyConfig::Config1>();
```

### Atomic Operations
For multi-threaded environments, the atomic member function provides access to the underlying std::atomic instance for a configuration, allowing for atomic operations.
```cpp
auto& atomicConfig = configs.atomic<MyConfig::Config1>();
atomicConfig.store(24);
```

Or use the atomic wait function to monitor when the config changes.
```cpp
auto oldValue = 50;
atomicConfig.wait(oldvalue);
```

Or use the atomic compare and exchange function to know if you were the one to change the value
```cpp
CONFIGS(Gates, Read, Write, Delete)

littlebits<Gates, bool> syncronizers;

// ... 
bool expected = false;
bool desired = true;
auto& atomic = syncronizers.atomic<Gates::Write>();
while (!atomic.compare_exchange_strong(expected, desired)) {
    expected = false;
}
// We set the flag to true, do our work and put the flag back.
syncronizers.set<Gates::Write>(false);
```

## Example
```cpp
#include "littlebits.h"

// Define configurations
CONFIGS(AppConfig, LogLevel, MaxConnections);

int main() {
    // Initialize configuration manager with default values
    littlebits<AppConfig, int> config({3, 100});

    // Set configuration values
    config.set<AppConfig::LogLevel>(2);
    config.set<AppConfig::MaxConnections>(150);

    // Retrieve and print configuration values
    std::cout << "LogLevel: " << config.get<AppConfig::LogLevel>() << std::endl;
    std::cout << "MaxConnections: " << config.get<AppConfig::MaxConnections>() << std::endl;

    return 0;
}
```
## License
Specify the license under which the LittleBits library is distributed. This section is typically found in a LICENSE file in the root of the project.