#include "semantic.h"
#include <iostream>

// Constructor: initialize scope stack with the global (outer) scope
SemanticAnalyzer::SemanticAnalyzer() {
    enterScope();
}

void SemanticAnalyzer::analyze(const std::unique_ptr<ASTNode>& root) {
    errors.clear();
    symbolTable.clear();
    scopeStack.clear();
    enterScope();
    visit(root.get());
    exitScope();
}

const std::vector<std::string>& SemanticAnalyzer::getErrors() const {
    return errors;
}

const std::unordered_map<std::string, Symbol>& SemanticAnalyzer::getSymbolTable() const {
    return symbolTable;
}

// Scope management
void SemanticAnalyzer::enterScope() {
    scopeStack.emplace_back();
}

void SemanticAnalyzer::exitScope() {
    if (!scopeStack.empty()) scopeStack.pop_back();
}

bool SemanticAnalyzer::declare(const std::string& name) {
    if (scopeStack.empty()) return false;
    if (scopeStack.back().count(name)) {
        errors.push_back("Redeclaration of '" + name + "' in this scope.");
        return false;
    }
    scopeStack.back().insert(name);
    return true;
}

bool SemanticAnalyzer::isDeclared(const std::string& name) const {
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
        if (it->count(name)) return true;
    }
    return false;
}

// Core visitor pattern logic
void SemanticAnalyzer::visit(const ASTNode* node) {
    if (!node) return;
    if (auto prog = dynamic_cast<const ProgramNode*>(node)) {
        visitProgram(prog);
    } else if (auto bin = dynamic_cast<const BinaryOpNode*>(node)) {
        visitBinaryOp(bin);
    } else if (auto unary = dynamic_cast<const UnaryOpNode*>(node)) {
        visitUnaryOp(unary);
    } else if (auto assign = dynamic_cast<const AssignmentNode*>(node)) {
        visitAssignment(assign);
    } else if (auto ident = dynamic_cast<const IdentifierNode*>(node)) {
        visitIdentifier(ident);
    } else if (dynamic_cast<const NumberLiteralNode*>(node) ||
               dynamic_cast<const StringLiteralNode*>(node)) {
        visitLiteral(node);
    }
    // Add more else-if's for new node types as you extend the AST
}

void SemanticAnalyzer::visitProgram(const ProgramNode* program) {
    for (const auto& stmt : program->statements) {
        visit(stmt.get());
    }
}

void SemanticAnalyzer::visitAssignment(const AssignmentNode* assign) {
    // Assume left is IdentifierNode for simplicity
    const auto* lhsIdent = dynamic_cast<IdentifierNode*>(assign->lhs.get());
    if (!lhsIdent) {
        errors.push_back("Left-hand side of assignment is not a variable name.");
        return;
    }
    // Declare if not already in the current scope
    if (!isDeclared(lhsIdent->name)) {
        declare(lhsIdent->name);
        symbolTable[lhsIdent->name] = Symbol{lhsIdent->name, "unknown", false};
    }
    visit(assign->rhs.get());
}

void SemanticAnalyzer::visitBinaryOp(const BinaryOpNode* bin) {
    visit(bin->left.get());
    visit(bin->right.get());
    // Optional: type checks, supported operator validation, etc.
}

void SemanticAnalyzer::visitUnaryOp(const UnaryOpNode* unary) {
    visit(unary->operand.get());
    // Optional: type check, operator validation, etc.
}

void SemanticAnalyzer::visitIdentifier(const IdentifierNode* ident) {
    if (!isDeclared(ident->name)) {
        errors.push_back("Reference to undeclared variable '" + ident->name + "'.");
    }
}

void SemanticAnalyzer::visitLiteral(const ASTNode* /*literal*/) {
    // Number and string literals always OK in most cases
}
