#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

/**
 * Token Types - Different categories of tokens that our lexer can recognize
 */
enum class TokenType {
    // Literals
    INTEGER,        // 123, 456
    FLOAT,          // 3.14, 2.5
    IDENTIFIER,     // variable names, function names
    STRING,         // "hello world"
    CHARACTER,      // 'a', 'b'
    
    // Keywords
    INT,            // int
    FLOAT_KW,       // float
    CHAR,           // char
    IF,             // if
    ELSE,           // else
    WHILE,          // while
    FOR,            // for
    RETURN,         // return
    MAIN,           // main
    PRINT,          // print (custom function)
    
    // Operators
    PLUS,           // +
    MINUS,          // -
    MULTIPLY,       // *
    DIVIDE,         // /
    ASSIGN,         // =
    EQUAL,          // ==
    NOT_EQUAL,      // !=
    LESS,           // <
    GREATER,        // >
    LESS_EQUAL,     // <=
    GREATER_EQUAL,  // >=
    
    // Delimiters
    SEMICOLON,      // ;
    COMMA,          // ,
    LPAREN,         // (
    RPAREN,         // )
    LBRACE,         // {
    RBRACE,         // }
    
    // Special
    END_OF_FILE,    // EOF
    NEWLINE,        // \n
    UNKNOWN         // Unknown character
};

/**
 * Token Structure - Represents a single token from the source code
 */
struct Token {
    TokenType type;     // Type of the token
    std::string value;  // String representation of the token
    int line;          // Line number where token appears
    int column;        // Column number where token appears
    
    // Constructor
    Token(TokenType t, const std::string& v, int l, int c) 
        : type(t), value(v), line(l), column(c) {}
    
    // Default constructor
    Token() : type(TokenType::UNKNOWN), value(""), line(0), column(0) {}
};

// Forward declaration - implementation should be in a separate .cpp file
std::string tokenTypeToString(TokenType type);

#endif // TOKEN_H