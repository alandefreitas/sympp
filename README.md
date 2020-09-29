# Sym<sup>pp</sup>

[![Build Status](https://img.shields.io/github/workflow/status/alandefreitas/sympp/SymPP?event=push&label=Build&logo=Github-Actions)](https://github.com/alandefreitas/sympp/actions?query=workflow%3ASymPP+event%3Apush)
[![Website](https://img.shields.io/website-up-down-green-red/http/alandefreitas.github.io/sympp.svg?label=Documentation)](https://alandefreitas.github.io/sympp/)
<!-- [![Latest Release](https://img.shields.io/github/release/alandefreitas/sympp.svg?label=Download)](https://GitHub.com/alandefreitas/sympp/releases/) -->

![Math](https://www.wellesley.edu/sites/default/files/styles/news_refresh_hero/public/assets/dailyshot/ds_461390782.jpg?itok=jr0Buv1t)

<div style="text-align:center">
<im src="documentation/im/front2d_b.svg" alt="" data-canonical-src="2-dimensional front" width="800" height="200" />
</div>

Symbolic computing deals with algorithms that manipulate mathematical expressions. They are useful for mathematical experiments, scientific computation, optimization, improving numerical methods, reducing approximation errors, and as an alternative when numerical analysis fails. However, algebraic manipulations tend to have very low performance, and that makes symbolic computing less attractive to a variety of fields that could otherwise benefit from it. In that context, this project proposes a library for symbolic computing whose symbols can compile themselves. SymPP can numerically evaluate expressions in runtime by 1) generating and compiling the symbolic tree as machine code, 2) concatenating recursive lambdas, or 3) with a depth-first search on the symbol nodes.

[![Facebook](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+Facebook&logo=facebook)](https://www.facebook.com/sharer/sharer.php?t=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself&u=https://github.com/alandefreitas/sympp/)
[![QZone](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+QZone&logo=qzone)](http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?url=https://github.com/alandefreitas/sympp/&title=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself&summary=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself)
[![Weibo](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+Weibo&logo=sina-weibo)](http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?url=https://github.com/alandefreitas/sympp/&title=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself&summary=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself)
[![Reddit](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+Reddit&logo=reddit)](http://www.reddit.com/submit?url=https://github.com/alandefreitas/sympp/&title=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself)
[![Twitter](https://img.shields.io/twitter/url/http/shields.io.svg?label=Share+on+Twitter&style=social)](https://twitter.com/intent/tweet?text=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself&url=https://github.com/alandefreitas/sympp/&hashtags=SymbolicComputing,ComputerAlgebra,SymbolicComputation)
[![LinkedIn](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+LinkedIn&logo=linkedin)](https://www.linkedin.com/shareArticle?mini=false&url=https://github.com/alandefreitas/sympp/&title=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself)
[![WhatsApp](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+WhatsApp&logo=whatsapp)](https://api.whatsapp.com/send?text=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself:+https://github.com/alandefreitas/sympp/)
[![Line.me](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+Line.me&logo=line)](https://lineit.line.me/share/ui?url=https://github.com/alandefreitas/sympp/&text=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself)
[![Telegram.me](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+Telegram.me&logo=telegram)](https://telegram.me/share/url?url=https://github.com/alandefreitas/sympp/&text=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself)
[![HackerNews](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+HackerNews&logo=y-combinator)](https://news.ycombinator.com/submitlink?u=https://github.com/alandefreitas/sympp/&t=SymPP:%20A%20Symbolic%20Library%20That%20Compiles%20Itself)

> Please note that this is a work in progress. See our [roadmap](#roadmap) to understand some of our plans for the future. 

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
<details>
<summary>Table of Contents</summary>

- [Examples](#examples)
  - [Introduction](#introduction)
  - [Operations](#operations)
  - [Equations](#equations)
  - [Simplification](#simplification)
  - [Compiling expressions](#compiling-expressions)
- [Benchmarks](#benchmarks)
- [Integration](#integration)
  - [Packages](#packages)
  - [Build from source](#build-from-source)
  - [CMake targets](#cmake-targets)
  - [Other build systems](#other-build-systems)
- [Limitations and Roadmap](#limitations-and-roadmap)
- [Contributing](#contributing)
  - [Contributors](#contributors)
- [Thanks](#thanks)

</details>
<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Examples

For complete examples, see the directory [examples](./examples/).

### Introduction

> ![Work in progress](https://image.flaticon.com/icons/png/128/2918/2918921.png) Work in progress

### Operations

> ![Work in progress](https://image.flaticon.com/icons/png/128/2918/2918921.png) Work in progress

### Equations

> ![Work in progress](https://image.flaticon.com/icons/png/128/2918/2918921.png) Work in progress

### Simplification

> ![Work in progress](https://image.flaticon.com/icons/png/128/2918/2918921.png) Work in progress

### Compiling expressions

> ![Work in progress](https://image.flaticon.com/icons/png/128/2918/2918921.png) Work in progress

## Benchmarks

These benchmarks illustrate how we can have significant performance gains by compiling expressions. The compiled expressions not only perform better than the usual symbolic evaluation but also the original unsimplified numeric evaluation.

> ![Work in progress](https://image.flaticon.com/icons/png/128/2918/2918921.png) Work in progress

## Integration

### Packages

You can download the [binary packages from the CI artifacts](https://github.com/alandefreitas/sympp/actions?query=workflow%3ASymPP+event%3Apush) or build the library [from the source files](#build-from-source). 

Once the package is installed, link your C++ program to the library and include the directories where you installed SymPP. 

Unless you changed the default options, the C++ library is likely to be in `/usr/local/` (Linux / Mac OS) or `C:/Program Files/` (Windows). The installer will try to find the directory where you usually keep your  libraries but that's not always perfect.

If you are using CMake, you can then find SymPP with the usual `find_package` command:

```cmake
find_package(SymPP REQUIRED)
# ...
target_link_libraries(my_target PUBLIC sympp)
```

CMake should be able to locate the `SymPPConfig.cmake` script automatically if you installed the library under `/usr/local/` (Linux / Mac OS) or `C:/Program Files/` (Windows). Otherwise, you need to include your installation directory in `CMAKE_MODULE_PATH` first: 

```cmake
list(APPEND CMAKE_MODULE_PATH put/your/installation/directory/here)
find_package(SymPP REQUIRED)
# ...
target_link_libraries(my_target PUBLIC sympp)
```

### Build from source

#### Dependencies

This section lists the dependencies you need before installing SymPP from source. The build script will try to find all these dependencies for you:

* C++17
* CMake 3.14 or higher
* LibTCC (Embedded)

<details>
    <summary>Instructions: Linux/Ubuntu/GCC</summary>
    
Check your GCC version

```bash
g++ --version
```

The output should have something like

```console
g++-8 (Ubuntu 8.4.0-1ubuntu1~18.04) 8.4.0
```

If you see a version before GCC-8, update it with

```bash
sudo apt update
sudo apt install gcc-8
sudo apt install g++-8
```

To update to any other version, like GCC-9 or GCC-10:

```bash
sudo apt install build-essential
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt install g++-10
```

Once you installed a newer version of GCC, you can link it to `update-alternatives`. For instance, if you have GCC-7 and GCC-10, you can link them with:

```bash
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 7
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 7
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 10
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 10
```

You can now use `update-alternatives` to set you default `gcc` and `g++`:

```bash
update-alternatives --config g++
update-alternatives --config gcc
```

Check your CMake version:

```bash
cmake --version
```

If it's older than CMake 3.14, update it with

```bash
sudo apt upgrade cmake
```

or download the most recent version from [cmake.org](https://cmake.org/).

[Later](#build-the-examples) when running CMake, make sure you are using GCC-8 or higher by appending the following options:

```bash
-DCMAKE_C_COMPILER=/usr/bin/gcc-8 -DCMAKE_CXX_COMPILER=/usr/bin/g++-8
```

</details>

<details>
    <summary>Instructions: Mac Os/Clang</summary>

Check your Clang version:

```bash
clang --version
```

The output should have something like

```console
Apple clang version 11.0.0 (clang-1100.0.33.8)
```

If you see a version before Clang 11, update XCode in the App Store or update clang with homebrew. 

Check your CMake version:

```bash
cmake --version
```

If it's older than CMake 3.14, update it with

```bash
sudo brew upgrade cmake
```

or download the most recent version from [cmake.org](https://cmake.org/).

If the last command fails because you don't have [Homebrew](https://brew.sh) on your computer, you can install it with

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

or you can follow the instructions in [https://brew.sh](https://brew.sh).

</details>

<details>
    <summary>Instructions: Windows/MSVC</summary>
    
* Make sure you have a recent version of [Visual Studio](https://visualstudio.microsoft.com)
* Download Git from [https://git-scm.com/download/win](https://git-scm.com/download/win) and install it
* Download CMake from [https://cmake.org/download/](https://cmake.org/download/) and install it

</details>

#### Build the Examples

This will build the examples in the `build/examples` directory:

```bash
mkdir build
cmake -version
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O2"
cmake --build . -j 2 --config Release
```

On windows, replace `-O2` with `/O2`.

#### Installing SymPP from Source

This will install SymPP on your system:

```bash
mkdir build
cmake -version
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O2" -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF 
cmake --build . -j 2 --config Release
cmake --install .
```

On windows, replace `-O2` with `/O2`. You might need `sudo` for this last command.

#### Building the packages

This will create the binary packages you can use to install SymPP on your system:

```bash
mkdir build
cmake -version
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O2" -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF
cmake --build . -j 2 --config Release
cmake --install .
cpack .
```

On windows, replace `-O2` with `/O2`. You might need `sudo` for this last command.

### CMake targets

#### Find it as a CMake Package

If you have the library installed, you can call

```cmake
find_package(SymPP)
```

from your CMake build script.

When creating your executable, link the library to the targets you want:

```
add_executable(my_target main.cpp)
target_link_libraries(my_target PUBLIC sympp)
```

Add this header to your source files:

```cpp
#include <sympp/sympp.h>
```

#### Use it as a CMake subdirectory

You can use SymPP directly in CMake projects without installing it. Check if you have [Cmake](http://cmake.org) 3.14+ installed:

```bash
cmake -version
```

Clone the whole project
 
```bash
git clone https://github.com/alandefreitas/sympp/
```

and add the subdirectory to your CMake project:

```cmake
add_subdirectory(sympp)
```

When creating your executable, link the library to the targets you want:

```
add_executable(my_target main.cpp)
target_link_libraries(my_target PUBLIC sympp)
```

Add this header to your source files:

```cpp
#include <sympp/sympp.h>
```

However, it's always recommended to look for SymPP with `find_package` before including it as a subdirectory. Otherwise, we can get [ODR errors](https://en.wikipedia.org/wiki/One_Definition_Rule) in larger projects. 

#### CMake with Automatic Download

Check if you have [Cmake](http://cmake.org) 3.14+ installed:

```bash
cmake -version
```

Install [CPM.cmake](https://github.com/TheLartians/CPM.cmake) and then:

```cmake
CPMAddPackage(
    NAME SymPP
    GITHUB_REPOSITORY alandefreitas/sympp
    GIT_TAG origin/master # or whatever tag you want
)
# ...
target_link_libraries(my_target PUBLIC sympp)
```

Then add this header to your source files:

```cpp
#include <sympp/sympp.h>
```

However, it's always recommended to look for SymPP with `find_package` before including it as a subdirectory. You can use:

```
option(CPM_USE_LOCAL_PACKAGES "Try `find_package` before downloading dependencies" ON)
```

to let CPM.cmake do that for you. Otherwise, we can get [ODR errors](https://en.wikipedia.org/wiki/One_Definition_Rule) in larger projects.

### Other build systems

If you want to use it in another build system you can either install the library (Section [*Installing*](#installing)) or you have to somehow rewrite the build script.

If you want to rewrite the build script, your project needs to 1) include the headers and compile all source files in the [`sources`](sources) directory, and 2) link the dependencies described in [`sources/CMakeLists.txt`](sources/CMakeLists.txt).

Then add this header to your source files:

```cpp
#include <sympp/sympp.h>
```

## Limitations and Roadmap

Things SymPP needs to improve: 

* Lots of tests
* Merge redundant `print` and `c_code` functions
* More printing formats (inline, code, latex)
* Smarter simplification functions

Things SymPP doesn't do yet:

* Serialization
* Interval arithmetic 
* Black-box function lambdas
* Calculus
* Variable integration graphs (VIG)
* Conditional VIGs
* OpenCL
* Matrices

## Contributing

There are many ways in which you can contribute to this library:

* Testing the library in new environments
* Contributing with interesting examples
* Developing new node types
* Finding bugs in general
* Whatever idea seems interesting to you

If contributing with code, please turn the pedantic mode ON (`-DBUILD_WITH_PEDANTIC_WARNINGS=ON`), don't forget cppcheck and clang-format.

<details>
    <summary>Example: CLion</summary>
    
![CLion Settings with Pedantic Mode](./documentation/images/pedantic_clion.png)
    
</details>

### Contributors

<!-- readme: collaborators,contributors -start --> 
<table>
<tr>
    <td align="center">
        <a href="https://github.com/alandefreitas">
            <img src="https://avatars0.githubusercontent.com/u/5369819?v=4" width="100;" alt="alandefreitas"/>
            <br />
            <sub><b>Alan De Freitas</b></sub>
        </a>
    </td></tr>
</table>
<!-- readme: collaborators,contributors -end -->

## Thanks

We would like to thank the developers of these libraries:

- [SymPy](https://www.sympy.org/): we often use SymPy as a reference for our interface
- [SymbolicC++](http://symboliccpp.sourceforge.net): we used SymbolicC++ as a reference for our polymorphic design
- [TinyCC](https://github.com/TinyCC/tinycc): we use TinyCC to compile expressions to machine code

