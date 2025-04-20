#include "lexer.h"
#include <regex>
#include <iostream>

using namespace std;

vector<Token> tokenize(const string& input) {
    vector<Token> tokens;
    regex tokenPattern(R"(\d+|[a-zA-Z_][a-zA-Z0-9_]*|[+\-*/=;])");
    auto words_begin = sregex_iterator(input.begin(), input.end(), tokenPattern);
    auto words_end = sregex_iterator();

    for (auto i = words_begin; i != words_end; ++i) {
        smatch match = *i;
        string val = match.str();
        if (regex_match(val, regex(R"(\d+)"))) {
            tokens.push_back({NUMBER, val});
        } else if (regex_match(val, regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"))) {
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
