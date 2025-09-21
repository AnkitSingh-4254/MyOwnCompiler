// src/lexer/lexer.h
// This header file defines the token types and the Token structure
// for the lexical analyzer.

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// Define the different types of tokens our lexer can recognize.
enum TokenType {
    NUMBER,     // E.g., 1, 10, 500
    ID,         // E.g., x, y, myVariable
    PLUS,       // '+' operator
    MINUS,      // '-' operator
    MULT,       // '*' operator
    DIV,        // '/' operator
    ASSIGN,     // '=' assignment operator
    SEMI,       // ';' statement terminator
    UNKNOWN     // Any character not recognized by the lexer
};

// Represents a single token with its type and value (lexeme).
struct Token {
    TokenType type;
    std::string value;
};

// Declares the main tokenization function.
std::vector<Token> tokenize(const std::string& input);

// Declares a helper function to convert TokenType to a string for easy printing.
std::string tokenTypeToStr(TokenType type);

#endif
