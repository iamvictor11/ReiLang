#pragma once
#include "core/lexer/token.hpp"
#include "core/base/env.hpp"
#include "util/error.hpp"
#include "precedence.hpp"

namespace luna
{
    class Parser
    {
    private:
        using _Fnuc = void(Parser::*)();
        struct _Rule final
        {
            _Fnuc prefix;
            _Fnuc infix;
            _Fnuc suffix;
            Precedence precedence;
            uint8_t is_left_assoc;
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
        } _cursor;
        Error::Reporter* _error_reporter;
    public:
        Parser(Token::List&& ts, Chunk* ck, Error::Reporter* er) : _tokens(std::move(ts)), _chunk(ck), _error_reporter(er) {};
        ~Parser() = default;
    public:
        void start();
    private:
        void _program();
        void _parsePrecedence(Precedence precedence);
    private:
        void _expression();
        void _assignExpr();
        void _groupingExpr();
        void _unaryExpr();
        void _binaryExpr();
        void _primaryExpr();
    private:
        void _statement();
        void _exprStmt();
        void _blockStmt();
        void _ifStmt();
        void _loopStmt();
        void _printStmt();
    private:
        void _declaration();
        void _varDecl();
        void _funcDecl();
        void _structDecl();
        void _classDecl();
    private:
        bool _isAtEnd() const;
        Token::Unit& _advance();
        void _pass();
        Token::Unit& _prev();
        Token::Unit& _peek();
        bool _check(Token::Type type);
        bool _check(std::initializer_list<Token::Type> types);
        bool _match(std::initializer_list<Token::Type> types);
        Token::Unit& _consume(Token::Type type, const std::string& message);
    private:
        void _emitB(Bytecode op);
        Bytecode _emitC(Value::Data value);
    };
}
