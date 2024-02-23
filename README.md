Please, star this repo if you find it useful, so that other people can discover it as well.

# TastyHedge

[tastyhedge.com](https://tastyhedge.com) is a blog about programming and quantitative finance. This
repository contains accompanying C++ code and market data to reproduce the examples discussed in the
blog.

Additionally, you can build and run `testyhedge` program that has the following interface:

```
tastyhedge - Financial Analytics

Usage:
    tastyhedge calibrate [--mds=<dir>] <src> <dst>

Global Options:
    --mds <dir>         Market Data Store [default: c:/mds]

    --version           Print version
```

## Build

To manage C++ dependencies we use [vcpkg](https://vcpkg.io/). Please,
[install vcpkg](https://vcpkg.io/en/getting-started) on Linux or Windows (or use default version
that comes with Visual Studio).

### Windows

In Visual Studio open and use as a
[CMake project](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio).

### Linux

Tested on Ubuntu 22.04.3 LTS (WSL) with GCC 11 (see `Makefile` for more options):

- **Config**

  ```
  cmake --preset linux-release
  ```

- **Build**

  ```
  cmake --build --preset linux-release --target tastyhedge
  ```

- **Install**

  ```
  cp ./out/linux-release/src/tastyhedge .
  ```

- **Run**

  ```
  ./tastyhedge
  ```
