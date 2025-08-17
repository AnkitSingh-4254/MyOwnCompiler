#include "lexer.h"

#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& sourceCode)
    : source(sourceCode), start(0), current(0), line(1), column(1) {}

std::vector<Token> Lexer::tokenize() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens.emplace_back(TokenType::END_OF_FILE, "", line, column);
    return tokens;
}

bool Lexer::isAtEnd() const {
    return current >= source.length();
}

char Lexer::peek() const {
    return isAtEnd() ? '\0' : source[current];
}

char Lexer::peekNext() const {
    return (current + 1 >= source.length()) ? '\0' : source[current + 1];
}

char Lexer::advance() {
    current++;
    column++;
    return source[current - 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source[current] != expected) return false;
    current++;
    column++;
    return true;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;
            case '\n':
                advance();
                line++;
                column = 1;
                break;
            case '/':
                if (peekNext() == '/') {
                    skipComment();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

void Lexer::skipComment() {
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

void Lexer::scanToken() {
    skipWhitespace();
    if (isAtEnd()) return;

    char c = advance();
    switch (c) {
        // Punctuation
        case '(': case ')':
        case '{': case '}':
        case ';': case ',':
            addToken(TokenType::PUNCTUATION);
            break;

        // Arithmetic & comparison operators
        case '+': case '-': case '*': case '/':
            addToken(TokenType::OPERATOR);
            break;

        case '=':
            addToken(match('=') ? TokenType::OPERATOR : TokenType::OPERATOR);
            break;

        case '!':
            addToken(match('=') ? TokenType::OPERATOR : TokenType::OPERATOR);
            break;

        case '<':
            addToken(match('=') ? TokenType::OPERATOR : TokenType::OPERATOR);
            break;

        case '>':
            addToken(match('=') ? TokenType::OPERATOR : TokenType::OPERATOR);
            break;

        case '&':
            addToken(match('&') ? TokenType::OPERATOR : TokenType::OPERATOR);
            break;

        case '|':
            addToken(match('|') ? TokenType::OPERATOR : TokenType::OPERATOR);
            break;

        case '"':
            stringLiteral();
            break;

        case '\'':
            charLiteral();
            break;

        default:
            if (std::isdigit(c)) {
                number();
            } else if (std::isalpha(c) || c == '_') {
                identifier();
            } else {
                tokens.emplace_back(TokenType::UNKNOWN, std::string(1, c), line, column);
            }
            break;
    }
}

void Lexer::addToken(TokenType type) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, line, column);
}

void Lexer::identifier() {
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        advance();
    }

    std::string text = source.substr(start, current - start);
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"int", TokenType::KEYWORD},
        {"return", TokenType::KEYWORD},
        {"if", TokenType::KEYWORD},
        {"else", TokenType::KEYWORD},
        {"while", TokenType::KEYWORD},
        {"for", TokenType::KEYWORD},
        {"void", TokenType::KEYWORD},
        {"char", TokenType::KEYWORD},
        {"float", TokenType::KEYWORD},
        {"bool", TokenType::KEYWORD},
        {"true", TokenType::KEYWORD},
        {"false", TokenType::KEYWORD}
    };

    TokenType type = TokenType::IDENTIFIER;
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        type = it->second;
    }

    tokens.emplace_back(type, text, line, column);
}

void Lexer::number() {
    while (!isAtEnd() && std::isdigit(peek())) {
        advance();
    }

    // Optional: support for decimal values
    if (!isAtEnd() && peek() == '.' && std::isdigit(peekNext())) {
        advance(); // consume '.'
        while (!isAtEnd() && std::isdigit(peek())) {
            advance();
        }
    }

    std::string numStr = source.substr(start, current - start);
    tokens.emplace_back(TokenType::NUMBER, numStr, line, column);
}

void Lexer::stringLiteral() {
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\n') {
            line++;
            column = 1;
        }
        advance();
    }

    if (isAtEnd()) {
        std::cerr << "Unterminated string at line " << line << "\n";
        return;
    }

    advance(); // Consume closing quote
    std::string value = source.substr(start + 1, current - start - 2); // Exclude quotes
    tokens.emplace_back(TokenType::STRING_LITERAL, value, line, column);
}

void Lexer::charLiteral() {
    if (isAtEnd()) {
        std::cerr << "Unterminated character literal at line " << line << "\n";
        return;
    }

    char value = advance();
    if (value == '\\') {
        // Escape sequence, consume next char
        value = advance(); // e.g., 'n', 't'
    }

    if (!match('\'')) {
        std::cerr << "Unterminated character literal at line " << line << "\n";
        return;
    }

    std::string valStr = source.substr(start + 1, current - start - 2); // Without quotes
    tokens.emplace_back(TokenType::CHAR_LITERAL, valStr, line, column);
}
