// src/lexer/lexer.cpp
// This file implements the lexical analysis functionality.

#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

// Include the header file to access Token and TokenType
#include "lexer.h"

// Converts a TokenType enum to a readable string.
std::string tokenTypeToStr(TokenType type) {
    switch (type) {
        case NUMBER: return "NUMBER";
        case ID: return "ID";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MULT: return "MULT";
        case DIV: return "DIV";
        case ASSIGN: return "ASSIGN";
        case SEMI: return "SEMI";
        default: return "UNKNOWN";
    }
}

// Tokenizes the input string based on a regular expression.
std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    // This regex pattern matches numbers, identifiers, and common operators.
    std::regex tokenPattern(R"(\d+|[a-zA-Z_][a-zA-Z0-9_]*|[+\-*/=;])");
    auto words_begin = std::sregex_iterator(input.begin(), input.end(), tokenPattern);
    auto words_end = std::sregex_iterator();

    for (auto i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string val = match.str();
        if (std::regex_match(val, std::regex(R"(\d+)"))) {
            tokens.push_back({NUMBER, val});
        } else if (std::regex_match(val, std::regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"))) {
            tokens.push_back({ID, val});
        } else if (val == "+") tokens.push_back({PLUS, val});
        else if (val == "-") tokens.push_back({MINUS, val});
        else if (val == "*") tokens.push_back({MULT, val});
        else if (val == "/") tokens.push_back({DIV, val});
        else if (val == "=") tokens.push_back({ASSIGN, val});
        else if (val == ";") tokens.push_back({SEMI, val});
        else tokens.push_back({UNKNOWN, val});
    }

    return tokens;
}
