#include "ast.h"

/**
 * AST Node Implementation
 * =======================
 * 
 * This file implements the visitor pattern methods for each AST node type.
 * The visitor pattern allows us to separate tree traversal from the operations
 * performed on each node, making it easy to add new operations (like semantic
 * analysis, code generation, optimization) without modifying the AST classes.
 */

// Program Node
void ProgramNode::accept(ASTVisitor* visitor) {
    visitor->visitProgram(this);
}

// Function Declaration Node
void FunctionDeclNode::accept(ASTVisitor* visitor) {
    visitor->visitFunctionDecl(this);
}

// Variable Declaration Node
void VariableDeclNode::accept(ASTVisitor* visitor) {
    visitor->visitVariableDecl(this);
}

// Assignment Node
void AssignmentNode::accept(ASTVisitor* visitor) {
    visitor->visitAssignment(this);
}

// Binary Operation Node
void BinaryOpNode::accept(ASTVisitor* visitor) {
    visitor->visitBinaryOp(this);
}

// Function Call Node
void FunctionCallNode::accept(ASTVisitor* visitor) {
    visitor->visitFunctionCall(this);
}

// If Statement Node
void IfStatementNode::accept(ASTVisitor* visitor) {
    visitor->visitIfStatement(this);
}

// Return Statement Node
void ReturnStatementNode::accept(ASTVisitor* visitor) {
    visitor->visitReturnStatement(this);
}

// Block Node
void BlockNode::accept(ASTVisitor* visitor) {
    visitor->visitBlock(this);
}

// Identifier Node
void IdentifierNode::accept(ASTVisitor* visitor) {
    visitor->visitIdentifier(this);
}

// Integer Literal Node
void IntegerLiteralNode::accept(ASTVisitor* visitor) {
    visitor->visitIntegerLiteral(this);
}

// Float Literal Node
void FloatLiteralNode::accept(ASTVisitor* visitor) {
    visitor->visitFloatLiteral(this);
}

/**
 * Utility function to convert DataType to string for debugging and error messages
 */
std::string dataTypeToString(DataType type) {
    switch (type) {
        case DataType::INT: return "int";
        case DataType::FLOAT: return "float";
        case DataType::CHAR: return "char";
        case DataType::STRING: return "string";
        case DataType::VOID: return "void";
        case DataType::UNKNOWN: return "unknown";
        default: return "unknown";
    }
}
