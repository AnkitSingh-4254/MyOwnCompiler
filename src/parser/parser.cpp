#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Token types (same as lexer)
enum TokenType {
    ID, NUMBER, ASSIGN, PLUS, MINUS, MULT, DIV, SEMI, UNKNOWN
};

struct Token {
    TokenType type;
    string value;
};

// Global variables
vector<Token> tokens;
int current = 0;

// Function to get current token
Token peek() {
    return tokens[current];
}

// Function to consume a token and move ahead
Token advance() {
    return tokens[current++];
}

// Match and consume a token of expected type
bool match(TokenType type) {
    if (current < tokens.size() && tokens[current].type == type) {
        advance();
        return true;
    }
    return false;
}

// === Grammar rules ===

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

// === For testing: mock lexer output ===
int main() {
    // Simulate lexer output: int x = 5 + 3;
    tokens = {
        {ID, "int"}, {ID, "x"}, {ASSIGN, "="},
        {NUMBER, "5"}, {PLUS, "+"}, {NUMBER, "3"}, {SEMI, ";"}
    };

    if (parseProgram()) {
        cout << " Parsing Successful!" << endl;
    } else {
        cout << " Syntax Error!" << endl;
    }

    return 0;
}
