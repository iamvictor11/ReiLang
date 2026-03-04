#include "parser.hpp"

namespace luna
{
    void Parser::start()
    {
        
    }
    ast::NRef Parser::_program()
    {

    }
#pragma region Expr
    ast::NRef Parser::_expression()
    {
        return Parser::_assignment();
    }
    ast::NRef Parser::_assignment()
    {
        ast::NRef expr = _logical_or();
        if (_match({
            Token::TK_ASSIGN, Token::TK_WALRUS,
            Token::TK_SELF_ADD, Token::TK_SELF_SUB, Token::TK_SELF_MUL,
            Token::TK_SELF_DIV, Token::TK_SELF_MOD, Token::TK_SELF_POW,
            Token::TK_SELF_BIT_AND, Token::TK_SELF_BIT_OR, Token::TK_SELF_BIT_XOR
        }))
        {
            Token::Type oper = _prev().type;
            ast::NRef value = _assignment();
            return ast::make_ref(Expr::Binary{std::move(expr), oper, std::move(value)});
        }
        return expr;
    }
    ast::NRef Parser::_logical_or()
    {
        ast::NRef expr = _logical_and();
        while (_match({Token::TK_OR}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _logical_and();
            expr = ast::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    ast::NRef Parser::_logical_and()
    {
        ast::NRef expr = _equality();
        while (_match({Token::TK_AND}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _equality();
            expr = ast::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    ast::NRef Parser::_bitwise_or()
    {
        ast::NRef expr = _bitwise_xor();
        while (_match({Token::TK_BIT_OR}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _bitwise_xor();
            expr = ast::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    ast::NRef Parser::_bitwise_xor()
    {
        ast::NRef expr = _bitwise_and();
        while (_match({Token::TK_BIT_XOR, Token::TK_BIT_XNOR}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _bitwise_and();
            expr = ast::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    ast::NRef Parser::_bitwise_and()
    {
        ast::NRef expr = _equality();
        while (_match({Token::TK_BIT_AND}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _equality();
            expr = ast::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    ast::NRef Parser::_equality()
    {
        ast::NRef expr = _comparison();
        while (_match({Token::TK_EQ, Token::TK_NE}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _comparison();
            expr = ast::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    ast::NRef Parser::_comparison()
    {
        ast::NRef expr = _term();
        while (_match({Token::TK_GT, Token::TK_GE, Token::TK_LT, Token::TK_LE}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _term();
            expr = ast::make_ref(Expr::Binary(std::move(expr), oper, std::move(right)));
        }
        return expr;
    }
    ast::NRef Parser::_term()
    {
        ast::NRef expr = _factor();
        while (_match({Token::TK_ADD, Token::TK_SUB}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _factor();
            expr = ast::make_ref(Expr::Binary(std::move(expr), oper, std::move(right)));
        }
        return expr;
    }
    ast::NRef Parser::_factor()
    {
        ast::NRef expr = _unary();
        while (_match({Token::TK_MUL, Token::TK_DIV, Token::TK_MOD}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _unary();
            expr = ast::make_ref(Expr::Binary(std::move(expr), oper, std::move(right)));
        }
        return expr;
    }
    ast::NRef Parser::_pow()
    {
        ast::NRef expr = _unary();
        if (_match({Token::TK_POW}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _pow();
            expr = ast::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    ast::NRef Parser::_unary()
    {
        if (_match({Token::TK_NOT, Token::TK_SUB, Token::TK_BIT_NOT}))
        {
            Token::Type oper = _prev().type;
            ast::NRef right = _unary();
            return ast::make_ref(Expr::Unary(oper, std::move(right)));
        }
        return _primary();
    }
    ast::NRef Parser::_primary()
    {   
        if (_match({
            Token::TK_NIL,
            Token::TK_LIT_INT, Token::TK_LIT_FLOAT, Token::TK_TRUE, Token::TK_FALSE,
            Token::TK_LIT_STRING
        }))
        {
            return ast::make_ref(Expr::Literal{_prev().type});
        }
        if (_match({Token::TK_LPAREN}))
        {
            ast::NRef expr = _expression();
            _consume(Token::TK_RPAREN, "括号未闭合");
            return ast::make_ref(Expr::Grouping{std::move(expr)});
        }
        _error_reporter->report("没有对应匹配", _peek().pos);
        return nullptr;
    }
#pragma endregion
#pragma region Kan/Move
    bool Parser::_isAtEnd() const
    {
        return _cursor.current == _tokens.size() - 1;
    }
    Token::Unit& Parser::_advance()
    {
        if (!_isAtEnd()) _cursor.current++;
        return _prev();
    }
    void Parser::_pass()
    {
        if (!_isAtEnd()) _cursor.current++;\
    }
    Token::Unit& Parser::_prev()
    {
        return _tokens.at(_cursor.current - 1);
    }
    Token::Unit& Parser::_peek()
    {
        return _tokens.at(_cursor.current);
    }
    Token::Unit& Parser::_next()
    {
        return _tokens.at(_cursor.current + 1);
    }
    bool Parser::_check(Token::Type type)
    {
        if (_isAtEnd()) return false;
        return _peek().type == type;
    }
    bool Parser::_match(std::initializer_list<Token::Type> types)
    {
        for (Token::Type type : types)
        {
            if (_check(type))
            {
                _pass();
                return true;
            }
        }
        return false;
    }
    Token::Type Parser::_consume(Token::Type type, const std::string& message)
    {
        if (_check(type))
            return _advance().type;
        _error_reporter->report(message, _peek().pos);
        return Token::TK_EOF;
    }
#pragma endregion
}
