#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "assembler.h"

class VirtualMachine {
public:
    VirtualMachine();

    // Load and execute a program (VM instructions)
    void execute(const std::vector<VMInstruction>& program);

    // Optional: access memory/register state for inspection
    int getVariable(const std::string& name) const;

private:
    std::vector<int> stack;
    std::unordered_map<std::string, int> memory;

    size_t ip = 0; // Instruction pointer

    void executeInstruction(const VMInstruction& instr);
};
