#ifndef OPCODE_H
#define OPCODE_H

#include <string>

// OpCode for code generation and assembler
enum class OpCode {
    LOAD_CONST,
    LOAD_VAR,
    STORE_VAR,
    ADD,
    SUB,
    MUL,
    DIV,
    NEG,
    CMP_EQ,
    CMP_NE,
    CMP_LT,
    CMP_GT,
    CMP_LE,
    CMP_GE,
    JUMP,
    JUMP_IF_FALSE,
    JUMP_IF_TRUE,
    LABEL,
    CALL,
    RETURN,
    PUSH_PARAM,
    PRINT,
    HALT,
    PUSH,
    POP,
    ALLOC,
    DEALLOC
};

// Instruction for codegen and assembler
struct Instruction {
    OpCode opcode;
    std::string operand1;
    std::string operand2;
    std::string label;
    std::string comment;

    Instruction(OpCode op, const std::string& op1 = "", const std::string& op2 = "", const std::string& lbl = "", const std::string& cmt = "")
        : opcode(op), operand1(op1), operand2(op2), label(lbl), comment(cmt) {}
};

#endif // OPCODE_H
