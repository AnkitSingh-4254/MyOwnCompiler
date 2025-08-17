// === MyOwnCompiler — Minimal End‑to‑End (header‑only) ===
// Files below are header-only so you can compile main.cpp quickly.
// Supports: identifiers, integers, assignments, +, *, ; and multi-line input.
// Extend later to add types, control flow, etc.

/* =========================
   File: lexer.h
   ========================= */
#pragma once
#include <string>
#include <vector>
#include <cctype>

enum class TokenType {
    Identifier, Number,
    Assign, Plus, Star, Semicolon,
    End,
};

struct Token {
    TokenType type{};
    std::string lexeme;
    int line = 1;
    int col = 1;
};

class Lexer {
    std::string src;
    size_t i = 0; int line = 1, col = 1;
public:
    explicit Lexer(const std::string& s) : src(s) {}

    std::vector<Token> tokenize() {
        std::vector<Token> out;
        while (true) {
            skipWS();
            if (i >= src.size()) { out.push_back({TokenType::End, "", line, col}); break; }
            char c = src[i];
            if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                size_t start = i; int l=line, cc=col;
                while (i<src.size() && (std::isalnum(static_cast<unsigned char>(src[i])) || src[i]=='_')) advance();
                out.push_back({TokenType::Identifier, src.substr(start, i-start), l, cc});
            } else if (std::isdigit(static_cast<unsigned char>(c))) {
                size_t start = i; int l=line, cc=col;
                while (i<src.size() && std::isdigit(static_cast<unsigned char>(src[i]))) advance();
                out.push_back({TokenType::Number, src.substr(start, i-start), l, cc});
            } else {
                int l=line, cc=col;
                switch (c) {
                    case '=': advance(); out.push_back({TokenType::Assign, "=", l, cc}); break;
                    case '+': advance(); out.push_back({TokenType::Plus, "+", l, cc}); break;
                    case '*': advance(); out.push_back({TokenType::Star, "*", l, cc}); break;
                    case ';': advance(); out.push_back({TokenType::Semicolon, ";", l, cc}); break;
                    default: // skip unknowns
                        advance(); break;
                }
            }
        }
        return out;
    }
private:
    void advance(){ if (i<src.size()) { if (src[i]=='\n'){line++; col=1;} else col++; i++; } }
    void skipWS(){ while (i<src.size() && std::isspace(static_cast<unsigned char>(src[i]))) advance(); }
};

/* =========================
   File: parser.h
   ========================= */
#pragma once
#include <memory>
#include <stdexcept>

struct Expr { virtual ~Expr()=default; };
struct NumberExpr : Expr { int value; explicit NumberExpr(int v):value(v){} };
struct VarExpr    : Expr { std::string name; explicit VarExpr(std::string n):name(std::move(n)){} };
struct BinaryExpr : Expr {
    char op; std::unique_ptr<Expr> lhs, rhs;
    BinaryExpr(char o, std::unique_ptr<Expr> L, std::unique_ptr<Expr> R)
        : op(o), lhs(std::move(L)), rhs(std::move(R)) {}
};

struct Stmt { virtual ~Stmt()=default; };
struct AssignStmt : Stmt { std::string name; std::unique_ptr<Expr> value; };

struct ProgramAST { std::vector<std::unique_ptr<Stmt>> stmts; };

class Parser {
    const std::vector<Token>& toks; size_t p=0;
public:
    explicit Parser(const std::vector<Token>& t):toks(t){}

    ProgramAST parseProgram(){
        ProgramAST prog; 
        while (!check(TokenType::End)) {
            prog.stmts.push_back(parseStmt());
        }
        return prog;
    }
private:
    bool check(TokenType t) const { return toks[p].type==t; }
    const Token& peek() const { return toks[p]; }
    const Token& advance(){ if (!check(TokenType::End)) p++; return toks[p-1]; }
    const Token& expect(TokenType t, const char* what){ if (!check(t)) error(std::string("expected ")+what); return advance(); }
    [[noreturn]] void error(const std::string& msg) const { throw std::runtime_error(msg+" at line "+std::to_string(peek().line)); }

