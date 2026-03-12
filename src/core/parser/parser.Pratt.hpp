#pragma once
#include "core/lexer/token.hpp"
#include "util/error.hpp"
#include "parser.hpp"
#include "precedence.hpp"

namespace luna
{
    template<>
    class Parser<PT_Pratt>
    {
    private:
        using _Fnuc = void(*)();
        struct _Rule final
        {
            _Fnuc prefix;
            _Fnuc infix;
            _Fnuc suffix;
            Precedence::Level precdence;
        };
    private:
        static _Rule _rules_s[];
    private:
        Token::List _tokens;
        Chunk* _chunk;
        struct
        {
            Token::Unit* prev = nullptr;
            Token::Unit* curr = nullptr;
            Token::Unit* next = nullptr;
        } _cursor;
        Error::Reporter* _error_reporter;
    public:
        Parser(Token::List&& ts, Chunk* ck, Error::Reporter* er) : _tokens(std::move(ts)), _chunk(ck), _error_reporter(er) {};
        ~Parser() = default;
    public:
        void start();
    private:
        void _program();
    private:
        void _expression();
        void _assign();
        void _unary();
        void _binary();
        void _primary();
        void _grouping();
    private:
        void _statement();
    private:
        void _parsePrecedence(Precedence::Level precedence);
    private:
        bool _isAtEnd() const;
        Token::Unit& _advance();
        void _pass();
        Token::Unit& _prev();
        Token::Unit& _peek();
        Token::Unit& _next();
        bool _check(Token::Type type);
        bool _check(std::initializer_list<Token::Type> types);
        bool _match(std::initializer_list<Token::Type> types);
        Token::Unit& _consume(Token::Type type, const std::string& message);
    };
}
