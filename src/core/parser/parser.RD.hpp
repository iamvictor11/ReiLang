#pragma once
#include "core/lexer/token.hpp"
#include "util/error.hpp"
#include "node.hpp"
#include "parser.hpp"

namespace luna
{
    template<>
    class Parser<PT_RD>
    {
    private:
        Token::List _tokens;
        ast::NRef _programNodeRef = nullptr;
        struct
        {
            uint64_t current = 0;
        } _cursor;
        Error::Reporter* _error_reporter;
    public:
        Parser(Token::List&& ts, Error::Reporter* er) : _tokens(std::move(ts)), _error_reporter(er) {};
        ~Parser() = default;
    public:
        [[nodiscard]] ast::NRef start();
        ast::NRef _program();
    private:
        ast::NRef _expression();
        ast::NRef _assignment();
        ast::NRef _logical_or();
        ast::NRef _logical_and();
        ast::NRef _bitwise_or();
        ast::NRef _bitwise_xor();
        ast::NRef _bitwise_and();
        ast::NRef _equality();
        ast::NRef _comparison();
        ast::NRef _bitwise_shift();
        ast::NRef _term();
        ast::NRef _factor();
        ast::NRef _pow();
        ast::NRef _unary();
        ast::NRef _primary();
    private:
        void _synchronize();
    private:
        bool _isAtEnd() const;
        Token::Unit& _advance();
        void _pass();
        Token::Unit& _prev();
        Token::Unit& _peek();
        Token::Unit& _next();
        bool _check(Token::Type type);
        bool _match(std::initializer_list<Token::Type> types);
        Token::Type _consume(Token::Type type, const std::string& message);
    };
}