    std::unique_ptr<Stmt> parseStmt(){
        // assignment: Identifier '=' expr ';'
        if (!check(TokenType::Identifier)) error("statement must start with identifier");
        auto name = peek().lexeme; advance();
        expect(TokenType::Assign, "'='");
        auto value = parseExpr();
        expect(TokenType::Semicolon, "';'");
        auto s = std::make_unique<AssignStmt>(); s->name = name; s->value = std::move(value); return s;
    }

    // E -> T ( '+' T )*
    std::unique_ptr<Expr> parseExpr(){
        auto lhs = parseTerm();
        while (check(TokenType::Plus)) { advance(); auto rhs = parseTerm(); lhs = std::make_unique<BinaryExpr>('+', std::move(lhs), std::move(rhs)); }
        return lhs;
    }
    // T -> P ( '*' P )*
    std::unique_ptr<Expr> parseTerm(){
        auto lhs = parsePrimary();
        while (check(TokenType::Star)) { advance(); auto rhs = parsePrimary(); lhs = std::make_unique<BinaryExpr>('*', std::move(lhs), std::move(rhs)); }
        return lhs;
    }
    // P -> Number | Identifier
    std::unique_ptr<Expr> parsePrimary(){
        if (check(TokenType::Number)) { int v = std::stoi(peek().lexeme); advance(); return std::make_unique<NumberExpr>(v); }
        if (check(TokenType::Identifier)) { auto n = peek().lexeme; advance(); return std::make_unique<VarExpr>(n); }
        error("expected number or identifier");
    }
};

/* =========================
   File: semantic.h
   ========================= */
#pragma once
#include <unordered_map>
#include <string>

class SymbolTable {
    std::unordered_map<std::string,int> table;
public:
    bool declared(const std::string& n) const { return table.count(n)>0; }
    void set(const std::string& n, int v){ table[n]=v; }
    int get(const std::string& n) const { auto it=table.find(n); return it==table.end()?0:it->second; }
};

class SemanticAnalyzer {
    std::vector<std::string> errors; SymbolTable sym;
public:
    void analyze(const ProgramAST& prog){ (void)prog; /* trivial for now */ }
    const std::vector<std::string>& getErrors() const { return errors; }
    SymbolTable& getSymbolTable(){ return sym; }
};

/* =========================
   File: codegen.h
   ========================= */
#pragma once
#include <string>

enum class OpCode { LDC=0, LDV=1, ADD=2, MUL=3, STV=4 };

struct IRInstr {
    OpCode opcode{}; std::string operand1; std::string operand2; // strings for easy printing
};

class CodeGenerator {
    std::vector<IRInstr> ir;
    SymbolTable* sym = nullptr;
public:
    void setSymbolTable(SymbolTable* s){ sym = s; }

    void generate(const ProgramAST& prog){
        for (const auto& st : prog.stmts) genStmt(*st);
    }
    const std::vector<IRInstr>& getInstructions() const { return ir; }
private:
    void genStmt(const Stmt& s){
        if (auto as = dynamic_cast<const AssignStmt*>(&s)) {
            genExpr(*as->value);
            ir.push_back({OpCode::STV, as->name, ""});
        }
    }
    void genExpr(const Expr& e){
        if (auto n = dynamic_cast<const NumberExpr*>(&e)) {
            ir.push_back({OpCode::LDC, std::to_string(n->value), ""});
        } else if (auto v = dynamic_cast<const VarExpr*>(&e)) {
            ir.push_back({OpCode::LDV, v->name, ""});
        } else if (auto b = dynamic_cast<const BinaryExpr*>(&e)) {
            genExpr(*b->lhs); genExpr(*b->rhs);
            if (b->op=='+') ir.push_back({OpCode::ADD, "", ""});
            else if (b->op=='*') ir.push_back({OpCode::MUL, "", ""});
        }
    }
};

/* =========================
   File: assembler.h
   ========================= */
#pragma once

enum class VMOp { PUSH=0, LOAD=1, ADD=2, MUL=3, STORE=4 };

struct VMInstr { VMOp opcode{}; std::string operand1; std::string operand2; };

