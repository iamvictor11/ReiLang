#include "lexer.hpp"
#include <cctype>
#include <charconv>

namespace luna
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
        while (!_isAtEnd() && _error_reporter->empty())
            _scan();
        _tokens.emplace_back(Token::TK_EOF, "", 0, _cursor.pos);
        return _tokens;
    }
#pragma region Scan
    void Lexer::_scan()
    {
        _skipWhite();
        _cursor.start = _cursor.current;
        char c = _advance();
        switch (c)
        {
        case ',': _addToken(Token::TK_COMMA); break;
        case '.': _addToken(Token::TK_DOT); break;
        case ':': _addToken(_match(':') ? Token::TK_DCOLON : (_match('=') ? Token::TK_WALRUS : Token::TK_COLON)); break;
        case ';': _addToken(Token::TK_SEMICOLON); break;
        case '(': _addToken(Token::TK_LPAREN); break;
        case ')': _addToken(Token::TK_RPAREN); break;
        case '[': _addToken(Token::TK_LBRACKET); break;
        case ']': _addToken(Token::TK_RBRACKET); break;
        case '{': _addToken(Token::TK_LBRACE); break;
        case '}': _addToken(Token::TK_RBRACE); break;
        case '+': _addToken(_match('=') ? Token::TK_SELF_ADD : Token::TK_ADD); break;
        case '-': _addToken(_match('=') ? Token::TK_SELF_SUB : (_match('>') ? Token::TK_RARROW : Token::TK_SUB)); break;
        case '*': _addToken(_match('=') ? Token::TK_SELF_MUL : (_match('*') ? (_match('=') ? Token::TK_SELF_POW : Token::TK_POW) : Token::TK_MUL)); break;
        case '/':
            if (_match('/')) _skipNote();
            else _addToken(_match('=') ? Token::TK_SELF_DIV : Token::TK_DIV);
            break;
        case '%': _addToken(_match('=') ? Token::TK_SELF_MOD : Token::TK_MOD); break;
        case '&': _addToken(_match('=') ? Token::TK_SELF_BIT_AND : (_match('&') ? Token::TK_AND : Token::TK_BIT_AND)); break;
        case '|': _addToken(_match('=') ? Token::TK_SELF_BIT_OR : (_match('|') ? Token::TK_OR : Token::TK_BIT_OR)); break;
        case '^': _addToken(_match('=') ? Token::TK_SELF_BIT_XOR : Token::TK_BIT_XOR); break;
        case '`': _addToken(_match('=') ? Token::TK_SELF_BIT_XNOR : Token::TK_BIT_XNOR); break;
        case '~': _addToken(_match('=') ? Token::TK_SELF_BIT_NOT : Token::TK_BIT_NOT); break;
        case '=': _addToken(_match('=') ? Token::TK_EQ : Token::TK_ASSIGN); break;
        case '!': _addToken(_match('=') ? Token::TK_NE : Token::TK_NOT); break;
        // case '<': _addToken(_match('=') ? Token::TK_LE : (_match('<') ? (_match('=') ? Token::TK_SELF_BIT_SHL : Token::TK_BIT_SHL) : (_match('-') ? Token::TK_LARROW : Token::TK_LT))); break;
        // case '>': _addToken(_match('=') ? Token::TK_GE : (_match('>') ? (_match('=') ? Token::TK_SELF_BIT_SHR : Token::TK_BIT_SHR) : (_match('-') ? Token::TK_RARROW : Token::TK_GT))); break;
        case '<':
            if (_match("<<=")) _addToken(Token::TK_SELF_BIT_SHL);
            else if (_match("<=")) _addToken(Token::TK_LE);
            else if (_match("<<")) _addToken(Token::TK_BIT_SHL);
            else if (_match("<-")) _addToken(Token::TK_LARROW);
            else _addToken(Token::TK_LT);
            break;
        case '>':
            if (_match(">>=")) _addToken(Token::TK_SELF_BIT_SHR);
            else if (_match(">=")) _addToken(Token::TK_GE);
            else if (_match(">>")) _addToken(Token::TK_BIT_SHR);
            else _addToken(Token::TK_GT);
            break;
        case '"': _lexString('"'); break;
        case '\'': _lexString('\''); break;
        case 'r': if (_peek() == '\'' || _peek() == '"') _lexRawString(_advance()); else _lexOther('r'); break;
        default: _lexOther(c); break;
        }
    }
    void Lexer::_moveCursor()
    {
        _cursor.current++;
        _cursor.pos.column++;
    }
    void Lexer::_moveCursorln()
    {
        _cursor.current++;
        _cursor.pos.line++;
        _cursor.pos.column = 1;
    }
    void Lexer::_smartMoceCursor(char c)
    {
        if (c == '\n') _moveCursorln();
        else _moveCursor();
    }
