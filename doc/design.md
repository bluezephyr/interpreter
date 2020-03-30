# Design
Design decisions are documented in this file.

## Basic Structure
The interpreter is divided into several different classes. The main classes are:
* **AST** - Abstract Syntax Tree
* **Lexer** - A lexical analyser that converts the input characters to tokens.
* **Parser** - A recursive descent parser that reads tokens from the Lexer and populates the AST
  using top down operator precedence ("Pratt Parser"). When a parser object is created, it takes
  a reference to a lexer object as input. 

In addition to this, a **REPL** (Read Eval Print Loop) is implemented in main. 

## Resource Allocation
[RAII](https://en.cppreference.com/w/cpp/language/raii) (Resource Allocation Is Initialization) is
used as a basic principle for memory allocation; the methods `new` and `delete` are not used (except
in unit tests). To achieve this, the code makes heavy use of constructions as `std::unique_ptr<>`,
`std::shared_ptr<>` and functions such as `std::make_unique`.

