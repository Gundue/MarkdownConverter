#include "Convertor.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>
#include <Windows.h>

using namespace std;

// HTML 특수 문자를 이스케이프하는 함수
string escape_html(const string& data) {
    string buffer;
    buffer.reserve(data.size());
    for (size_t pos = 0; pos != data.size(); ++pos) {
        switch (data[pos]) {
        case '&':  buffer.append("&amp;");       break;
        case '"': buffer.append("&quot;");      break;
        case '\'': buffer.append("&apos;");      break;
        case '<':  buffer.append("&lt;");        break;
        case '>':  buffer.append("&gt;");        break;
        default:   buffer.append(&data[pos], 1); break;
        }
    }
    return buffer;
}


// 인라인 마크다운 요소를 처리하는 함수
string processInlineMarkdown(const string& text) {
    string result = text;
    // 링크: [text](url) - Raw String Literal 사용으로 수정
    result = regex_replace(result, regex(R"(\[([^\]]+)\]\(([^)]+)\))"), R"(<a href="$2">$1</a>)");
    // 굵게, 기울임, 취소선
    result = regex_replace(result, regex(R"(\*\*(.*?)\*\*)"), R"(<strong>$1</strong>)");
    result = regex_replace(result, regex(R"(\*(.*?)\*)"), R"(<em>$1</em>)");
    result = regex_replace(result, regex(R"(~~(.*?)~~)"), R"(<s>$1</s>)");
    return result;
}

// 문자열 앞뒤 공백을 제거하는 함수
string trim(const string& str) {
    const char* whitespace = " \t\n\r\f\v";
    size_t first = str.find_first_not_of(whitespace);
    if (string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(whitespace);
    return str.substr(first, (last - first + 1));
}

// 구분자로 문자열을 나누는 함수
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// 마크다운을 HTML로 변환하는 주 함수
string markdownToHtml(const string& markdown) {
    stringstream ss(markdown);
    string line;
    string html;

    bool in_ul = false;
    bool in_ol = false;
    bool in_table = false;
    bool in_code_block = false;

    auto end_lists = [&]() {
        if (in_ul) { html += "</ul>\n"; in_ul = false; }
        if (in_ol) { html += "</ol>\n"; in_ol = false; }
        };

    auto end_table = [&]() {
        if (in_table) { html += "</tbody>\n</table>\n"; in_table = false; }
        };

    vector<string> lines;
    while (getline(ss, line)) {
        lines.push_back(line);
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        string current_line = lines[i];

        if (trim(current_line).rfind("```", 0) == 0) {
            if (!in_code_block) {
                end_lists();
                end_table();
                html += "<pre><code>";
                in_code_block = true;
            }
            else {
                html.pop_back(); // 마지막 개행 문자 제거
                html += "</code></pre>\n";
                in_code_block = false;
            }
            continue;
        }

        if (in_code_block) {
            html += escape_html(current_line) + "\n";
            continue;
        }

        string trimmed_line = trim(current_line);

        bool is_ul_item = trimmed_line.rfind("- ", 0) == 0 || trimmed_line.rfind("* ", 0) == 0 || trimmed_line.rfind("+ ", 0) == 0;
        bool is_ol_item = !trimmed_line.empty() && isdigit(trimmed_line[0]) && trimmed_line.find(". ") > 0 && trimmed_line.find(". ") < 4;
        bool is_table_row = trimmed_line.find('|') != string::npos;

        if (!is_ul_item && !is_ol_item) end_lists();
        if (!is_table_row) end_table();

        if (trimmed_line.empty()) {
            continue;
        }

        if (trimmed_line.rfind("### ", 0) == 0) {
            html += "<h3>" + processInlineMarkdown(trimmed_line.substr(4)) + "</h3>\n";
        }
        else if (trimmed_line.rfind("## ", 0) == 0) {
            html += "<h2>" + processInlineMarkdown(trimmed_line.substr(3)) + "</h2>\n";
        }
        else if (trimmed_line.rfind("# ", 0) == 0) {
            html += "<h1>" + processInlineMarkdown(trimmed_line.substr(2)) + "</h1>\n";
        }
        else if (trimmed_line.rfind(">> ", 0) == 0) {
            html += "<blockquote><blockquote><p>" + processInlineMarkdown(trimmed_line.substr(3)) + "</p></blockquote></blockquote>\n";
        }
        else if (trimmed_line.rfind("> ", 0) == 0) {
            html += "<blockquote><p>" + processInlineMarkdown(trimmed_line.substr(2)) + "</p></blockquote>\n";
        }
        else if (is_ul_item) {
            if (!in_ul) {
                html += "<ul>\n";
                in_ul = true;
            }
            string item = trimmed_line.substr(2);
            if (item.rfind("[x] ", 0) == 0) {
                html += "  <li><input type=\"checkbox\" checked> " + processInlineMarkdown(item.substr(4)) + "</li>\n";
            }
            else if (item.rfind("[ ] ", 0) == 0) {
                html += "  <li><input type=\"checkbox\"> " + processInlineMarkdown(item.substr(4)) + "</li>\n";
            }
            else {
                html += "  <li>" + processInlineMarkdown(item) + "</li>\n";
            }
        }
        else if (is_ol_item) {
            if (!in_ol) {
                html += "<ol>\n";
                in_ol = true;
            }
            html += "  <li>" + processInlineMarkdown(trimmed_line.substr(trimmed_line.find(". ") + 2)) + "</li>\n";
        }
        else if (trimmed_line == "---") {
            html += "<hr>\n";
        }
        else if (is_table_row) {
            if (!in_table) {
                // Check if the next line is a separator line
                if (i + 1 < lines.size()) {
                    string separator_line = trim(lines[i + 1]);
                    if (separator_line.find('|') != string::npos && separator_line.find('-') != string::npos) {
                        in_table = true;
                        html += "<table>\n<thead>\n<tr>\n";

                        // Process header row (current_line)
                        vector<string> headers = split(current_line, '|');
                        if (!headers.empty() && trim(headers.front()).empty()) {
                            headers.erase(headers.begin());
                        }
                        if (!headers.empty() && trim(headers.back()).empty()) {
                            headers.pop_back();
                        }
                        for (const auto& header : headers) {
                            html += "<th>" + processInlineMarkdown(trim(header)) + "</th>\n";
                        }

                        html += "</tr>\n</thead>\n<tbody>\n";
                        i++; // Move past the separator line
                    }
                }
            }
            else {
                // Process data row
                html += "<tr>\n";
                vector<string> cells = split(current_line, '|');
                if (!cells.empty() && trim(cells.front()).empty()) {
                    cells.erase(cells.begin());
                }
                if (!cells.empty() && trim(cells.back()).empty()) {
                    cells.pop_back();
                }
                for (const auto& cell : cells) {
                    html += "<td>" + processInlineMarkdown(trim(cell)) + "</td>\n";
                }
                html += "</tr>\n";
            }
        }
        else {
            html += "<p>" + processInlineMarkdown(trimmed_line) + "</p>\n";
        }
    }

    end_lists();
    end_table();
    if (in_code_block) { html += "</code></pre>\n"; }

    return html;
}