#pragma endregion
#pragma region Kan/Move
    bool Lexer::_isAtEnd() const
    {
        return _cursor.current >= _source.length();
    }
    bool Lexer::_isAtEnd(size_t offset) const
    {
        return _cursor.current + offset >= _source.length();
    }
    char Lexer::_advance()
    {
        if (_isAtEnd()) return '\0';
        char c = _source.at(_cursor.current);
        _smartMoceCursor(c);
        return c;
    }
    void Lexer::_pass()
    {
        if (_isAtEnd()) return;
        _smartMoceCursor(_source.at(_cursor.current));
    }
    void Lexer::_pass(size_t step)
    {
        while (step > 0)
        {
            if (_isAtEnd()) return;
            _smartMoceCursor(_source.at(_cursor.current));
            step--;
        }
    }
    bool Lexer::_match(char c)
    {
        if (_isAtEnd() || _source.at(_cursor.current) != c)
            return false;
        _smartMoceCursor(c);
        return true;
    }
    bool Lexer::_match(const char* s, size_t omit)
    {
        size_t i = 0;
        const char* p = s + omit;
        for (; *p != '\0'; p++)
        {
            if (_isAtEnd(i))
                return false;
            if (_peek(i) != *p)
                return false;
            i++;
        }
        _pass(i);
        return true;
    }
    char Lexer::_peek() const
    {
        if (_isAtEnd()) return '\0';
        return _source.at(_cursor.current);
    }
    char Lexer::_peek(size_t offset) const
    {
        if (_isAtEnd(offset)) return '\0';
        return _source.at(_cursor.current + offset);
    }
