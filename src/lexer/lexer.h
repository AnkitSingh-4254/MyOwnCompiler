#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>

enum TokenType {
    ID, NUMBER, ASSIGN, PLUS, MINUS, SEMI, EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input);

#endif // LEXER_H
