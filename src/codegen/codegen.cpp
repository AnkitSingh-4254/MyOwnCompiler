#include <iostream>
#include <vector>
#include <string>
#include "../parser/parser.h"

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

vector<Quadruple> generateIntermediateCode(ASTNode* root) {
    // Placeholder logic for generating intermediate code from AST
    vector<Quadruple> quads;
    // Example: A simple statement `int x = 5 + 3;`
    quads.push_back({"+", "5", "3", "t1"});
    quads.push_back({"=", "t1", "", "x"});
    return quads;
}

// int main() {
    // vector<Quadruple> quads;

    // // Example input
    // quads.push_back({"+", "10", "5", "t1"});
    // quads.push_back({"=", "t1", "", "x"});

    // generateCode(quads);

    // return 0;
// }
