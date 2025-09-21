// src/parser/parser.cpp
// This file implements the parser, which builds an Abstract Syntax Tree (AST)
// from a stream of tokens.

#include <iostream>
#include "parser.h"

// Forward declarations
static std::unique_ptr<ParserASTNode> parseExpression(const std::vector<Token>& tokens, size_t& currentTokenIndex);
static std::unique_ptr<ParserASTNode> parseStatement(const std::vector<Token>& tokens, size_t& currentTokenIndex);

// Main parsing function.
// It parses a single statement or expression and returns its AST.
std::unique_ptr<ParserASTNode> parse(const std::vector<Token>& tokens) {
    size_t currentTokenIndex = 0;
    auto node = parseStatement(tokens, currentTokenIndex);
    return node;
}

// Parse a single statement: either assignment (ID = expr ;) or a plain expression (; optional)
static std::unique_ptr<ParserASTNode> parseStatement(const std::vector<Token>& tokens, size_t& i) {
    if (i < tokens.size()) {
        // Lookahead for simple assignment: ID '='
        if (i + 1 < tokens.size() && tokens[i].type == ID && tokens[i + 1].type == ASSIGN) {
            auto left = std::make_unique<ParserASTNode>(ParserASTNode::ID, tokens[i].value);
            i += 2; // consume ID and '='
            auto right = parseExpression(tokens, i);
            if (!right) {
                std::cerr << "Parser Error: Missing right-hand side of assignment." << std::endl;
                return nullptr;
            }
            auto assign = std::make_unique<ParserASTNode>(ParserASTNode::BINARY_OP, "=");
            assign->children.push_back(std::move(left));
            assign->children.push_back(std::move(right));
            if (i < tokens.size() && tokens[i].type == SEMI) { ++i; } // optional ';'
            return std::move(assign);
        }
    }
    // Fallback to plain expression
    auto expr = parseExpression(tokens, i);
    if (i < tokens.size() && tokens[i].type == SEMI) { ++i; }
    return expr;
}

// Simple expression parser for basic arithmetic (left term, then optional +/- rhs).
static std::unique_ptr<ParserASTNode> parseExpression(const std::vector<Token>& tokens, size_t& currentTokenIndex) {
    if (currentTokenIndex >= tokens.size()) {
        return nullptr;
    }

    std::unique_ptr<ParserASTNode> left = nullptr;

    // Handle number or ID literals.
    if (tokens[currentTokenIndex].type == NUMBER) {
        left = std::make_unique<ParserASTNode>(ParserASTNode::NUMBER_LITERAL, tokens[currentTokenIndex].value);
        currentTokenIndex++;
    } else if (tokens[currentTokenIndex].type == ID) {
        left = std::make_unique<ParserASTNode>(ParserASTNode::ID, tokens[currentTokenIndex].value);
        currentTokenIndex++;
    } else {
        std::cerr << "Parser Error: Unexpected token type at index " << currentTokenIndex << std::endl;
        return nullptr;
    }

    // Check for a binary operator (+ or - for now).
    if (currentTokenIndex < tokens.size() && (tokens[currentTokenIndex].type == PLUS || tokens[currentTokenIndex].type == MINUS)) {
        Token opToken = tokens[currentTokenIndex];
        currentTokenIndex++;

        // Recursively parse the right side of the expression.
        std::unique_ptr<ParserASTNode> right = parseExpression(tokens, currentTokenIndex);

        if (right) {
            auto binaryOpNode = std::make_unique<ParserASTNode>(ParserASTNode::BINARY_OP, opToken.value);
            binaryOpNode->children.push_back(std::move(left));
            binaryOpNode->children.push_back(std::move(right));
            return binaryOpNode;
        } else {
            std::cerr << "Parser Error: Missing right-hand side of binary operation." << std::endl;
            return nullptr;
        }
    }

    return left;
}
