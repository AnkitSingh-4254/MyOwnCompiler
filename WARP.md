# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

Project summary
- C++17 project built with CMake, wrapped by a small Makefile.
- Active source tree: cpp-compiler/ (the root-level src/ is historical and not built).
- Requires CMake >= 3.10 and a C++17 compiler (clang or gcc).

Quick start
- Build and run
  ```bash path=null start=null
  make run
  ```
- Build (Debug)
  ```bash path=null start=null
  make build BUILD_TYPE=Debug
  ```
- Clean
  ```bash path=null start=null
  make clean
  ```
- Direct CMake (if you prefer)
  ```bash path=null start=null
  cmake -S cpp-compiler -B cpp-compiler/build -DCMAKE_BUILD_TYPE=Release
  cmake --build cpp-compiler/build -- -j
  ./cpp-compiler/build/mycompiler
  ```
- Debugging (macOS)
  ```bash path=null start=null
  make build BUILD_TYPE=Debug
  lldb ./cpp-compiler/build/mycompiler -o run
  ```
- Tests/linters: none configured in this repo.

What’s here (high level)
This repo implements a small compiler pipeline with a VM backend. The code lives in cpp-compiler/src/:
- Lexer (lexer/): tokenize source into tokens.
- Parser (parser/): builds a minimal parser-local AST.
- Common (common/): shared AST classes (ProgramNode, NumberLiteralNode, IdentifierNode, AssignmentNode, BinaryOpNode, UnaryOpNode) and token utilities.
- Semantic (semantic/): SemanticAnalyzer walks the shared AST, tracks symbols, and reports errors.
- Codegen (codegen/): emits a simple linear IR (Instruction, OpCode).
- Assembler + VM (assembler/, vm.*): translates IR to VM instructions and executes them.

Entrypoint
- File: cpp-compiler/src/main.cpp
- Behavior: runs the full pipeline on a tiny assignment (e.g., "x = 5 + 3;") and prints the result from the VM. The parser uses its own lightweight AST which is converted to the shared AST before semantic/codegen.

Notes
- Prefer the Makefile targets; they point CMake at the right source/build dirs.
- Build outputs land in ./cpp-compiler/build/ (binary: ./cpp-compiler/build/mycompiler).
- The root-level src/ tree is legacy and excluded from the build.

Pointers
- Build wrapper: Makefile
- CMake: cpp-compiler/CMakeLists.txt
- Phases: cpp-compiler/src/{lexer,parser,common,semantic,codegen,assembler}
- VM: cpp-compiler/src/vm.*
- Main: cpp-compiler/src/main.cpp
