# SiteGen Lexer Toolkit

A small, extensible C++ lexer framework designed for **static site generation**, **syntax highlighting**, and **code-aware tooling**.

This project intentionally avoids heavyweight parser generators and build systems in favor of:
- hand-written state machines
- explicit control flow
- a simple, transparent Makefile
- zero runtime dependencies

The result is a lexer that is easy to reason about, easy to extend, and easy to embed into other tools.

---

## Project Goals

- Provide a **generic base lexer** (`Lexer`)
- Allow **language-specific lexers** via subclassing (e.g. `CPPLexer`, `JSLexer`)
- Tokenize source files for:
  - HTML syntax highlighting
  - static documentation pages
  - code tutorials
- Keep file I/O **outside** the lexer
- Keep the build system **simple and portable**

This is *not* a full compiler frontend — correctness is defined by **highlighting and tooling needs**, not by full language conformance.

---

## Architecture Overview

```
[file] → string → Lexer → tokens → HTML / JSON / output
```

### Key principles

- **Lexer does not open files**
- **Lexer operates on a `std::string`**
- **One-character-at-a-time scanning**
- **State-machine based dispatch**
- **No global state**

---

## Directory Layout

```
.
├── include/
│   ├── lexer.h          # Base lexer + Token definitions
│   ├── CPPLexer.h       # C++ lexer interface
│   └── JSLexer.h        # (optional) JavaScript lexer
├── src/
│   ├── lexer.cpp        # Core lexer implementation
│   ├── CPPLexer.cpp     # C++ lexer implementation
│   └── JSLexer.cpp      # JavaScript lexer implementation
├── build/
│   ├── debug/
│   └── release/
├── Makefile
└── README.md
```

---

## Core Types

### `TokenType`

Tokens are classified into high-level categories suitable for syntax highlighting:

- `Whitespace`
- `Newline`
- `Comment`
- `PreprocessorDirective`
- `String`
- `Number`
- `Operator`
- `Punctuator`
- `StandardKeyword`
- `StandardIdentifier`
- `SpecialIdentifier`

### `Token`

```cpp
struct Token {
    TokenType type;
    std::string text;
};
```

---

## Lexer Design

### Base Lexer (`Lexer`)

- Stores a reference to the source text
- Tracks position (`pos`)
- Maintains a token buffer
- Dispatches state handlers via a function table

```cpp
while (!eof()) {
    (this->*handlers()[state])();
}
```

The base lexer is **language-agnostic**.

---

### Language Lexers

Language lexers subclass `Lexer` and provide:

- a state enum
- handler functions (`handleNormal`, `handleIdentifier`, etc.)
- a handler table mapping states → member functions

Example:

```cpp
class CPPLexer : public Lexer {
public:
    using Lexer::Lexer;
};
```

---

## File I/O

File reading is intentionally **outside** the lexer.

Example helper:

```cpp
std::string src = readFile("example.cpp");
CPPLexer lexer(src);
auto tokens = lexer.lex();
```

This keeps the lexer:
- testable
- reusable
- independent of the filesystem

---

## Build System

The project uses a **single Makefile** with:

- separate `debug` and `release` builds
- user-selectable compiler (`g++` or `clang++`)
- optional sanitizers
- optional language lexers
- dependency tracking

### Default build

```sh
make
```

Builds:
```
build/debug/sitegen
build/release/sitegen
```

(Core lexer only)

---

### Select compiler

```sh
make CXX=clang++
```

---

### Enable lexers

```sh
make LEXERS=cpp
make LEXERS="cpp js"
```

---

### Debug with sanitizers

```sh
make debug SAN=address
make debug SAN=address,undefined CXX=clang++
```

---

### Clean

```sh
make clean
```

---

## Design Philosophy

- **Explicit over implicit**
- **Simple state machines over generators**
- **Build systems should be readable**
- **Lexers should be boring and predictable**
- **Tooling correctness > language completeness**

This project is designed to grow **incrementally**, not all at once.

---

## Planned Extensions

- HTML token emission
- JSON token output
- Line/column tracking
- Incremental rebuilds
- Additional language lexers
- Unit tests

---

## License

MIT (C) Jacob Petriella <aesycos@icloud.com>

---

## Author Notes

This codebase is intentionally conservative and mechanical.
It is designed to be *understood*, not impressed by.

If you are building a site generator, documentation system, or code tutorial platform, this lexer is meant to be a solid foundation — not a black box.
