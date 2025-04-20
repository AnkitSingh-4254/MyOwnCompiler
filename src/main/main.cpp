#include<iostream>
using namespace std ;




#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h"
#include "codegen/codegen.h"

int main() {
    // Tokenize the input code
    string inputCode = "int x = 5 + 3;";
    vector<Token> tokens = tokenize(inputCode);

    // Parse the tokens into an AST
    ASTNode* astRoot = parse(tokens);

    // Perform semantic checking
    if (semanticCheck(astRoot)) {
        cout << "Semantic check passed." << endl;

        // Generate intermediate code
        vector<Quadruple> intermediateCode = generateIntermediateCode(astRoot);

        // Generate final code
        generateCode(intermediateCode);
    } else {
        cout << "Semantic check failed." << endl;
    }

    return 0;
}
