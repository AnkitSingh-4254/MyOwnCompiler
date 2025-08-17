#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "codegen.h"

// Enum for the virtual machine opcodes
enum class VMOpCode {
    VM_PUSH,
    VM_POP,
    VM_LOAD,
    VM_STORE,
    VM_ADD,
    VM_SUB,
    VM_MUL,
    VM_DIV,
    VM_NEG,
    VM_CMP_EQ,
    VM_CMP_NE,
    VM_CMP_LT,
    VM_CMP_LE,
    VM_CMP_GT,
    VM_CMP_GE,
    VM_JUMP,
    VM_JUMP_IF_TRUE,
    VM_JUMP_IF_FALSE,
    VM_LABEL,
    VM_CALL,
    VM_RETURN
    // Extend this list to match all supported instructions
};

// VM instruction structure
struct VMInstruction {
    VMOpCode opcode;
    std::string operand1;
    std::string operand2;
    std::string label;

    VMInstruction(VMOpCode code,
                  const std::string& op1 = "",
                  const std::string& op2 = "",
                  const std::string& lbl = "")
        : opcode(code), operand1(op1), operand2(op2), label(lbl) {}
};

class Assembler {
public:
    Assembler();

    // Accepts intermediate representation and produces VM instructions
    void assemble(const std::vector<Instruction>& irCode);

    const std::vector<VMInstruction>& getVMInstructions() const;

private:
    void emit(VMOpCode opcode,
              const std::string& operand1 = "",
              const std::string& operand2 = "",
              const std::string& label = "");

    // Maps IR OpCode to VMOpCode
    VMOpCode mapOpCode(OpCode op) const;

    std::vector<VMInstruction> vmInstructions;
};
