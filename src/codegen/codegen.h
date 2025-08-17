#ifndef CODEGEN_H
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
