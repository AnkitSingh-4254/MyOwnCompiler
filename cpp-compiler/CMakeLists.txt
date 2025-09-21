cmake_minimum_required(VERSION 3.10)
project(mycompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Add the executable and all .cpp files
add_executable(mycompiler
    src/main.cpp
    src/vm.cpp

    src/assembler/assembler.cpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/codegen/codegen.cpp
    src/codegen/opcode.h     # included for completeness; not required by CMake
    src/common/ast.cpp
    src/common/token.cpp
    src/semantic/semantic.cpp
)

# Add include directories so headers like "lexer.h" can be found
target_include_directories(mycompiler PRIVATE
    src
    src/assembler
    src/lexer
    src/parser
    src/codegen
    src/common
    src/semantic

)
# Custom run target
add_custom_target(run
    COMMAND mycompiler
    DEPENDS mycompiler
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
