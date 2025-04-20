codegen.cpp'#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Quadruple {
    string op;
    string arg1;
    string arg2;
    string result;
};

void generateCode(const vector<Quadruple>& quads) {
    for (const auto& q : quads) {
        if (q.op == "=") {
            cout << q.result << " = " << q.arg1 << endl;
        } else {
            cout << q.result << " = " << q.arg1 << " " << q.op << " " << q.arg2 << endl;
        }
    }
}



int main() {
    vector<Quadruple> quads;

    // Example input
    quads.push_back({"+", "10", "5", "t1"});
    quads.push_back({"=", "t1", "", "x"});

    generateCode(quads);

    return 0;
}
'
codegen.h'#ifndef CODEGEN_H
#define CODEGEN_H

#include <vector>
#include <string>
#include "../parser/parser.h"

struct Quadruple {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

std::vector<Quadruple> generateIntermediateCode(ASTNode* root);
void generateCode(const std::vector<Quadruple>& code);

#endif // CODEGEN_H
'
lexer.cpp'#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

using namespace std;  

enum TokenType {
    ID, NUMBER, ASSIGN, PLUS, MINUS, MULT, DIV, SEMI, UNKNOWN
};

string tokenTypeToStr(TokenType type) {
    switch(type) {
        case ID: return "ID";
        case NUMBER: return "NUMBER";
        case ASSIGN: return "ASSIGN";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MULT: return "MULT";
        case DIV: return "DIV";
        case SEMI: return "SEMI";
        default: return "UNKNOWN";
    }
}

struct Token {
    TokenType type;
    string value;
};

vector<Token> tokenize(const string& code) {
    vector<Token> tokens;

    regex tokenPatterns(
        R"((\d+)|([a-zA-Z_][a-zA-Z0-9_]*)|(\+)|(-)|(\*)|(/)|(=)|(;))"
    );

    auto words_begin = sregex_iterator(code.begin(), code.end(), tokenPatterns);
    auto words_end = sregex_iterator();

    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;
        string val = match.str();

        if (regex_match(val, regex(R"(\d+)"))) {
            tokens.push_back({NUMBER, val});
        } else if (regex_match(val, regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"))) {
            if (val == "int" || val == "float" || val == "char") {
                tokens.push_back({ID, val}); 
            } else {
                tokens.push_back({ID, val});
            }
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

int main() {
    ifstream infile("test_programs/text_input.txt");
    string code((istreambuf_iterator<char>(infile)),
                istreambuf_iterator<char>());

    vector<Token> tokens = tokenize(code);

    for (const auto& token : tokens) {
        cout << tokenTypeToStr(token.type) << ": " << token.value << endl;
    }

    return 0;
}'
lexer.h'#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

struct Token {
    std::string type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input);

#endif // LEXER_H
'
parser.cpp'#include <iostream>
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
'
parser.h'#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "../lexer/lexer.h"

struct ASTNode {
    std::string type;
    std::string value;
    std::vector<ASTNode*> children;
};

ASTNode* parse(const std::vector<Token>& tokens);

#endif // PARSER_H
'
semantic.cpp'#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <regex>

using namespace std;

// Symbol Table: variable name -> type
unordered_map<string, string> symbolTable;

// Helper to trim whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

// Function to check if a type is valid
bool isValidType(const string& type) {
    return type == "int" || type == "float" || type == "string";
}

// Function to check if a value matches a type
bool isValueTypeValid(const string& type, const string& value) {
    if (type == "int") return regex_match(value, regex("^[0-9]+$"));
    if (type == "float") return regex_match(value, regex("^[0-9]*\\.[0-9]+$"));
    if (type == "string") return regex_match(value, regex("^\".*\"$"));
    return false;
}

int main() {
    ifstream infile("test_programs/text_input.txt");
    string line;
    bool errorFound = false;

    while (getline(infile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // Check for declaration and assignment: e.g. int x = 10;
        smatch match;
        if (regex_match(line, match, regex("^(int|float|string)\\s+([a-zA-Z_][a-zA-Z0-9_]*)\\s*=\\s*(.+);$"))) {
            string type = match[1];
            string varName = match[2];
            string value = trim(match[3]);

            if (symbolTable.find(varName) != symbolTable.end()) {
                cout << "Semantic Error: Variable '" << varName << "' already declared." << endl;
                errorFound = true;
            } else if (!isValueTypeValid(type, value)) {
                cout << "Semantic Error: Type mismatch for variable '" << varName << "'." << endl;
                errorFound = true;
            } else {
                symbolTable[varName] = type;
            }
        }
        // Check for usage/assignment: e.g. x = 5;
        else if (regex_match(line, match, regex("^([a-zA-Z_][a-zA-Z0-9_]*)\\s*=\\s*(.+);$"))) {
            string varName = match[1];
            string value = trim(match[2]);

            if (symbolTable.find(varName) == symbolTable.end()) {
                cout << "Semantic Error: Variable '" << varName << "' used before declaration." << endl;
                errorFound = true;
            } else {
                string type = symbolTable[varName];
                if (!isValueTypeValid(type, value)) {
                    cout << "Semantic Error: Type mismatch for variable '" << varName << "'." << endl;
                    errorFound = true;
                }
            }
        }
        // Not recognized
        else {
            cout << "Semantic Error: Unrecognized statement: '" << line << "'" << endl;
            errorFound = true;
        }
    }

    if (!errorFound) {
        cout << "Semantic Analysis Passed!" << endl;
    }

    return 0;
}
'
semantic.h'#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../parser/parser.h"

bool semanticCheck(ASTNode* root);

#endif // SEMANTIC_H
'
main.cpp'#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h"
#include "codegen/codegen.h"

using namespace std;

int main() {
    // 1. Read input from test_programs/text_input.txt
    ifstream inputFile("test_programs/text_input.txt");
    if (!inputFile) {
        cerr << "Error: Cannot open input file!" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string input = buffer.str();

    // 2. Lexer
    vector<Token> tokens = tokenize(input);
    cout << "Tokens:\n";
    for (auto& tok : tokens) {
        cout << tok.type << ": " << tok.value << endl;
    }

    // 3. Parser
    ASTNode* root = parse(tokens);
    cout << "\nParsing successful.\n";

    // 4. Semantic Analysis
    if (!semanticCheck(root)) {
        cerr << "Semantic check failed!" << endl;
        return 1;
    }
    cout << "Semantic check passed.\n";

    // 5. Code Generation
    vector<Quadruple> code = generateIntermediateCode(root);
    generateCode(code);

    // Free memory if needed (AST cleanup)
    // TODO: Delete AST nodes properly

    return 0;
}
'
