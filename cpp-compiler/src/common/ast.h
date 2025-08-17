#pragma once

#include <string>
#include <vector>
#include <memory>

// Forward declare for cyclic references
class ASTNode;

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;
};

class NumberLiteralNode : public ASTNode {
public:
    std::string value;
    explicit NumberLiteralNode(const std::string& val) : value(val) {}
};

class IdentifierNode : public ASTNode {
public:
    std::string name;
    explicit IdentifierNode(const std::string& n) : name(n) {}
};

class StringLiteralNode : public ASTNode {
public:
    std::string value;
    explicit StringLiteralNode(const std::string& val) : value(val) {}
};

class AssignmentNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> lhs;
    std::unique_ptr<ASTNode> rhs;

    AssignmentNode(std::unique_ptr<ASTNode> lhs, std::unique_ptr<ASTNode> rhs)
        : lhs(std::move(lhs)), rhs(std::move(rhs)) {}
};

class BinaryOpNode : public ASTNode {
public:
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(const std::string& op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}
};

class UnaryOpNode : public ASTNode {
public:
    std::string op;
    std::unique_ptr<ASTNode> operand;

    UnaryOpNode(const std::string& op, std::unique_ptr<ASTNode> operand)
        : op(op), operand(std::move(operand)) {}
};
