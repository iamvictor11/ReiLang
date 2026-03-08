#include "parser.RD.hpp"

namespace luna
{
    using namespace ast;
    NRef Parser<PT_RD>::start()
    {
        _programNodeRef = _program();
        return std::move(_programNodeRef);
    }
    NRef Parser<PT_RD>::_program()
    {
        Program p{};
        while (!_isAtEnd())
        {
            NRef nr = _statement();
            if (nr)
                p.nodes.push_back(std::move(nr));
        }
        return Node::make_ref(std::move(p));
    }
#pragma region Expr
    NRef Parser<PT_RD>::_expression()
    {
        return Parser<PT_RD>::_assignmentExpr();
    }
    NRef Parser<PT_RD>::_assignmentExpr()
    {
        NRef expr = _logicalOrExpr();
        if (_match({
            Token::TK_ASSIGN, Token::TK_WALRUS,
            Token::TK_SELF_ADD, Token::TK_SELF_SUB, Token::TK_SELF_MUL, Token::TK_SELF_DIV, Token::TK_SELF_MOD, Token::TK_SELF_POW,
            Token::TK_SELF_BIT_AND, Token::TK_SELF_BIT_OR, Token::TK_SELF_BIT_XOR, Token::TK_SELF_BIT_XNOR, Token::TK_SELF_BIT_SHL, Token::TK_SELF_BIT_SHR
        }))
        {
            if (!std::holds_alternative<Expr::VarName>(expr->data))
            {
                _error_reporter->report("赋值表达式左边必须为左值", _prev().pos);
                return nullptr;
            }
            Token::Type oper = _prev().type;
            NRef value = _assignmentExpr();
            return Node::make_ref(Expr::Assign{std::move(expr), oper, std::move(value)});
        }
        return expr;
    }
    NRef Parser<PT_RD>::_logicalOrExpr()
    {
        NRef expr = _logicalAndExpr();
        while (_match({Token::TK_OR}))
        {
            Token::Type oper = _prev().type;
            NRef right = _logicalAndExpr();
            expr = Node::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    NRef Parser<PT_RD>::_logicalAndExpr()
    {
        NRef expr = _bitwiseOrExpr();
        while (_match({Token::TK_AND}))
        {
            Token::Type oper = _prev().type;
            NRef right = _bitwiseOrExpr();
            expr = Node::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    NRef Parser<PT_RD>::_bitwiseOrExpr()
    {
        NRef expr = _bitwiseXorExpr();
        while (_match({Token::TK_BIT_OR}))
        {
            Token::Type oper = _prev().type;
            NRef right = _bitwiseXorExpr();
            expr = Node::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    NRef Parser<PT_RD>::_bitwiseXorExpr()
    {
        NRef expr = _bitwiseAndExpr();
        while (_match({Token::TK_BIT_XOR, Token::TK_BIT_XNOR}))
        {
            Token::Type oper = _prev().type;
            NRef right = _bitwiseAndExpr();
            expr = Node::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    NRef Parser<PT_RD>::_bitwiseAndExpr()
    {
        NRef expr = _equalityExpr();
        while (_match({Token::TK_BIT_AND}))
        {
            Token::Type oper = _prev().type;
            NRef right = _equalityExpr();
            expr = Node::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    NRef Parser<PT_RD>::_equalityExpr()
    {
        NRef expr = _comparisonExpr();
        while (_match({Token::TK_EQ, Token::TK_NE}))
        {
            Token::Type oper = _prev().type;
            NRef right = _comparisonExpr();
            expr = Node::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    NRef Parser<PT_RD>::_comparisonExpr()
    {
        NRef expr = _bitwiseShiftExpr();
        while (_match({Token::TK_GT, Token::TK_GE, Token::TK_LT, Token::TK_LE}))
        {
            Token::Type oper = _prev().type;
            NRef right = _bitwiseShiftExpr();
            expr = Node::make_ref(Expr::Binary(std::move(expr), oper, std::move(right)));
        }
        return expr;
    }
    NRef Parser<PT_RD>::_bitwiseShiftExpr()
    {
        NRef expr = _termExpr();
        while (_match({Token::TK_BIT_SHL, Token::TK_BIT_SHR}))
        {
            Token::Type oper = _prev().type;
            NRef right = _termExpr();
            expr = Node::make_ref(Expr::Binary(std::move(expr), oper, std::move(right)));
        }
        return expr;
    }
    NRef Parser<PT_RD>::_termExpr()
    {
        NRef expr = _factorExpr();
        while (_match({Token::TK_ADD, Token::TK_SUB}))
        {
            Token::Type oper = _prev().type;
            NRef right = _factorExpr();
            expr = Node::make_ref(Expr::Binary(std::move(expr), oper, std::move(right)));
        }
        return expr;
    }
    NRef Parser<PT_RD>::_factorExpr()
    {
        NRef expr = _powExpr();
        while (_match({Token::TK_MUL, Token::TK_DIV, Token::TK_MOD}))
        {
            Token::Type oper = _prev().type;
            NRef right = _powExpr();
            expr = Node::make_ref(Expr::Binary(std::move(expr), oper, std::move(right)));
        }
        return expr;
    }
    NRef Parser<PT_RD>::_powExpr()
    {
        NRef expr = _unaryExpr();
        if (_match({Token::TK_POW}))
        {
            Token::Type oper = _prev().type;
            NRef right = _powExpr();
            expr = Node::make_ref(Expr::Binary{std::move(expr), oper, std::move(right)});
        }
        return expr;
    }
    NRef Parser<PT_RD>::_unaryExpr()
    {
        if (_match({Token::TK_SUB, Token::TK_NOT, Token::TK_BIT_NOT}))
        {
            Token::Type oper = _prev().type;
            NRef right = _unaryExpr();
            return Node::make_ref(Expr::Unary(oper, std::move(right)));
        }
        return _primaryExpr();
    }
    NRef Parser<PT_RD>::_primaryExpr()
    {   
        if (_match({
            Token::TK_NIL,
            Token::TK_LIT_INT, Token::TK_LIT_FLOAT, Token::TK_TRUE, Token::TK_FALSE,
            Token::TK_LIT_STRING
        }))
        {
            return Node::make_ref(Expr::Literal{_prev().literal});
        }
        else if (_match({Token::TK_LPAREN}))
        {
            NRef expr = _expression();
            _consume(Token::TK_RPAREN, "括号未闭合");
            return Node::make_ref(Expr::Grouping{std::move(expr)});
        }
        return _varNameExpr();
    }
    ast::NRef Parser<PT_RD>::_varNameExpr()
    {
        if (_match({Token::TK_IDENT}))
        {
            if (_check({
                Token::TK_ASSIGN, Token::TK_WALRUS,
                Token::TK_SELF_ADD, Token::TK_SELF_SUB, Token::TK_SELF_MUL, Token::TK_SELF_DIV, Token::TK_SELF_MOD, Token::TK_SELF_POW,
                Token::TK_SELF_BIT_AND, Token::TK_SELF_BIT_OR, Token::TK_SELF_BIT_XOR, Token::TK_SELF_BIT_XNOR, Token::TK_SELF_BIT_SHL, Token::TK_SELF_BIT_SHR
            }))
                return Node::make_ref(Expr::VarName{std::string(_prev().lexeme), true});
            return Node::make_ref(Expr::VarName{std::string(_prev().lexeme), false});
        }
        _error_reporter->report("缺少表达式", _advance().pos);
        return nullptr;
    }
#pragma endregion
#pragma region Stmt
ast::NRef Parser<PT_RD>::_statement()
{
    if (_match({Token::TK_VAR, Token::TK_LET}))
        return _varDeclStmt();
    else if (_match({Token::TK_PRINT, Token::TK_PRINTLN}))
        return _printStmt();
    else if (_match({Token::TK_LBRACE}))
        return _blockStmt();
    else if (_match({Token::TK_IF}))
        return _ifelseStmt();
    else if (_match({Token::TK_LOOP}))
        return _loopStmt();
    else if (_match({Token::TK_SEMICOLON}))
        return nullptr;
    return _expressionStmt();
}
ast::NRef Parser<PT_RD>::_varDeclStmt()
{
    std::string name = std::string(_consume(Token::TK_IDENT, "缺少变量名").lexeme);
    bool is_const = (_prev().type == Token::TK_LET);
    NRef initializer = nullptr;
    if (_match({Token::TK_ASSIGN}))
        initializer = _expression();
    _consume(Token::TK_SEMICOLON, "变量声明语句后要有';'结尾");
    return Node::make_ref(Stmt::VarDecl{name, is_const, std::move(initializer)});
}
ast::NRef Parser<PT_RD>::_printStmt()
{
    Token::Unit punit = _prev();
    NRef val = _expression();
    if (val == nullptr)
        _error_reporter->report("打印语句缺失表达式", punit.pos);
    _consume(Token::TK_SEMICOLON, "打印语句后要有';'结尾");
    return Node::make_ref(Stmt::Print{punit.type, std::move(val)});
}
ast::NRef Parser<PT_RD>::_expressionStmt()
{
    ast::NRef expr = _expression();
    _consume(Token::TK_SEMICOLON, "表达式语句后要有';'结尾");
    return Node::make_ref(Stmt::Expression{std::move(expr)});
}
ast::NRef Parser<PT_RD>::_blockStmt()
{
    std::vector<NRef> statements {};
    while (!_isAtEnd() && !_check(Token::TK_RBRACE))
    {
        NRef nr = _statement();
        if (nr)
            statements.push_back(std::move(nr));
    }
    _consume(Token::TK_RBRACE, "语句块未封闭");
    return Node::make_ref(Stmt::Block{std::move(statements)});
}
ast::NRef Parser<PT_RD>::_ifelseStmt()
{
    NRef condition = _expression();
    NRef thenBlock = _statement();
    if (!thenBlock)
    {
        _error_reporter->report("则语句块不能为空", _prev().pos);
        return nullptr;
    }
    if (_match({Token::TK_ELSE}))
        return Node::make_ref(Stmt::Ifelse{std::move(condition), std::move(thenBlock), std::move(_statement())});
    else if (_match({Token::TK_ELIF}))
        return Node::make_ref(Stmt::Ifelse{std::move(condition), std::move(thenBlock), std::move(_ifelseStmt())});
    return Node::make_ref(Stmt::Ifelse{std::move(condition), std::move(thenBlock), nullptr});
}
ast::NRef Parser<PT_RD>::_loopStmt()
{
    NRef condition = _expression();
    NRef doBlock = _statement();
    return Node::make_ref(Stmt::Loop{std::move(condition), std::move(doBlock)});
}
void Parser<PT_RD>::_synchronize()
{
    using namespace Token;
    _advance();
    while (!_isAtEnd())
    {
        if (_prev().type == TK_SEMICOLON) return;
        switch (_peek().type)
        {
        case TK_DEF:
        case TK_VAR:
        case TK_LET:
        case TK_REF:
        case TK_TYPE_INT:
        case TK_TYPE_FLOAT:
        case TK_TYPE_STRING:
        case TK_PACKAGE:
        case TK_IMPORT:
        case TK_IF:
        case TK_ELIF:
        case TK_ELSE:
        case TK_LOOP:
        case TK_CONTINUE:
        case TK_BREAK:
        case TK_FUNC:
        case TK_RETURN:
        case TK_STRUCT:
        case TK_NAMESPACE:
        case TK_GLOBAL:
        case TK_LOCAL:
        case TK_INTERFACE:
        case TK_CLASS:
        case TK_THIS:
        case TK_SUPER:
        case TK_STATIC:
        case TK_PUBLIC:
        case TK_PROTECTED:
        case TK_PRIVATE:
        case TK_VIRTUAL:
        case TK_OVERRIDE:
        case TK_IS:
        case TK_ENTITY:
        case TK_COMPONENT:
        case TK_HAS:
        case TK_NEW:
        case TK_DEL:
        case TK_PRINT:
        case TK_PRINTLN:
            return;
        default:
            break;
        }
        _advance();
    }
}
#pragma endregion
#pragma region Kan/Move
    bool Parser<PT_RD>::_isAtEnd() const
    {
        return _tokens.at(_cursor.current).type == Token::TK_EOF;
    }
    Token::Unit& Parser<PT_RD>::_advance()
    {
        if (!_isAtEnd()) _cursor.current++;
        return _prev();
    }
    void Parser<PT_RD>::_pass()
    {
        if (!_isAtEnd()) _cursor.current++;
    }
    Token::Unit& Parser<PT_RD>::_prev()
    {
        return _tokens.at(_cursor.current - 1);
    }
    Token::Unit& Parser<PT_RD>::_peek()
    {
        return _tokens.at(_cursor.current);
    }
    Token::Unit& Parser<PT_RD>::_next()
    {
        return _tokens.at(_cursor.current + 1);
    }
    bool Parser<PT_RD>::_check(Token::Type type)
    {
        if (_isAtEnd()) return false;
        return _peek().type == type;
    }
    bool Parser<PT_RD>::_check(std::initializer_list<Token::Type> types)
    {
        for (Token::Type type : types)
            if (_check(type))
                return true;
        return false;
    }
    bool Parser<PT_RD>::_match(std::initializer_list<Token::Type> types)
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
    Token::Unit& Parser<PT_RD>::_consume(Token::Type type, const std::string& message)
    {
        if (_check(type))
            return _advance();
        _error_reporter->report(message, _peek().pos);
        return _peek();
    }
#pragma endregion
}
