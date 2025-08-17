#include "parser.h"
#include <stdexcept>
#include <iostream>

// ---------------- Constructor ----------------
Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

// ---------------- Utility Functions ----------------
const Token& Parser::peek() const {
    return tokens[current];
}

const Token& Parser::previous() const {
    return tokens[current - 1];
}

const Token& Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::matchOperator(const std::string& op) {
    if (peek().type == TokenType::OPERATOR && peek().lexeme == op) {
        advance();
        return true;
    }
    return false;
}

const Token& Parser::consume(TokenType expected, const std::string& errorMessage) {
    if (check(expected)) return advance();
    throw std::runtime_error("Parse error at '" + peek().lexeme + "': " + errorMessage);
}

// ---------------- Error Recovery ----------------
void Parser::synchronize() {
    advance();
    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;
        switch (peek().type) {
            case TokenType::KEYWORD:
            case TokenType::IDENTIFIER:
            case TokenType::NUMBER:
                return;
            default:
                break;
        }
        advance();
    }
}

// ---------------- Parsing Entry ----------------
std::unique_ptr<ProgramNode> Parser::parseProgram() {
    auto program = std::make_unique<ProgramNode>();

    while (!isAtEnd()) {
        try {
            auto decl = parseDeclaration();
            if (decl) program->statements.push_back(std::move(decl));
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            synchronize();
        }
    }
    return program;
}

// ---------------- Grammar Rules ----------------
std::unique_ptr<ASTNode> Parser::parseDeclaration() {
    // For now, treat every top-level as a statement
    return parseStatement();
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    // Extend later for if, while, etc.
    return parseExpression();
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseAssignment();
}

std::unique_ptr<ASTNode> Parser::parseAssignment() {
    auto expr = parseEquality();

    if (matchOperator("=")) {
        auto value = parseAssignment();
        return std::make_unique<BinaryNode>("=", std::move(expr), std::move(value));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseEquality() {
    auto expr = parseComparison();

    while (matchOperator("==") || matchOperator("!=")) {
        std::string op = previous().lexeme;
        auto right = parseComparison();
        expr = std::make_unique<BinaryNode>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseComparison() {
    auto expr = parseTerm();

    while (matchOperator("<") || matchOperator(">") || matchOperator("<=") || matchOperator(">=")) {
        std::string op = previous().lexeme;
        auto right = parseTerm();
        expr = std::make_unique<BinaryNode>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    auto expr = parseFactor();

    while (matchOperator("+") || matchOperator("-")) {
        std::string op = previous().lexeme;
        auto right = parseFactor();
        expr = std::make_unique<BinaryNode>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    auto expr = parseUnary();

    while (matchOperator("*") || matchOperator("/")) {
        std::string op = previous().lexeme;
        auto right = parseUnary();
        expr = std::make_unique<BinaryNode>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseUnary() {
    if (matchOperator("!") || matchOperator("-")) {
        std::string op = previous().lexeme;
        auto right = parseUnary();
        return std::make_unique<UnaryNode>(op, std::move(right));
    }
    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (match(TokenType::NUMBER)) {
        return std::make_unique<NumberNode>(previous().lexeme);
    }

    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<IdentifierNode>(previous().lexeme);
    }

    if (match(TokenType::LEFT_PAREN)) {
        auto expr = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return expr;
    }

    throw std::runtime_error("Unexpected token: " + peek().lexeme);
}
