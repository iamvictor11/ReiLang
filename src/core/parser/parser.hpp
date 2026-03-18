#pragma once
#include "core/lexer/token.hpp"
#include "core/base/env.hpp"
#include "util/error.hpp"
#include "precedence.hpp"

namespace rei
{
    class Parser
    {
    private:
        using _MyFnuc = void(Parser::*)();
        struct _Rule final
        {
            _MyFnuc prefix;
            _MyFnuc infix;
            _MyFnuc suffix;
            Precedence precedence;
            uint8_t is_left_assoc;
        };
    private:
        struct _LoopCtx final
        {
            Bytecode start;
            Bytecode depth;
            std::vector<Bytecode> breaks;
        };
        struct _FuncCtx final
        {
            Bytecode depth;
            std::vector<Bytecode> returns;
        };
    private:
        static _Rule _rules_s[];
    private:
        std::vector<_LoopCtx> _loop_ctxs {};
        std::vector<_FuncCtx> _func_ctxs {};
        Bytecode _closed_floors = 0;
    private:
        Token::List _tokens;
        Chunk* _chunk;
        Env* _env;
        struct
        {
            Token::Unit* prev = nullptr;
            Token::Unit* curr = nullptr;
        } _cursor;
        Error::Reporter* _error_reporter;
    public:
        Parser(Token::List&& ts, Chunk* ck, Env* ev, Error::Reporter* er) : _tokens(std::move(ts)), _chunk(ck), _env(ev), _error_reporter(er) {};
        ~Parser() = default;
    public:
        void start();
    private:
        void _program();
        void _parsePrecedence(Precedence precedence);
        /// @param opt 0 def, 1 get, 2 set;
        void _parseVariable(const std::string& name, Bytecode depth, uint8_t opt);
    private:
        void _expression();
        void _assignExpr();
        void _groupingExpr();
        void _unaryExpr();
        void _binaryExpr();
        void _primaryExpr();
        void _varExpr();
        void _callExpr();
    private:
        void _statement();
        void _exprStmt();
        void _blockStmt();
        void _ifStmt();
        void _loopStmt();
        void _breakStmt();
        void _continueStmt();
        void _returnStmt();
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
        void _consume(Token::Type type, const std::string& message);
    private:
        void _emitB(Bytecode op);
        Bytecode _emitC(Value::Data value);
        void _patchB(Bytecode pos, Bytecode op);
    private:
        void _reporterError(const std::string& msg);
    };
}
