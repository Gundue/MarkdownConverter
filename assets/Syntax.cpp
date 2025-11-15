#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>
#include <iostream>

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
    int n = static_cast<int>(code.size());

    while (i < n) {
        char c = code[i];

        // WHITESPACE
        if (isspace(static_cast<unsigned char>(c))) {
            int start = i;
            while (i < n && isspace(static_cast<unsigned char>(code[i]))) i++;
            tokens.push_back({TokenType::WHITESPACE, code.substr(start, i - start)});
        }

        // IDENTIFIER or KEYWORD
        else if (isalpha(static_cast<unsigned char>(c)) || c == '_') {
            int start = i;
            while (i < n && (isalnum(static_cast<unsigned char>(code[i])) || code[i] == '_')) i++;
            std::string word = code.substr(start, i - start);

            tokens.push_back({
                KEYWORDS.count(word) ? TokenType::KEYWORD : TokenType::IDENTIFIER,
                word
            });
        }

        // NUMBER
        else if (isdigit(static_cast<unsigned char>(c))) {
            int start = i;
            while (i < n && isdigit(static_cast<unsigned char>(code[i]))) i++;
            tokens.push_back({TokenType::NUMBER, code.substr(start, i - start)});
        }

        // STRING (단순 처리: "..." 형태만)
        else if (c == '"') {
            int start = i++;
            while (i < n) {
                if (code[i] == '\\' && i + 1 < n) {
                    // 이스케이프 문자 건너뛰기 (\" 같은 경우)
                    i += 2;
                } else if (code[i] == '"') {
                    i++; // closing "
                    break;
                } else {
                    i++;
                }
            }
            tokens.push_back({TokenType::STRING, code.substr(start, i - start)});
        }

        // OPERATOR
        else if (isOperator(c)) {
            // 간단히 한 글자 연산자 처리 (>=, == 등 다중 문자 연산자는 필요시 확장)
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

    for (const auto& t : tokens) {
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

int main() {
    // raw-string 구분자를 CODE로 지정: 내부에 )" 가 있어도 안전
    std::string code = R"CODE(for (auto& t : tokens) {
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
    })CODE";

    auto tokens = lex(code);
    std::string html = highlight(tokens);

    std::cout << html << std::endl;
    return 0;
}
