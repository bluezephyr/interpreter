# Monkey Language Interpreter
This repository contains an interpreter for the Monkey language written in C++. The work is based on
the book *"Writing An Interpreter In Go"* by Thorsten Ball (https://interpreterbook.com/) but with the
implementation done in C++ instead of Go. For more details regarding the Monkey Language, see [MonkeyLang](https://monkeylang.org/).

**Note!** The current status of the implementation is very rudimentary; only a first version of a
lexical analyser and a simple REPL have been implemented so far.

## REPL
A first version of a Read Eval Print Loop (REPL) has been implemented. The REPL is an interactive
command line tool that reads input, send this input to the interpreter and outputs a result. An
example can be seen below. The interpreter will take a line as input an output its tokens.

    Monkey Programming Language Interpreter!
    See https://monkeylang.org/ for more information
    Type in your commands (exit with CRTL-D)
    >>> let x=5;
    let
    x
    =
    5
    ;
    >>>

## Build
The implementation of the interpreter is done i C++ and the build chain uses CMake. See
[CMake](https://cmake.org/) for more information.

Use the steps below to build and run the interpreter. The procedure need a Unix-like build environment.
On Windows it is possible to use [Cygwin](https://www.cygwin.com/). Make sure to include a compiler
for C++ and CMake.

    mkdir build
    cd build
    cmake ..
    make all -j

This will build an executable in the folder build/src.

## Unit Tests
The project includes a set of unit tests that use the [CppUTest](http://cpputest.github.io/)
test framework. To be able to build the tests, the CppUTest framework must be available on the computer.
This can be accomplished either by having the framework installed or by downloading and building it
locally. The CMakeLists.txt for the test cases support both alternatives.

To build the unit tests, set `COMPILE_TESTS=ON` when calling CMake.

### Pre-Installed CppUTest
On a system with CppUTest pre-installed, use the following sequence to generate makefiles, build
and run the tests:

    mkdir build
    cd build
    cmake -DCOMPILE_TESTS=ON ..
    make all test -j

### Locally Built CppUTest
To use a locally built version of the CppUTest framework, set the `CPPUTEST_HOME` variable to specify
the location of the locally built CppUTest framework (`-DCPPUTEST_HOME=\<path to CppUTest>`). Note that
the build needs the file `CppUTestConfig.cmake` that is created during the build.

Use the steps below to build the CppUTest framework or see [CppUTest](http://cpputest.github.io/).

    git clone https://github.com/cpputest/cpputest.git
    cd cpputest/cpputest_build
    cmake ..
    make all test -j
