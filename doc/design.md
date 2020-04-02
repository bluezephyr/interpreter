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

# Parsing
The parsing is done using recursion. A list of token is provided by the Lexer and the Parser uses
two pointers - curToken and peekToken - to access this list. Each language structure has its own
parse function. Each parse function evaluates the curToken and if it is syntactically correct,
the token is consumed. This means that the ownership of the curToken is moved from the Lexer to the
parser. It is therefore also required by the parse function to update the curToken and the peekToken
pointers at this stage.

If an unexpected token is encountered, an exception is raised. When such an exception is raised, the
token is not consumed. Thus, the curToken and the peekToken pointers are not updates. The parser will
then roll back to a defined state and continue the parsing from there. An example is when parsing
statements. If an error is found while parsing an expression, the statement will be discarded, the
parser will scan the token list until the next statement and continue the parsing there.
