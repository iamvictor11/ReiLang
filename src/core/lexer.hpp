#pragma once
#include "token.hpp"

namespace luna
{
    namespace Error
    {
        class Reporter;
    }
    class Lexer final
    {
    private:
        std::string _source;
        Token::List _tokens;
        struct
        {
            uint64_t start = 0;
            uint64_t current = 0;
            Position pos = {1, 1};
        } _cursor;
        Error::Reporter* _error_reporter = nullptr;
    public:
        Lexer() = default;
        ~Lexer() = default;
    public:
        const Token::List& tokenize(const std::string& s, Error::Reporter* er);
    private:
        void _scan();
        void _moveCursor();
        void _moveCursorln();
    private:
        bool _isAtEnd() const;
        char _advance();
        bool _match(char expected);
        char _peek();
    private:
        void _addToken(Token::Type type);
        void _addToken(Token::Type type, Token::Literal literal);
    };
}
