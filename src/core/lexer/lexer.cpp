#include "lexer.hpp"
#include <cctype>
#include <charconv>
#include <format>

namespace rei
{
#pragma region Aid
    static bool isBdigit(char c)
    {
        return c == '0' || c == '1';
    }
    static bool isOdigit(char c)
    {
        return c >= '0' && c <= '7';
    }
#pragma endregion
    Token::List& Lexer::start()
    {
        while (!isAtEnd_())
            scan_();
        tokens_.emplace_back(Token::TK_EOF, "", 0, cursor_.pos);
        return tokens_;
    }
#pragma region Scan
    void Lexer::scan_()
    {
        skipWhite_();
        cursor_.start = cursor_.current;
        char c = advance_();
        switch (c)
        {
        case '\0': break;
        case ',': addToken_(Token::TK_COMMA); break;
        case '.': addToken_(Token::TK_DOT); break;
        case ':': addToken_(match_(':') ? Token::TK_DCOLON : (match_('=') ? Token::TK_WALRUS : Token::TK_COLON)); break;
        case ';': addToken_(Token::TK_SEMICOLON); break;
        case '(': addToken_(Token::TK_LPAREN); break;
        case ')': addToken_(Token::TK_RPAREN); break;
        case '[': addToken_(Token::TK_LBRACKET); break;
        case ']': addToken_(Token::TK_RBRACKET); break;
        case '{': addToken_(Token::TK_LBRACE); break;
        case '}': addToken_(Token::TK_RBRACE); break;
        case '+': addToken_(match_('=') ? Token::TK_SELF_ADD : Token::TK_ADD); break;
        case '-': addToken_(match_('=') ? Token::TK_SELF_SUB : (match_('>') ? Token::TK_RARROW : Token::TK_SUB)); break;
        case '*': addToken_(match_('=') ? Token::TK_SELF_MUL : (match_('*') ? (match_('=') ? Token::TK_SELF_POW : Token::TK_POW) : Token::TK_MUL)); break;
        case '/':
            if (match_('/')) skipNote_();
            else addToken_(match_('=') ? Token::TK_SELF_DIV : Token::TK_DIV);
            break;
        case '%': addToken_(match_('=') ? Token::TK_SELF_MOD : Token::TK_MOD); break;
        case '&': addToken_(match_('=') ? Token::TK_SELF_BIT_AND : (match_('&') ? Token::TK_AND : Token::TK_BIT_AND)); break;
        case '|': addToken_(match_('=') ? Token::TK_SELF_BIT_OR : (match_('|') ? Token::TK_OR : Token::TK_BIT_OR)); break;
        case '^': addToken_(match_('=') ? Token::TK_SELF_BIT_XOR : Token::TK_BIT_XOR); break;
        case '`': addToken_(match_('=') ? Token::TK_SELF_BIT_XNOR : Token::TK_BIT_XNOR); break;
        case '~': addToken_(match_('=') ? Token::TK_SELF_BIT_NOT : Token::TK_BIT_NOT); break;
        case '=': addToken_(match_('=') ? Token::TK_EQ : Token::TK_ASSIGN); break;
        case '!': addToken_(match_('=') ? Token::TK_NE : Token::TK_NOT); break;
        // case '<': _addToken(_match('=') ? Token::TK_LE : (_match('<') ? (_match('=') ? Token::TK_SELF_BIT_SHL : Token::TK_BIT_SHL) : (_match('-') ? Token::TK_LARROW : Token::TK_LT))); break;
        // case '>': _addToken(_match('=') ? Token::TK_GE : (_match('>') ? (_match('=') ? Token::TK_SELF_BIT_SHR : Token::TK_BIT_SHR) : (_match('-') ? Token::TK_RARROW : Token::TK_GT))); break;
        case '<':
            if (match_("<<=")) addToken_(Token::TK_SELF_BIT_SHL);
            else if (match_("<=")) addToken_(Token::TK_LE);
            else if (match_("<<")) addToken_(Token::TK_BIT_SHL);
            else if (match_("<-")) addToken_(Token::TK_LARROW);
            else addToken_(Token::TK_LT);
            break;
        case '>':
            if (match_(">>=")) addToken_(Token::TK_SELF_BIT_SHR);
            else if (match_(">=")) addToken_(Token::TK_GE);
            else if (match_(">>")) addToken_(Token::TK_BIT_SHR);
            else addToken_(Token::TK_GT);
            break;
        case '"': lexString_('"'); break;
        case '\'': lexString_('\''); break;
        case 'r': if (peek_() == '\'' || peek_() == '"') lexRawString_(advance_()); else lexOther_('r'); break;
        default: lexOther_(c); break;
        }
    }
    void Lexer::moveCursor_()
    {
        cursor_.current++;
        cursor_.pos.column++;
    }
    void Lexer::moveCursorln_()
    {
        cursor_.current++;
        cursor_.pos.line++;
        cursor_.pos.column = 1;
    }
    void Lexer::smartMoceCursor_(char c)
    {
        if (c == '\n') moveCursorln_();
        else moveCursor_();
    }
#pragma endregion
#pragma region Kan/Move
    bool Lexer::isAtEnd_() const
    {
        return cursor_.current >= source_.length();
    }
    bool Lexer::isAtEnd_(size_t offset) const
    {
        return cursor_.current + offset >= source_.length();
    }
    char Lexer::advance_()
    {
        if (isAtEnd_()) return '\0';
        char c = source_.at(cursor_.current);
        smartMoceCursor_(c);
        return c;
    }
    void Lexer::pass_()
    {
        if (isAtEnd_()) return;
        smartMoceCursor_(source_.at(cursor_.current));
    }
    void Lexer::pass_(size_t step)
    {
        while (step > 0)
        {
            if (isAtEnd_()) return;
            smartMoceCursor_(source_.at(cursor_.current));
            step--;
        }
    }
    bool Lexer::match_(char c)
    {
        if (isAtEnd_() || source_.at(cursor_.current) != c)
            return false;
        smartMoceCursor_(c);
        return true;
    }
    bool Lexer::match_(const char* s, size_t omit)
    {
        size_t i = 0;
        const char* p = s + omit;
        for (; *p != '\0'; p++)
        {
            if (isAtEnd_(i))
                return false;
            if (peek_(i) != *p)
                return false;
            i++;
        }
        pass_(i);
        return true;
    }
    char Lexer::peek_() const
    {
        if (isAtEnd_()) return '\0';
        return source_.at(cursor_.current);
    }
    char Lexer::peek_(int offset) const
    {
        if (isAtEnd_(offset)) return '\0';
        return source_.at(cursor_.current + offset);
    }
#pragma endregion
#pragma region Skip
void Lexer::skipNote_()
{
    while (peek_() != '\n' && !isAtEnd_())
        moveCursor_();
}
void Lexer::skipWhite_()
{
    while (!isAtEnd_())
    {
        char c = peek_();
        switch (c)
        {
        case ' ':
        case '\t':
        case '\r':
            moveCursor_();
            break;
        case '\n':
            moveCursorln_();
            break;
        default:
            return;
        }
    }
}
#pragma endregion
#pragma region Lex
void Lexer::lexOther_(char c)
{
    if (isdigit(c))
    {
        lexNumber_();
        return;
    }
    else if (isalpha(c) || c == '_')
    {
        lexIdentifier_();
        return;
    }
    _error_reporter->report(std::format("未知的字符'{}'(HEX: 0x{:02x})", c, (unsigned char)c), cursor_.pos);
}
void Lexer::lexNumber_()
{
    bool is_float = false;
    const char* start = source_.data() + cursor_.start;
    printf("debug n %c\n", peek_(-1));
    if (peek_(-1) == '0')
    {
        char next = peek_();
        printf("debug 0 %c\n", next);
        // 二进制
        if (next == 'b' || next == 'B')
        {
            pass_(2);
            while (isBdigit(peek_())) pass_();
            std::string_view lexeme {start, cursor_.current - cursor_.start};
            Integer value = 0;
            for (size_t i = 2; i < lexeme.length(); i++)
                value = (value << 1) | (lexeme[i] - '0');
            printf("debug b %s\n", std::string(lexeme).c_str());
            addToken_(Token::TK_LIT_INT, lexeme, value);
            return;
        }
        // 八进制
        if (next == 'o' || next == 'O')
        {
            pass_(2);
            while (isOdigit(peek_())) pass_();
            std::string_view lexeme {start, cursor_.current - cursor_.start};
            Integer value = 0;
            for (size_t i = 2; i < lexeme.length(); i++)
                value = value * 8 + (lexeme[i] - '0');
            printf("debug o %s\n", std::string(lexeme).c_str());
            addToken_(Token::TK_LIT_INT, lexeme, value);
            return;
        }
        // 十六进制
        if (next == 'x' || next == 'X')
        {
            pass_(2);
            while (isxdigit(peek_())) pass_();
            std::string_view lexeme {start, cursor_.current - cursor_.start};
            Integer value = 0;
            for (size_t i = 2; i < lexeme.length(); i++)
            {
                char c = lexeme[i];
                value = value * 16 + (isdigit(c) ? c - '0' : tolower(c) - 'a' + 10);
            }
            printf("debug x %s\n", std::string(lexeme).c_str());
            addToken_(Token::TK_LIT_INT, lexeme, value);
            return;
        }
    }
    else
    {
        // 整数
        while (isdigit(peek_())) pass_();
        // 浮点数
        if (peek_() == '.' && isdigit(peek_(1)))
        {
            is_float = true;
            pass_();
            while (isdigit(peek_())) pass_();
        }
        // 科学计数法
        if (peek_() == 'e' || peek_() == 'E')
        {
            char next = peek_(1);
            if (isdigit(next) || next == '+' || next == '-')
            {
                is_float = true;
                pass_();
                if (peek_() == '+' || peek_() == '-')
                    pass_();
                if (isdigit(peek_()))
                {
                    pass_();
                    while (isdigit(peek_())) pass_();
                }
                else
                {
                    _error_reporter->report("科学计数法格式错误", cursor_.pos);
                    return;
                }
            }
        }
    }
    // ok
    const char* end = source_.data() + cursor_.current;
    std::string_view lexeme {start, cursor_.current - cursor_.start};
    if (is_float)
    {
        Float value;
        auto [ptr, ec] = std::from_chars(start, end, value);
        if (ec == std::errc()) addToken_(Token::TK_LIT_FLOAT, lexeme, value);
        else _error_reporter->report("错误的浮点数格式", cursor_.pos);
    }
    else
    {
        Integer value;
        auto [ptr, ec] = std::from_chars(start, end, value);
        if (ec == std::errc()) addToken_(Token::TK_LIT_INT, lexeme, value);
        else _error_reporter->report("错误的整数格式", cursor_.pos);
    }
}
void Lexer::lexString_(char beg)
{
    std::string value;
    while (peek_() != beg && !isAtEnd_())
    {
        char c = advance_();
        // 转义字符
        if (c == '\\')
        {
            if (isAtEnd_()) break;
            char next = advance_();
            switch (next)
            {
                case 'n':  value += '\n'; break;
                case 'r':  value += '\r'; break;
                case 't':  value += '\t'; break;
                case '\\': value += '\\'; break;
                case '"':  value += '"';  break;
                case '\'': value += '\''; break;
                default: value += c; value += next; break;
            }
        }
        else
        {
            value += c;
        }
    }
    if (isAtEnd_())
    {
        _error_reporter->report("字符串未闭合", cursor_.pos);
        return;
    }
    pass_();
    addToken_(Token::TK_LIT_STRING, value);
}
void Lexer::lexRawString_(char beg)
{
    std::string value;
    while (peek_() != beg && !isAtEnd_())
    {
        char c = advance_();
        // 转义字符
        if (c == '\\')
        {
            if (isAtEnd_()) break;
            char next = advance_();
            switch (next)
            {
                case '\\': value += '\\'; break;
                case '"':  value += '"';  break;
                case '\'': value += '\''; break;
                default: value += c; value += next; break;
            }
        }
        else
        {
            value += c;
        }
    }
    if (isAtEnd_())
    {
        _error_reporter->report("字符串未闭合", cursor_.pos);
        return;
    }
    pass_();
    addToken_(Token::TK_LIT_STRING, value);
}
void Lexer::lexIdentifier_()
{
    auto isidentifier = [](char c) -> bool
    {
        return isalnum(c) || c == '_';
    };
    while (isidentifier(peek_())) pass_();
    std::string_view lexeme {source_.data() + cursor_.start, cursor_.current - cursor_.start};
    Token::Type type = Token::toTypeFromKeyword(lexeme);
    Value::Data literal = Token::toLiteralFromKeyword(type);
    addToken_(type, lexeme, literal);
}
#pragma endregion
#pragma region Xie
    void Lexer::addToken_(Token::Type type)
    {
        std::string_view lexeme {source_.data() + cursor_.start, cursor_.current - cursor_.start};
        tokens_.emplace_back(type, lexeme, cursor_.pos);
    }
    void Lexer::addToken_(Token::Type type, Value::Data literal)
    {
        std::string_view lexeme {source_.data() + cursor_.start, cursor_.current - cursor_.start};
        tokens_.emplace_back(type, lexeme, literal, cursor_.pos);
    }
    void Lexer::addToken_(Token::Type type, std::string_view lexeme, Value::Data literal)
    {
        tokens_.emplace_back(type, lexeme, literal, cursor_.pos);
    }
#pragma endregion
}
