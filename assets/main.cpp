#include <string>
#include "Convertor.h"
#include <iostream>

using namespace std;

int main()
{
    string markdown_text = R"(# Heading 1
## Heading 2
### Heading 3

[Go to Google](https://www.google.com)

> Quotation

- Unordered List Item 1
* Unordered List Item 2
+ Unordered List Item 3

- [x] Completed Task
- [ ] Incomplete Task

1. First Ordered Item
2. Second Ordered Item

```cpp
#include <iostream>
int main() {
    cout << "Hello, World!";
    return 0;
}
```

**Bold**, *Italic*, ~~Strikethrough~~

---
| Name | Age |
|------|------|
| Hong Gil-dong | 25 |
| Kim Cheol-su | 30 |
)";

    string html_output = markdownToHtml(markdown_text);
    cout << html_output << endl;

    // syntax highlighting test

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