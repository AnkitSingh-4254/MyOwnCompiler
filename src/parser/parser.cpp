#include <iostream>
#include <vector>
#include <string>
#include "../lexer/lexer.h"
#include "parser.h"

using namespace std;

// === Global variables ===
vector<Token> tokens;
int current = 0;

// === Utility functions ===
Token peek() {
    return tokens[current];
}

Token advance() {
    return tokens[current++];
}

bool match(TokenType type) {
    if (current < tokens.size() && tokens[current].type == type) {
        advance();
        return true;
    }
    return false;
}

// === Grammar functions ===

bool parseTerm() {
    return match(NUMBER) || match(ID);
}

bool parseExpr() {
    if (!parseTerm()) return false;
    while (match(PLUS) || match(MINUS)) {
        if (!parseTerm()) return false;
    }
    return true;
}

bool parseType() {
    if (peek().type == ID &&
        (peek().value == "int" || peek().value == "float" || peek().value == "char")) {
        advance();
        return true;
    }
    return false;
}

bool parseStmt() {
    if (!parseType()) return false;
    if (!match(ID)) return false;
    if (!match(ASSIGN)) return false;
    if (!parseExpr()) return false;
    if (!match(SEMI)) return false;
    return true;
}

bool parseProgram() {
    while (current < tokens.size()) {
        if (!parseStmt()) return false;
    }
    return true;
}

// === Parse entry point ===
ASTNode* parse(const vector<Token>& tokensVec) {
    tokens = tokensVec;
    current = 0;

    if (parseProgram()) {
        ASTNode* root = new ASTNode{"Program", "", {}};
        return root;
    } else {
        cerr << "Syntax Error!" << endl;
        return nullptr;
    }
}
