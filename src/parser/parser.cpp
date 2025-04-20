#include <iostream>
#include <vector>
#include <string>
#include "/Users/ankitsingh/Desktop/MyOwnCompiler/src/lexer/lexer.h"
#include "/Users/ankitsingh/Desktop/MyOwnCompiler/src/parser/parser.h"

using namespace std;

// Global variables
extern vector<Token> tokens;  // Declared in lexer.cpp
extern int current;           // Declared in lexer.cpp

// Grammar rule functions

bool parseTerm() {
    if (match(NUMBER) || match(ID)) {
        return true;
    }
    return false;
}

bool parseExpr() {
    if (!parseTerm()) return false;

    while (match(PLUS) || match(MINUS)) {
        if (!parseTerm()) return false;
    }
    return true;
}

bool parseType() {
    // For now, just treat ID with value "int"/"float"/"char" as type
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

// Main parsing function
ASTNode* parse(const std::vector<Token>& tokensVec) {
    tokens = tokensVec;
    current = 0;

    if (parseProgram()) {
        ASTNode* root = new ASTNode{"Program", "", {}};
        // For now, just return a placeholder root node
        return root;
    } else {
        std::cerr << "Syntax Error!" << std::endl;
        return nullptr;
    }
}

// === For testing: mock lexer output ===

int main() {
    std::string input = "int x = 5 + 3;";
    std::vector<Token> tokens = tokenize(input);  // ✅ Now 'tokens' is declared

    ASTNode* root = parse(tokens);  // ✅ Works now
    // (You can do something with 'root' here)

    return 0;
}
