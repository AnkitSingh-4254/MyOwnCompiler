#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include "ast.h"

// Struct to represent declared symbols (variables/functions)
struct Symbol {
    std::string name;
    std::string type;
    bool isFunction = false;
    // Add more metadata if needed (e.g., parameter list, scope level, etc.)
};

class SemanticAnalyzer {
public:
    SemanticAnalyzer();

    // Entry point
    void analyze(const std::unique_ptr<ASTNode>& root);

    // Access errors after analysis
    const std::vector<std::string>& getErrors() const;

    // Access symbol table after analysis (if needed)
    const std::unordered_map<std::string, Symbol>& getSymbolTable() const;

private:
    void visit(const ASTNode* node);
    void visitProgram(const ProgramNode* program);
    void visitBinaryOp(const BinaryOpNode* bin);
    void visitUnaryOp(const UnaryOpNode* unary);
    void visitAssignment(const AssignmentNode* assign);
    void visitIdentifier(const IdentifierNode* ident);
    void visitLiteral(const ASTNode* literal); // covers NumberLiteralNode, StringLiteralNode

    std::unordered_map<std::string, Symbol> symbolTable;
    std::vector<std::unordered_set<std::string>> scopeStack;
    std::vector<std::string> errors;

    void enterScope();
    void exitScope();
    bool declare(const std::string& name);
    bool isDeclared(const std::string& name) const;
};
