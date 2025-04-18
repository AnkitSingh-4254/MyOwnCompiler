#include <iostream>
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
}