class Assembler {
    std::vector<VMInstr> code;
public:
    void assemble(const std::vector<IRInstr>& ir){
        code.clear();
        for (const auto& ins : ir) {
            switch (ins.opcode) {
                case OpCode::LDC:  code.push_back({VMOp::PUSH, ins.operand1, ""}); break;
                case OpCode::LDV:  code.push_back({VMOp::LOAD, ins.operand1, ""}); break;
                case OpCode::ADD:  code.push_back({VMOp::ADD,  "", ""}); break;
                case OpCode::MUL:  code.push_back({VMOp::MUL,  "", ""}); break;
                case OpCode::STV:  code.push_back({VMOp::STORE, ins.operand1, ""}); break;
            }
        }
    }
    const std::vector<VMInstr>& getVMInstructions() const { return code; }
};

/* =========================
   File: vm.h
   ========================= */
#pragma once
#include <unordered_map>
#include <stack>
#include <iostream>

class VirtualMachine {
    std::vector<VMInstr> program;
    std::stack<int> st;
    std::unordered_map<std::string,int> vars;
public:
    void execute(const std::vector<VMInstr>& code){
        program = code;
        for (const auto& ins : program) {
            switch (ins.opcode) {
                case VMOp::PUSH: st.push(std::stoi(ins.operand1)); break;
                case VMOp::LOAD: st.push(getVariable(ins.operand1)); break;
                case VMOp::ADD:  { int b=pop(), a=pop(); st.push(a+b); } break;
                case VMOp::MUL:  { int b=pop(), a=pop(); st.push(a*b); } break;
                case VMOp::STORE:{ int v=pop(); vars[ins.operand1]=v; } break;
            }
        }
    }

    int getVariable(const std::string& name) const {
        auto it = vars.find(name); return it==vars.end()?0:it->second;
    }
private:
    int pop(){ int v=st.top(); st.pop(); return v; }
};

/* =========================
   File: main.cpp (your snippet)
   ========================= */
// #include <iostream>
// #include <string>
// #include <fstream>
// #include <sstream>
//
// #include "lexer.h"
// #include "parser.h"
// #include "semantic.h"
// #include "codegen.h"
// #include "assembler.h"
// #include "vm.h"
//
// int main() {
//     std::string sourceCode = "a = 5 + 3;\nb = a * 2;";
//
//     Lexer lexer(sourceCode);
//     auto tokens = lexer.tokenize();
//
//     std::cout << "[Tokens]" << std::endl;
//     for (const auto& token : tokens) {
//         std::cout << token.lexeme << "  [" << static_cast<int>(token.type) << "]\n";
//     }
//
//     Parser parser(tokens);
//     auto ast = parser.parseProgram();
//
//     SemanticAnalyzer sema;
//     sema.analyze(ast);
//     const auto& errors = sema.getErrors();
//     if (!errors.empty()) {
//         std::cerr << "\nSemantic Errors:\n";
//         for (const auto& err : errors) std::cerr << "  ➜ " << err << std::endl;
//         return 1;
//     }
//
//     CodeGenerator codegen; codegen.setSymbolTable(&sema.getSymbolTable());
//     codegen.generate(ast);
//     const auto& ir = codegen.getInstructions();
//
//     std::cout << "\n[Intermediate Code]\n";
//     for (const auto& instr : ir) {
//         std::cout << static_cast<int>(instr.opcode) << " " << instr.operand1 << " " << instr.operand2 << "\n";
//     }
//
//     Assembler assembler; assembler.assemble(ir); const auto& vmCode = assembler.getVMInstructions();
//     std::cout << "\n[VM Instructions]\n";
//     for (const auto& instr : vmCode) {
//         std::cout << static_cast<int>(instr.opcode) << " " << instr.operand1 << " " << instr.operand2 << "\n";
//     }
//
//     VirtualMachine vm; vm.execute(vmCode);
//     std::cout << "\n[Final State]\n";
//     std::cout << "a = " << vm.getVariable("a") << "\n";
//     std::cout << "b = " << vm.getVariable("b") << "\n";
//     return 0;
// }
