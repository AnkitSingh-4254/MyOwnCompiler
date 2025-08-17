#include "codegen.h"

// Utility: construct temp variable names
std::string CodeGenerator::makeTempVar() {
    return "_t" + std::to_string(tempVarCounter++);
}

CodeGenerator::CodeGenerator() : tempVarCounter(0), symbolTable(nullptr) {}

void CodeGenerator::setSymbolTable(const std::unordered_map<std::string, Symbol>* table) {
    symbolTable = table;
}

void CodeGenerator::generate(const std::unique_ptr<ASTNode>& root) {
    instructions.clear();
    tempVarCounter = 0;
    visit(root.get());
}

const std::vector<Instruction>& CodeGenerator::getInstructions() const {
    return instructions;
}

void CodeGenerator::visit(const ASTNode* node) {
    if (!node) return;
    if (auto prog = dynamic_cast<const ProgramNode*>(node)) {
        visitProgram(prog);
    } else if (auto assign = dynamic_cast<const AssignmentNode*>(node)) {
        visitAssignment(assign);
    } else if (auto bin = dynamic_cast<const BinaryOpNode*>(node)) {
        visitBinaryOp(bin);
    } else if (auto unary = dynamic_cast<const UnaryOpNode*>(node)) {
        visitUnaryOp(unary);
    } else if (auto ident = dynamic_cast<const IdentifierNode*>(node)) {
        visitIdentifier(ident);
    } else if (auto num = dynamic_cast<const NumberLiteralNode*>(node)) {
        visitNumberLiteral(num);
    } else if (auto str = dynamic_cast<const StringLiteralNode*>(node)) {
        visitStringLiteral(str);
    }
    // Add more else-if's as your AST expands
}

void CodeGenerator::visitProgram(const ProgramNode* prog) {
    for (const auto& stmt : prog->statements) {
        visit(stmt.get());
    }
}

void CodeGenerator::visitAssignment(const AssignmentNode* assign) {
    // Assume simple variable = expression
    const auto* lhsIdent = dynamic_cast<IdentifierNode*>(assign->lhs.get());
    if (!lhsIdent) return;

    // Evaluate right-hand side
    visit(assign->rhs.get());

    // Store top of stack (result) in variable
    instructions.emplace_back(OpCode::STORE, lhsIdent->name);
    variables[lhsIdent->name] = VariableInfo{lhsIdent->name};  // Optionally, set offset/location
}

void CodeGenerator::visitBinaryOp(const BinaryOpNode* bin) {
    // Post-order / left-right for stack machine
    visit(bin->left.get());
    visit(bin->right.get());
    // Map operator to OpCode
    OpCode op = OpCode::ADD;
    if      (bin->op == "+") op = OpCode::ADD;
    else if (bin->op == "-") op = OpCode::SUB;
    else if (bin->op == "*") op = OpCode::MUL;
    else if (bin->op == "/") op = OpCode::DIV;
    else if (bin->op == "==") op = OpCode::CMP_EQ;
    else if (bin->op == "!=") op = OpCode::CMP_NE;
    else if (bin->op == "<")  op = OpCode::CMP_LT;
    else if (bin->op == "<=") op = OpCode::CMP_LE;
    else if (bin->op == ">")  op = OpCode::CMP_GT;
    else if (bin->op == ">=") op = OpCode::CMP_GE;
    // Add more operators as needed
    instructions.emplace_back(op);
}

void CodeGenerator::visitUnaryOp(const UnaryOpNode* unary) {
    visit(unary->operand.get());
    OpCode op = OpCode::NEG;
    if (unary->op == "-") op = OpCode::NEG;
    else if (unary->op == "!") op = OpCode::CMP_EQ; // Or custom unary-not opcode
    instructions.emplace_back(op);
}

void CodeGenerator::visitIdentifier(const IdentifierNode* ident) {
    // Load variable onto stack
    instructions.emplace_back(OpCode::LOAD, ident->name);
}

void CodeGenerator::visitNumberLiteral(const NumberLiteralNode* num) {
    instructions.emplace_back(OpCode::PUSH, num->value);
}

void CodeGenerator::visitStringLiteral(const StringLiteralNode* str) {
    instructions.emplace_back(OpCode::PUSH, str->value);
}
