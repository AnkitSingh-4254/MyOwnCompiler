# MyOwnCompiler

MyOwnCompiler is a C++ compiler project with separate phases for lexing, parsing, semantic analysis, code generation, and a simple VM. The current codebase is organized under `cpp-compiler/` and uses CMake for builds.

## Features
- Lexical analysis (lexer)
- Parsing (recursive descent)
- Semantic analysis
- Code generation + opcodes
- Simple VM backend

## Project layout
- `cpp-compiler/` — CMake-based C++ source tree
  - `src/assembler/` — assembler
  - `src/lexer/` — lexer
  - `src/parser/` — parser
  - `src/semantic/` — semantic analysis
  - `src/codegen/` — code generation
  - `src/common/` — shared AST/token utilities
  - `src/main.cpp` — entrypoint

## Build (with CMake)
Out-of-source build in the `cpp-compiler/build/` directory:

```bash
cmake -S cpp-compiler -B cpp-compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build cpp-compiler/build -- -j
```

Run the compiler:

```bash
./cpp-compiler/build/mycompiler
```

Clean build artefacts (safe to delete):

```bash
rm -rf cpp-compiler/build
```

## Development
- Requires: CMake (>= 3.10) and a C++17 compiler (Clang or GCC)
- Recommended: VS Code with C++ extensions

## License
See `LICENSE`.
