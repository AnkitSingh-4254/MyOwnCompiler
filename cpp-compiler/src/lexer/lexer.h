#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cctype>
#include <iostream>

// Token types for your language
enum class TokenType {
    IDENTIFIER,
    KEYWORD,
    NUMBER,
    OPERATOR,
    PUNCTUATION,
    STRING_LITERAL,
    CHAR_LITERAL,
    END_OF_FILE,
    UNKNOWN
};

// Struct representing a single token
struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;

    Token(TokenType type, const std::string& lexeme, int line = 0, int column = 0)
        : type(type), lexeme(lexeme), line(line), column(column) {}
};

class Lexer {
public:
    explicit Lexer(const std::string& sourceCode);
    std::vector<Token> tokenize();

private:
    char peek() const;
    char peekNext() const;
    char advance();
    bool match(char expected);
    bool isAtEnd() const;

    void skipWhitespace();
    void skipComment();

    void scanToken();
    void identifier();
    void number();
    void stringLiteral();
    void charLiteral();
    void addToken(TokenType type);

    std::string source;
    std::vector<Token> tokens;
    size_t start = 0;
    size_t current = 0;
    int line = 1;
    int column = 1;
};

