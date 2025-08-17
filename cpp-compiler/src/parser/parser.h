#pragma once

#include <vector>
#include <memory>
#include "lexer.h"
#include "ast.h"

// Forward declarations for top-level AST nodes if needed
class ASTNode;
class ProgramNode;

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    // Entry point for parsing
    std::unique_ptr<ProgramNode> parseProgram();

private:
    const Token& peek() const;
    const Token& previous() const;
    const Token& advance();
    const Token& consume(TokenType expected, const std::string& errorMessage);
    bool match(TokenType type);
    bool matchOperator(const std::string& op);
    bool check(TokenType type) const;
    bool isAtEnd() const;

    void synchronize(); // error recovery

    // Parser rule helpers (suggested for a toy C-like language)
    std::unique_ptr<ASTNode> parseDeclaration();
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseAssignment();
    std::unique_ptr<ASTNode> parseEquality();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parsePrimary();

    std::vector<Token> tokens;
    size_t current = 0;
};
