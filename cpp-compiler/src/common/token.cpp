#include "token.h"

/**
 * Utility function to convert TokenType to string for debugging
 */
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING: return "STRING";
        case TokenType::CHARACTER: return "CHARACTER";
        case TokenType::INT: return "INT";
        case TokenType::FLOAT_KW: return "FLOAT_KW";
        case TokenType::CHAR: return "CHAR";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::FOR: return "FOR";
        case TokenType::RETURN: return "RETURN";
        case TokenType::MAIN: return "MAIN";
        case TokenType::PRINT: return "PRINT";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::GREATER: return "GREATER";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}