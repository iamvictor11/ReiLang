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
        [[nodiscard]] const Token::List& tokenize(const std::string& s, Error::Reporter* er);
    private:
        void _scan();
        void _moveCursor();
        void _moveCursorln();
        void _smartMoceCursor(char c);
    private:
        bool _isAtEnd() const;
        bool _isAtEnd(size_t offset) const;
        char _advance();
        void _pass();
        void _pass(size_t step);
        bool _match(char c);
        char _peek() const;
        char _peek(size_t offset) const;
    private:
        void _skipNote();
        void _skipWhite();
    private:
        void _lexOther(char c);
        void _lexNumber();
        void _lexString(char beg);
        void _lexRawString(char beg);
        void _lexIdentifier();
    private:
        void _addToken(Token::Type type);
        void _addToken(Token::Type type, Value::Data literal);
        void _addToken(Token::Type type, std::string_view lexeme, Value::Data literal);
    };
}
