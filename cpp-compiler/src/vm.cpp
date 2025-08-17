#include "vm.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

VirtualMachine::VirtualMachine() : ip(0) {}

void VirtualMachine::execute(const std::vector<VMInstruction>& program) {
    stack.clear();
    memory.clear();
    ip = 0;
    while (ip < program.size()) {
        executeInstruction(program[ip]);
        ip++;
    }
}

void VirtualMachine::executeInstruction(const VMInstruction& instr) {
    if (instr.opcode == VMOpCode::VM_PUSH) {
        stack.push_back(std::stoi(instr.operand1));
    } else if (instr.opcode == VMOpCode::VM_POP) {
        if (!stack.empty()) stack.pop_back();
    } else if (instr.opcode == VMOpCode::VM_LOAD) {
        stack.push_back(memory[instr.operand1]);
    } else if (instr.opcode == VMOpCode::VM_STORE) {
        if (!stack.empty()) {
            int val = stack.back(); stack.pop_back();
            memory[instr.operand1] = val;
        }
    } else if (instr.opcode == VMOpCode::VM_ADD) {
        int b = stack.back(); stack.pop_back();
        int a = stack.back(); stack.pop_back();
        stack.push_back(a + b);
    } else if (instr.opcode == VMOpCode::VM_SUB) {
        int b = stack.back(); stack.pop_back();
        int a = stack.back(); stack.pop_back();
        stack.push_back(a - b);
    } else if (instr.opcode == VMOpCode::VM_MUL) {
        int b = stack.back(); stack.pop_back();
        int a = stack.back(); stack.pop_back();
        stack.push_back(a * b);
    } else if (instr.opcode == VMOpCode::VM_DIV) {
        int b = stack.back(); stack.pop_back();
        int a = stack.back(); stack.pop_back();
        stack.push_back(a / b);
    } else if (instr.opcode == VMOpCode::VM_NEG) {
        int a = stack.back(); stack.pop_back();
        stack.push_back(-a);
    } else if (instr.opcode == VMOpCode::VM_CMP_EQ) {
        int b = stack.back(); stack.pop_back();
        int a = stack.back(); stack.pop_back();
        stack.push_back(a == b ? 1 : 0);
    } else if (instr.opcode == VMOpCode::VM_CMP_NE) {
        int b = stack.back(); stack.pop_back();
        int a = stack.back(); stack.pop_back();
        stack.push_back(a != b ? 1 : 0);
    } else if (instr.opcode == VMOpCode::VM_CMP_LT) {
        int b = stack.back(); stack.pop_back();
        int a = stack.back(); stack.pop_back();
        stack.push_back(a < b ? 1 : 0);
    } else if (instr.opcode == VMOpCode::VM_CMP_LE) {
        int b = stack.back(); stack.pop_back();
        int a = stack.back(); stack.pop_back();
        stack.push_back(a <= b ? 1 : 0);
    } else if (instr.opcode == VMOpCode::VM_CMP_GT) {
        int b = stack.back(); stack.pop_back();
        int a = stack.back(); stack.pop_back();
        stack.push_back(a > b ? 1 : 0);
    } else if (instr.opcode == VMOpCode::VM_CMP_GE) {
        int b = stack.back(); stack.pop_back();
        int a = stack.back(); stack.pop_back();
        stack.push_back(a >= b ? 1 : 0);
    }
    // You can further coordinate jump/call/return instructions as needed
}

int VirtualMachine::getVariable(const std::string& name) const {
    auto it = memory.find(name);
    if (it != memory.end()) return it->second;
    throw std::runtime_error("Variable not found: " + name);
}
