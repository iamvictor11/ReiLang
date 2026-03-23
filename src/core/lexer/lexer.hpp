#pragma once
#include "token.hpp"
#include "util/error.hpp"

namespace rei
{
    class Lexer final
    {
    private:
        std::string source_;
        Token::List tokens_;
        struct
        {
            uint64_t start = 0;
            uint64_t current = 0;
            Position pos = {1, 1};
        } cursor_;
        Error::Reporter* error_reporter_;
    public:
        Lexer(std::string&& s, Error::Reporter* er) : source_(std::move(s)), error_reporter_(er) {};
        ~Lexer() = default;
    public:
        [[nodiscard]] Token::List& start();
    private:
        void scan_();
        void moveCursor_();
        void moveCursorln_();
        void smartMoceCursor_(char c);
    private:
        bool isAtEnd_() const;
        bool isAtEnd_(size_t offset) const;
        char advance_();
        void pass_();
        void pass_(size_t step);
        bool match_(char c);
        bool match_(const char* s, size_t omit = 1);
        char peek_() const;
        char peek_(int offset) const;
    private:
        void skipNote_();
        void skipWhite_();
    private:
        void lexOther_(char c);
        void lexNumber_();
        void lexString_(char beg);
        void lexRawString_(char beg);
        void lexIdentifier_();
    private:
        void addToken_(Token::Type type);
        void addToken_(Token::Type type, Value::Data literal);
        void addToken_(Token::Type type, std::string_view lexeme, Value::Data literal);
    };
}