#pragma endregion
#pragma region Skip
void Lexer::_skipNote()
{
    while (_peek() != '\n' && !_isAtEnd())
        _moveCursor();
}
void Lexer::_skipWhite()
{
    while (!_isAtEnd())
    {
        char c = _peek();
        switch (c)
        {
        case ' ':
        case '\t':
        case '\r':
            _moveCursor();
            break;
        case '\n':
            _moveCursorln();
            break;
        default:
            return;
        }
    }
}
#pragma endregion
#pragma region Lex
void Lexer::_lexOther(char c)
{
    if (isdigit(c))
    {
        _lexNumber();
        return;
    }
    else if (isalpha(c) || c == '_')
    {
        _lexIdentifier();
        return;
    }
    _error_reporter->report("未知的字符", _cursor.pos);
}
void Lexer::_lexNumber()
{
    bool is_float = false;
    const char* start = _source.data() + _cursor.start;
    if (_peek() == 0)
    {
        char next = _peek(1);
        // 二进制
        if (next == 'b' || next == 'B')
        {
            _pass(2);
            while (isBdigit(_peek())) _pass();
            std::string_view lexeme {start, _cursor.current - _cursor.start};
            Int value = 0;
            for (size_t i = 2; i < lexeme.length(); ++i)
                value = (value << 1) | (lexeme[i] - '0');
            _addToken(Token::TK_LIT_INT, lexeme, value);
            return;
        }
        // 八进制
        if (next == 'o' || next == 'O')
        {
            _pass(2);
            while (isOdigit(_peek())) _pass();
            std::string_view lexeme {start, _cursor.current - _cursor.start};
            Int value = 0;
            for (size_t i = 2; i < lexeme.length(); ++i)
                value = value * 8 + (lexeme[i] - '0');
            _addToken(Token::TK_LIT_INT, lexeme, value);
            return;
        }
        // 十六进制
        if (next == 'x' || next == 'X')
        {
            _pass(2);
            while (isxdigit(_peek())) _pass();
            std::string_view lexeme {start, _cursor.current - _cursor.start};
            Int value = 0;
            for (size_t i = 2; i < lexeme.length(); ++i)
            {
                char c = lexeme[i];
                value = value * 16 + (isdigit(c) ? c - '0' : tolower(c) - 'a' + 10);
            }
            _addToken(Token::TK_LIT_INT, lexeme, value);
            return;
        }
    }
    else
    {
        // 整数
        while (isdigit(_peek())) _pass();
        // 浮点数
        if (_peek() == '.' && isdigit(_peek(1)))
        {
            is_float = true;
            _pass();
            while (isdigit(_peek())) _pass();
        }
        // 科学计数法
        if (_peek() == 'e' || _peek() == 'E')
        {
            char next = _peek(1);
            if (isdigit(next) || next == '+' || next == '-')
            {
                is_float = true;
                _pass();
                if (_peek() == '+' || _peek() == '-')
                    _pass();
                if (isdigit(_peek()))
                {
                    _pass();
                    while (isdigit(_peek())) _pass();
                }
                else
                {
                    _error_reporter->report("科学计数法格式错误", _cursor.pos);
                    return;
                }
            }
        }
    }
    // ok
    const char* end = _source.data() + _cursor.current;
    std::string_view lexeme {start, _cursor.current - _cursor.start};
    if (is_float)
    {
        Float value;
        auto [ptr, ec] = std::from_chars(start, end, value);
        if (ec == std::errc()) _addToken(Token::TK_LIT_FLOAT, lexeme, value);
        else _error_reporter->report("错误的浮点数格式", _cursor.pos);
    }
    else
    {
        Int value;
        auto [ptr, ec] = std::from_chars(start, end, value);
        if (ec == std::errc()) _addToken(Token::TK_LIT_INT, lexeme, value);
        else _error_reporter->report("错误的整数格式", _cursor.pos);
    }
}
void Lexer::_lexString(char beg)
{
    std::string value;
    while (_peek() != beg && !_isAtEnd())
    {
        char c = _advance();
        // 转义字符
        if (c == '\\')
        {
            if (_isAtEnd()) break;
            char next = _advance();
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
    if (_isAtEnd())
    {
        _error_reporter->report("字符串未闭合", _cursor.pos);
        return;
    }
    _pass();
    _addToken(Token::TK_LIT_STRING, value);
}
void Lexer::_lexRawString(char beg)
{
    std::string value;
    while (_peek() != beg && !_isAtEnd())
    {
        char c = _advance();
        // 转义字符
        if (c == '\\')
        {
            if (_isAtEnd()) break;
            char next = _advance();
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
    if (_isAtEnd())
    {
        _error_reporter->report("字符串未闭合", _cursor.pos);
        return;
    }
    _pass();
    _addToken(Token::TK_LIT_STRING, value);
}
void Lexer::_lexIdentifier()
{
    auto isidentifier = [](char c) -> bool
    {
        return isalnum(c) || isdigit(c);
    };
    while (isidentifier(_peek())) _pass();
    std::string_view lexeme {_source.data() + _cursor.start, _cursor.current - _cursor.start};
    Token::Type type = Token::toTypeFromKeyword(lexeme);
    Value::Data literal = Token::toLiteralFromKeyword(type);
    _addToken(type, lexeme, literal);
}
#pragma endregion
#pragma region Xie
    void Lexer::_addToken(Token::Type type)
    {
        std::string_view lexeme {_source.data() + _cursor.start, _cursor.current - _cursor.start};
        _tokens.emplace_back(type, lexeme, _cursor.pos);
    }
    void Lexer::_addToken(Token::Type type, Value::Data literal)
    {
        std::string_view lexeme {_source.data() + _cursor.start, _cursor.current - _cursor.start};
        _tokens.emplace_back(type, lexeme, literal, _cursor.pos);
    }
    void Lexer::_addToken(Token::Type type, std::string_view lexeme, Value::Data literal)
    {
        _tokens.emplace_back(type, lexeme, literal, _cursor.pos);
    }
#pragma endregion
}
