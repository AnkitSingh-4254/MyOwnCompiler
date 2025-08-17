#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "ast.h"

// Enum for the intermediate code opcodes
enum class OpCode {
    PUSH,
    POP,
    LOAD,
    STORE,
    ADD,
    SUB,
    MUL,
    DIV,
    NEG,
    CMP_EQ,
    CMP_NE,
    CMP_LT,
    CMP_LE,
    CMP_GT,
    CMP_GE,
    JUMP,
    JUMP_IF_TRUE,
    JUMP_IF_FALSE,
    LABEL,
    CALL,
    RETURN,
    // Add more as your language requires
};

// Structure for intermediate code instructions
struct Instruction {
    OpCode opcode;
    std::string operand1;
    std::string operand2;
    std::string label;
    std::string comment;
    Instruction(OpCode op, const std::string& op1 = "", const std::string& op2 = "", const std::string& lbl = "", const std::string& cmt = "")
        : opcode(op), operand1(op1), operand2(op2), label(lbl), comment(cmt) {}
};

// Variable metadata (for codegen and storage)
struct VariableInfo {
    std::string name;
    int offset = 0; // stack offset or memory position, if relevant
};

class CodeGenerator {
public:
    CodeGenerator();

    void generate(const std::unique_ptr<ASTNode>& root);

    const std::vector<Instruction>& getInstructions() const;

    // Usually links to symbols for easier mapping
    void setSymbolTable(const std::unordered_map<std::string, Symbol>* table);

private:
    void visit(const ASTNode* node);
    void visitProgram(const ProgramNode* prog);
    void visitAssignment(const AssignmentNode* assign);
    void visitBinaryOp(const BinaryOpNode* bin);
    void visitUnaryOp(const UnaryOpNode* unary);
    void visitIdentifier(const IdentifierNode* ident);
    void visitNumberLiteral(const NumberLiteralNode* num);
    void visitStringLiteral(const StringLiteralNode* str);

    std::vector<Instruction> instructions;
    std::unordered_map<std::string, VariableInfo> variables;

    const std::unordered_map<std::string, Symbol>* symbolTable = nullptr;

    std::string makeTempVar();
    int tempVarCounter = 0;
};
