#pragma once
#include <string>
#include <vector>

enum class TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, WHITESPACE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

std::string markdownToHtml(const std::string& markdown);
std::string highlight(const std::vector<Token>& tokens);
std::vector<Token> lex(const std::string& code);
