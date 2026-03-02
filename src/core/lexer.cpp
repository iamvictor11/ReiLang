#include "lexer.hpp"
#include "util/error.hpp"

namespace luna
{
    const Token::List& Lexer::tokenize(const std::string& s, Error::Reporter* er)
    {
        _source = s;
        _error_reporter = er;
        while (!_isAtEnd() && _error_reporter->empty())
        {
            _cursor.start = _cursor.current;
            _scan();
        }
        _tokens.emplace_back(Token::TK_EOF, "", 0, _cursor.pos);
        return _tokens;
    }
#pragma region Scan
    void Lexer::_scan()
    {
        char c = _advance();
        switch (c)
        {
        case '#': _addToken(Token::TK_DEF); break;
        case ',': _addToken(Token::TK_COMMA); break;
        case '.': _addToken(Token::TK_DOT); break;
        case ';': _addToken(Token::TK_SEMICOLON); break;
        case '(': _addToken(Token::TK_LPAREN); break;
        case ')': _addToken(Token::TK_RPAREN); break;
        case '[': _addToken(Token::TK_LBRACKET); break;
        case ']': _addToken(Token::TK_RBRACKET); break;
        case '{': _addToken(Token::TK_LBRACE); break;
        case '}': _addToken(Token::TK_RBRACE); break;
        case '+': _addToken(_match('=') ? Token::TK_SELF_ADD : Token::TK_ADD); break;
        case '-': _addToken(_match('=') ? Token::TK_SELF_SUB : Token::TK_SUB); break;
        case '*': _addToken(_match('=') ? Token::TK_SELF_MUL : Token::TK_MUL); break;
        case '/': _addToken(_match('=') ? Token::TK_SELF_DIV : Token::TK_DIV); break;
        case '%': _addToken(_match('=') ? Token::TK_SELF_MOD : Token::TK_MOD); break;
        case '&': _addToken(_match('=') ? Token::TK_SELF_BIT_AND : (_match('&') ? Token::TK_AND : Token::TK_BIT_AND)); break;
        case '|': _addToken(_match('=') ? Token::TK_SELF_BIT_OR : (_match('|') ? Token::TK_OR : Token::TK_BIT_OR)); break;
        case '^': _addToken(_match('=') ? Token::TK_SELF_BIT_XOR : Token::TK_BIT_XOR); break;
        case '`': _addToken(_match('=') ? Token::TK_SELF_BIT_XNOR : Token::TK_BIT_XNOR); break;
        case '~': _addToken(_match('=') ? Token::TK_SELF_BIT_NOT : Token::TK_BIT_NOT); break;
        case '=': _addToken(_match('=') ? Token::TK_EQ : Token::TK_ASSIGN); break;
        case '!': _addToken(_match('=') ? Token::TK_NE : Token::TK_NOT); break;
        case '<': _addToken(_match('=') ? Token::TK_LE : (_match('<') ? Token::TK_BIT_SHL : (_match('=') ? Token::TK_SELF_BIT_SHL : Token::TK_LT))); break;
        case '>': _addToken(_match('=') ? Token::TK_GE : (_match('>') ? Token::TK_BIT_SHR : (_match('=') ? Token::TK_SELF_BIT_SHR : Token::TK_GT))); break;
        default: _error_reporter->report("unknow character", _cursor.pos); break;
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
#pragma endregion
#pragma region Kan
    bool Lexer::_isAtEnd() const
    {
        return _cursor.current >= _source.length();
    }
    char Lexer::_advance()
    {
        char c = _source.at(_cursor.current);
        _moveCursor();
        return c;
    }
    bool Lexer::_match(char expected)
    {
        if (_isAtEnd() && _source.at(_cursor.current) != expected)
            return false;
        _moveCursor();
        return true;
    }
    char Lexer::_peek()
    {
        if (_isAtEnd()) return '\0';
        return _source.at(_cursor.current);
    }
#pragma endregion
#pragma region Xie
    void Lexer::_addToken(Token::Type type)
    {
        std::string lexeme = _source.substr(_cursor.start, _cursor.current);
        _tokens.emplace_back(type, lexeme, _cursor.pos);
    }
    void Lexer::_addToken(Token::Type type, Token::Literal literal)
    {
        std::string lexeme = _source.substr(_cursor.start, _cursor.current);
        _tokens.emplace_back(type, lexeme, literal, _cursor.pos);
    }
#pragma endregion
}
