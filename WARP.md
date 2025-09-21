# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

Project summary
- Language/tooling: C++17 with CMake. A convenience Makefile wraps common CMake actions.
- Primary source tree: cpp-compiler/ (uses CMake). Note: a separate src/ tree exists but is not part of the CMake build.
- Prerequisites: CMake >= 3.10 and a C++17 compiler (clang or gcc)

Common commands
- Build (Release by default)
  ```bash path=null start=null
  make build
  ```
- Build (Debug)
  ```bash path=null start=null
  make build BUILD_TYPE=Debug
  ```
- Run the compiler demo
  ```bash path=null start=null
  make run
  # or directly
  ./cpp-compiler/build/mycompiler
  ```
- Clean build artifacts
  ```bash path=null start=null
  make clean
  ```
- Direct CMake (without Makefile)
  ```bash path=null start=null
  cmake -S cpp-compiler -B cpp-compiler/build -DCMAKE_BUILD_TYPE=Release
  cmake --build cpp-compiler/build -- -j
  cmake --build cpp-compiler/build --target run   # runs the demo target
  ```
- Debugging (macOS, lldb)
  ```bash path=null start=null
  make build BUILD_TYPE=Debug
  lldb ./cpp-compiler/build/mycompiler -o run
  ```
- Tests: none configured in this repo as of now.
- Lint/format: no configuration (e.g., clang-tidy/clang-format) is present in this repo.

Big-picture architecture
This repository implements a simple compiler pipeline with clearly separated phases, plus a small VM backend. The active code lives under cpp-compiler/src/:

1) Lexical analysis (tokenization)
   - Location: cpp-compiler/src/lexer/
   - Key types/APIs: Token, TokenType, tokenize(...), tokenTypeToStr(...)
   - Responsibility: Convert source text into a sequence of tokens for the parser.

2) Parsing (AST construction)
   - Location: cpp-compiler/src/parser/
   - Key types/APIs: ASTNode (parser-local simple AST in this folder), parse(...)
   - Responsibility: Build an abstract syntax tree (AST) from tokens using a straightforward recursive-descent approach.

3) Shared AST and tokens
   - Location: cpp-compiler/src/common/
   - Key types: AST node classes (ProgramNode, NumberLiteralNode, IdentifierNode, AssignmentNode, BinaryOpNode, UnaryOpNode), token.h definitions
   - Responsibility: Shared data structures used by later phases (semantic analysis, codegen, VM), separate from the parser’s minimal AST typedefs.

4) Semantic analysis
   - Location: cpp-compiler/src/semantic/
   - Key types/APIs: SemanticAnalyzer, Symbol, symbol table accessors
   - Responsibility: Walk the AST, enforce language rules, build symbol tables, and collect semantic errors.

5) Intermediate representation (IR) code generation
   - Location: cpp-compiler/src/codegen/
   - Key types/APIs: OpCode enum, Instruction struct, CodeGenerator
   - Responsibility: Convert the AST (and semantic info) into a linear IR (vector<Instruction>) suitable for assembly/VM execution.

6) Assembler + Virtual Machine backend
   - Locations: cpp-compiler/src/assembler/, cpp-compiler/src/vm.*
   - Key types/APIs: VMOpCode, VMInstruction, Assembler (IR -> VM instructions), VirtualMachine (execute)
   - Responsibility: Translate IR to VM instructions and execute them on a minimal stack-based VM.

7) Entrypoint
   - File: cpp-compiler/src/main.cpp
   - Behavior: Demonstrates the pipeline on a hardcoded snippet (e.g., "int x = 5 + 3;") by tokenizing, parsing, and printing an AST. It does not currently accept CLI inputs.

Build outputs
- Binary: ./cpp-compiler/build/mycompiler
- Build directory: ./cpp-compiler/build (safe to delete; recreated by configure/build)

Noteworthy notes for Warp
- Prefer the Makefile targets (build, run, clean) for routine tasks; they wrap the correct CMake source/build directories.
- The src/ directory at the repo root contains older/experimental files and is not included in the CMake target. Focus on cpp-compiler/ for authoritative code.
- No tests or lint configuration exist at this time; commands above are intentionally limited to build/run/clean.

Design notes and integration hints
- Dual ASTs: The parser uses a minimal AST (cpp-compiler/src/parser/parser.h), while later phases use the shared classes in cpp-compiler/src/common/ast.h. If you extend the pipeline beyond parsing in main, ensure there is a conversion step from the parser-local AST to the shared AST types before semantic analysis and codegen.
- Dual token definitions: A lightweight token is defined in cpp-compiler/src/lexer/lexer.h, while a richer Token/TokenType lives in cpp-compiler/src/common/token.h (with line/column). Consider unifying or adding an adapter if you route tokens into later phases that expect common/token.h.
- Entrypoint scope: cpp-compiler/src/main.cpp demonstrates tokenization and parsing on a hardcoded snippet. It does not invoke semantic analysis, IR generation, or VM execution yet. To exercise the full pipeline, wire SemanticAnalyzer -> CodeGenerator -> Assembler -> VirtualMachine in main or a separate driver.
- CMake tips: When adding new source files, include them in add_executable in cpp-compiler/CMakeLists.txt, and if you add new include dirs, reflect them in target_include_directories.

Source references
- Build wrapper: Makefile
- CMake config: cpp-compiler/CMakeLists.txt
- Compiler phases: cpp-compiler/src/{lexer,parser,common,semantic,codegen,assembler}
- VM: cpp-compiler/src/vm.*
- Demo main: cpp-compiler/src/main.cpp
