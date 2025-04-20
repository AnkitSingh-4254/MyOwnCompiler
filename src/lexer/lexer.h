#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum TokenType {
    NUMBER, ID, PLUS, MINUS, MULT, DIV, ASSIGN, SEMI, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input);

#endif
