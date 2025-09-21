// src/parser/parser.h
// This header file defines the structure for the Abstract Syntax Tree (AST)
// and the declaration for the parser function.

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <memory>
#include "lexer/lexer.h"

// Define a simple node for the Abstract Syntax Tree used by the parser.
// Kept minimal; later phases use shared AST under common/ast.h
struct ParserASTNode {
    enum NodeType {
        EXPRESSION,
        BINARY_OP,
        NUMBER_LITERAL,
        ID
    };

    NodeType type;
    std::string value;
    std::vector<std::unique_ptr<ParserASTNode>> children;

    ParserASTNode(NodeType type, const std::string& value) : type(type), value(value) {}
    virtual ~ParserASTNode() = default;
};

// Declares the main parsing function.
// It will take a vector of tokens and build an AST for a single statement/expression.
std::unique_ptr<ParserASTNode> parse(const std::vector<Token>& tokens);

#endif
