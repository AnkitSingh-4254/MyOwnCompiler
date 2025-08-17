#ifndef PARSER_H
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
