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
        ast::NRef _assignmentExpr();
        ast::NRef _logicalOrExpr();
        ast::NRef _logicalAndExpr();
        ast::NRef _bitwiseOrExpr();
        ast::NRef _bitwiseXorExpr();
        ast::NRef _bitwiseAndExpr();
        ast::NRef _equalityExpr();
        ast::NRef _comparisonExpr();
        ast::NRef _bitwiseShiftExpr();
        ast::NRef _termExpr();
        ast::NRef _factorExpr();
        ast::NRef _powExpr();
        ast::NRef _unaryExpr();
        ast::NRef _primaryExpr();
        ast::NRef _varNameExpr();
    private:
        ast::NRef _statement();
        ast::NRef _varDeclStmt();
        ast::NRef _printStmt();
        ast::NRef _expressionStmt();
        ast::NRef _blockStmt();
        void _synchronize();
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
