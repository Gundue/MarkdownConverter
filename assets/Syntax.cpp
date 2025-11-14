#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>

enum class TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, WHITESPACE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

const std::unordered_set<std::string> KEYWORDS = {
    "for", "if", "else", "while",
    "switch", "case", "break", "continue", "return",
    "int", "float", "double", "char", "void",
    "struct", "class", "public", "private", "protected",
    "auto"
};

bool isOperator(char c) {
    return std::string("+-*/=%<>!&|:").find(c) != std::string::npos;
}

std::vector<Token> lex(const std::string& code) {
    std::vector<Token> tokens;
    int i = 0;

    while (i < code.size()) {
        char c = code[i];

        // WHITESPACE
        if (isspace(c)) {
            int start = i;
            while (i < code.size() && isspace(code[i])) i++;
            tokens.push_back({TokenType::WHITESPACE, code.substr(start, i - start)});
        }

        // IDENTIFIER or KEYWORD
        else if (isalpha(c) || c == '_') {
            int start = i;
            while (i < code.size() && (isalnum(code[i]) || code[i] == '_')) i++;
            std::string word = code.substr(start, i - start);

            tokens.push_back({
                KEYWORDS.count(word) ? TokenType::KEYWORD : TokenType::IDENTIFIER,
                word
            });
        }

        // NUMBER
        else if (isdigit(c)) {
            int start = i;
            while (i < code.size() && isdigit(code[i])) i++;
            tokens.push_back({TokenType::NUMBER, code.substr(start, i - start)});
        }

        // STRING
        else if (c == '"') {
            int start = i++;
            while (i < code.size() && code[i] != '"') i++;
            i++; // closing "
            tokens.push_back({TokenType::STRING, code.substr(start, i - start)});
        }

        // OPERATOR
        else if (isOperator(c)) {
            tokens.push_back({TokenType::OPERATOR, std::string(1, c)});
            i++;
        }

        else {
            tokens.push_back({TokenType::UNKNOWN, std::string(1, c)});
            i++;
        }
    }
    return tokens;
}

// HTML 색칠
std::string highlight(const std::vector<Token>& tokens) {
    std::string html;

    for (auto& t : tokens) {
        switch (t.type) {
            case TokenType::KEYWORD:
                html += "<span style='color: blue; font-weight:bold'>" + t.value + "</span>";
                break;
            case TokenType::NUMBER:
                html += "<span style='color: red'>" + t.value + "</span>";
                break;
            case TokenType::STRING:
                html += "<span style='color: green'>" + t.value + "</span>";
                break;
            case TokenType::OPERATOR:
                html += "<span style='color: purple'>" + t.value + "</span>";
                break;
            default:
                html += t.value;
        }
    }
    return html;
}

#include <iostream>

int main() {
    std::string code = R"(for (auto& t : tokens) {
        switch (t.type) {
            case TokenType::KEYWORD:
                html += "<span style='color: blue; font-weight:bold'>" + t.value + "</span>";
                break;
            case TokenType::NUMBER:
                html += "<span style='color: red'>" + t.value + "</span>";
                break;
            case TokenType::STRING:
                html += "<span style='color: green'>" + t.value + "</span>";
                break;
            case TokenType::OPERATOR:
                html += "<span style='color: purple'>" + t.value + "</span>";
                break;
            default:
                html += t.value;
        }
    })";

    auto tokens = lex(code);
    std::string html = highlight(tokens);

    std::cout << html << std::endl;
}
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>

enum class TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, WHITESPACE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

const std::unordered_set<std::string> KEYWORDS = {
    "for", "if", "else", "while",
    "switch", "case", "break", "continue", "return",
    "int", "float", "double", "char", "void",
    "struct", "class", "public", "private", "protected",
    "auto"
};

bool isOperator(char c) {
    return std::string("+-*/=%<>!&|:").find(c) != std::string::npos;
}

std::vector<Token> lex(const std::string& code) {
    std::vector<Token> tokens;
    int i = 0;

    while (i < code.size()) {
        char c = code[i];

        // WHITESPACE
        if (isspace(c)) {
            int start = i;
            while (i < code.size() && isspace(code[i])) i++;
            tokens.push_back({TokenType::WHITESPACE, code.substr(start, i - start)});
        }

        // IDENTIFIER or KEYWORD
        else if (isalpha(c) || c == '_') {
            int start = i;
            while (i < code.size() && (isalnum(code[i]) || code[i] == '_')) i++;
            std::string word = code.substr(start, i - start);

            tokens.push_back({
                KEYWORDS.count(word) ? TokenType::KEYWORD : TokenType::IDENTIFIER,
                word
            });
        }

        // NUMBER
        else if (isdigit(c)) {
            int start = i;
            while (i < code.size() && isdigit(code[i])) i++;
            tokens.push_back({TokenType::NUMBER, code.substr(start, i - start)});
        }

        // STRING
        else if (c == '"') {
            int start = i++;
            while (i < code.size() && code[i] != '"') i++;
            i++; // closing "
            tokens.push_back({TokenType::STRING, code.substr(start, i - start)});
        }

        // OPERATOR
        else if (isOperator(c)) {
            tokens.push_back({TokenType::OPERATOR, std::string(1, c)});
            i++;
        }

        else {
            tokens.push_back({TokenType::UNKNOWN, std::string(1, c)});
            i++;
        }
    }
    return tokens;
}

// HTML 색칠
std::string highlight(const std::vector<Token>& tokens) {
    std::string html;

    for (auto& t : tokens) {
        switch (t.type) {
            case TokenType::KEYWORD:
                html += "<span style='color: blue; font-weight:bold'>" + t.value + "</span>";
                break;
            case TokenType::NUMBER:
                html += "<span style='color: red'>" + t.value + "</span>";
                break;
            case TokenType::STRING:
                html += "<span style='color: green'>" + t.value + "</span>";
                break;
            case TokenType::OPERATOR:
                html += "<span style='color: purple'>" + t.value + "</span>";
                break;
            default:
                html += t.value;
        }
    }
    return html;
}

#include <iostream>

int main() {
    std::string code = R"(for (auto& t : tokens) {
        switch (t.type) {
            case TokenType::KEYWORD:
                html += "<span style='color: blue; font-weight:bold'>" + t.value + "</span>";
                break;
            case TokenType::NUMBER:
                html += "<span style='color: red'>" + t.value + "</span>";
                break;
            case TokenType::STRING:
                html += "<span style='color: green'>" + t.value + "</span>";
                break;
            case TokenType::OPERATOR:
                html += "<span style='color: purple'>" + t.value + "</span>";
                break;
            default:
                html += t.value;
        }
    })";

    auto tokens = lex(code);
    std::string html = highlight(tokens);

    std::cout << html << std::endl;
}
