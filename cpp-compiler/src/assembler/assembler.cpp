#include "assembler.h"
#include <stdexcept>
#include <iostream>

Assembler::Assembler() = default;

void Assembler::assemble(const std::vector<Instruction>& irCode) {
    vmInstructions.clear();
    for (const auto& instr : irCode) {
        VMOpCode vmOp = mapOpCode(instr.opcode);
        emit(vmOp, instr.operand1, instr.operand2, instr.label);
    }
}

const std::vector<VMInstruction>& Assembler::getVMInstructions() const {
    return vmInstructions;
}

void Assembler::emit(VMOpCode opcode,
                     const std::string& operand1,
                     const std::string& operand2,
                     const std::string& label) {
    vmInstructions.emplace_back(opcode, operand1, operand2, label);
}

VMOpCode Assembler::mapOpCode(OpCode op) const {
    switch (op) {
        case OpCode::PUSH:           return VMOpCode::VM_PUSH;
        case OpCode::POP:            return VMOpCode::VM_POP;
        case OpCode::LOAD:           return VMOpCode::VM_LOAD;
        case OpCode::STORE:          return VMOpCode::VM_STORE;
        case OpCode::ADD:            return VMOpCode::VM_ADD;
        case OpCode::SUB:            return VMOpCode::VM_SUB;
        case OpCode::MUL:            return VMOpCode::VM_MUL;
        case OpCode::DIV:            return VMOpCode::VM_DIV;
        case OpCode::NEG:            return VMOpCode::VM_NEG;
        case OpCode::CMP_EQ:         return VMOpCode::VM_CMP_EQ;
        case OpCode::CMP_NE:         return VMOpCode::VM_CMP_NE;
        case OpCode::CMP_LT:         return VMOpCode::VM_CMP_LT;
        case OpCode::CMP_LE:         return VMOpCode::VM_CMP_LE;
        case OpCode::CMP_GT:         return VMOpCode::VM_CMP_GT;
        case OpCode::CMP_GE:         return VMOpCode::VM_CMP_GE;
        case OpCode::JUMP:           return VMOpCode::VM_JUMP;
        case OpCode::JUMP_IF_TRUE:   return VMOpCode::VM_JUMP_IF_TRUE;
        case OpCode::JUMP_IF_FALSE:  return VMOpCode::VM_JUMP_IF_FALSE;
        case OpCode::LABEL:          return VMOpCode::VM_LABEL;
        case OpCode::CALL:           return VMOpCode::VM_CALL;
        case OpCode::RETURN:         return VMOpCode::VM_RETURN;
        // Extend here as you add more OpCodes
        default:
            std::cerr << "Unknown OpCode in assembler (possibly not mapped): " << static_cast<int>(op) << "\n";
            throw std::runtime_error("Unknown OpCode mapping in Assembler.");
    }
}
