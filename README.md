# TastyHedge

This repository contains the accompanying code for the [tastyhedge.com](https://tastyhedge.com)
blog.

## Build

### Windows

In Visual Studio open as a CMake project. Then use as a regular C++ project.

### Linux

Tested on Ubuntu 22.04.3 LTS (WSL) with GCC 11:

- **Config**

  ```
  cmake --preset linux-release
  ```

- **Build**

  ```
  cmake --build --preset linux-release --target tastyhedge
  ```

- **Install & Run**

  ```
  cp ./out/linux-release/src/tastyhedge .
  ```

  ```
  ./tastyhedge
  ```

For more available options see `Makefile`.
