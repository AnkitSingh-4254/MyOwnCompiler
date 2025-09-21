#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Include headers for the lexer and parser.
#include "lexer/lexer.h"
#include "parser/parser.h"

// Include shared AST and later phases
#include "common/ast.h"
#include "semantic/semantic.h"
#include "codegen/codegen.h"
#include "assembler/assembler.h"
#include "vm.h"

// Convert parser-local AST (ParserASTNode) to shared AST (common/ast.h)
static std::unique_ptr<ASTNode> convertToSharedAST(const ParserASTNode* node) {
    if (!node) return nullptr;
    using NT = ParserASTNode::NodeType;
    switch (node->type) {
        case NT::NUMBER_LITERAL:
            return std::make_unique<NumberLiteralNode>(node->value);
        case NT::ID:
            return std::make_unique<IdentifierNode>(node->value);
        case NT::BINARY_OP: {
            // Expect two children
            if (node->children.size() != 2) return nullptr;
            const auto* left = node->children[0].get();
            const auto* right = node->children[1].get();
            if (node->value == "=") {
                auto lhs = convertToSharedAST(left);
                auto rhs = convertToSharedAST(right);
                return std::make_unique<AssignmentNode>(std::move(lhs), std::move(rhs));
            }
            auto lhs = convertToSharedAST(left);
            auto rhs = convertToSharedAST(right);
            return std::make_unique<BinaryOpNode>(node->value, std::move(lhs), std::move(rhs));
        }
        case NT::EXPRESSION:
        default:
            // Treat as transparent wrapper
            if (!node->children.empty()) {
                return convertToSharedAST(node->children[0].get());
            }
            return nullptr;
    }
}

int main() {
    // A simple test program to demonstrate the full pipeline.
    // Keep it simple to match current parser support (assignment and +/- expressions).
    std::string inputCode = "x = 5 + 3;";

    // 1) Lexical Analysis
    std::cout << "Starting Lexical Analysis..." << std::endl;
    std::vector<Token> tokens = tokenize(inputCode);
    std::cout << "Lexical analysis complete. Tokens:" << std::endl;
    for (const auto& token : tokens) {
        std::cout << "  " << tokenTypeToStr(token.type) << ": " << token.value << std::endl;
    }
    std::cout << std::endl;

    // 2) Parsing (parser-local AST)
    std::cout << "Starting Parsing..." << std::endl;
    std::unique_ptr<ParserASTNode> parseAst = parse(tokens);
    if (!parseAst) {
        std::cerr << "Parsing failed." << std::endl;
        return 1;
    }
    std::cout << "Parsing complete." << std::endl;

    // 3) Convert to shared AST and wrap in ProgramNode
    auto program = std::make_unique<ProgramNode>();
    auto stmt = convertToSharedAST(parseAst.get());
    if (!stmt) {
        std::cerr << "AST conversion failed." << std::endl;
        return 1;
    }
    program->statements.push_back(std::move(stmt));
    std::unique_ptr<ASTNode> programRoot = std::move(program);

    // 4) Semantic analysis
    SemanticAnalyzer sema;
    sema.analyze(programRoot);
    const auto& errors = sema.getErrors();
    if (!errors.empty()) {
        std::cerr << "Semantic errors (" << errors.size() << "):\n";
        for (const auto& e : errors) std::cerr << "  - " << e << "\n";
        return 1;
    }

    // 5) IR code generation
    CodeGenerator codegen;
    codegen.setSymbolTable(&sema.getSymbolTable());
    codegen.generate(programRoot);

    // 6) Assemble IR to VM instructions
    Assembler assembler;
    assembler.assemble(codegen.getInstructions());

    // 7) Execute on the VM
    VirtualMachine vm;
    vm.execute(assembler.getVMInstructions());

    // Report a simple result
    try {
        int xVal = vm.getVariable("x");
        std::cout << "Execution complete. x = " << xVal << std::endl;
    } catch (const std::exception& ex) {
        std::cout << "Execution complete. (No variable 'x' found)" << std::endl;
    }

    return 0;
}
