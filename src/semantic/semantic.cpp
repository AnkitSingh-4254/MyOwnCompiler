#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <regex>
#include "../parser/parser.h"

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

bool semanticCheck(ASTNode* root) {
    // Placeholder logic for semantic analysis
    // For now, just return true
    return true;
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

            if (isValidType(type)) {
                if (!isValueTypeValid(type, value)) {
                    cerr << "Error: Type mismatch in assignment " << varName << " = " << value << endl;
                    errorFound = true;
                }
            } else {
                cerr << "Error: Invalid type " << type << endl;
                errorFound = true;
            }
        }
    }

    if (errorFound) {
        cout << "Semantic analysis found errors!" << endl;
    } else {
        cout << "Semantic analysis completed successfully!" << endl;
    }

    return 0;
}
