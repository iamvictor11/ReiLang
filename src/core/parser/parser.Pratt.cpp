#include "parser.Pratt.hpp"
#include <format>
#include <iostream>

namespace luna
{
    using namespace Token;
    using namespace Precedence;
    Parser<PT_Pratt>::_Rule Parser<PT_Pratt>::_rules_s[] =
    {
        /* 注释 */
        [TK_NOTE]  = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        /* 基础 */
        [TK_ASSIGN]    = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_NONE, 0},
        [TK_WALRUS]    = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_NONE, 0},
        /* 标识符 */
        [TK_IDENT] = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        /* 字面量 */
        [TK_LIT_INT]    = {&Parser<PT_Pratt>::_primary, nullptr, nullptr, PREC_NONE, 0},
        [TK_LIT_FLOAT]  = {&Parser<PT_Pratt>::_primary, nullptr, nullptr, PREC_NONE, 0},
        [TK_LIT_STRING] = {&Parser<PT_Pratt>::_primary, nullptr, nullptr, PREC_NONE, 0},
        /* 数学运算 */
        [TK_ADD]   = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_TERM, 1},
        [TK_SUB]   = {&Parser<PT_Pratt>::_unary, &Parser<PT_Pratt>::_binary, nullptr, PREC_TERM, 1},
        [TK_MUL]   = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_FACTOR, 1},
        [TK_DIV]   = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_FACTOR, 1},
        [TK_MOD]   = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_FACTOR, 1},
        [TK_POW]   = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_POW, 1},
        [TK_SELF_ADD]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_SUB]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_MUL]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_DIV]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_MOD]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_POW]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        /* 位运算 */
        [TK_BIT_AND]   = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_BAND, 1},
        [TK_BIT_OR]    = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_BOR, 1},
        [TK_BIT_XOR]   = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_BXOR, 1},
        [TK_BIT_XNOR]  = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_BXOR, 1},
        [TK_BIT_NOT]   = {&Parser<PT_Pratt>::_unary, nullptr, nullptr, PREC_UNARY, 1},
        [TK_BIT_SHL]   = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_SHIFT, 1},
        [TK_BIT_SHR]   = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_SHIFT, 1},
        [TK_SELF_BIT_AND]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_OR]   = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_XOR]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_XNOR] = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_NOT]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_SHL]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_SHR]  = {nullptr, &Parser<PT_Pratt>::_assign, nullptr, PREC_ASSIGN, 0},
        /* 比较运算 */
        [TK_EQ]    = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_EQUAL, 1},
        [TK_NE]    = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_EQUAL, 1},
        [TK_LT]    = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_COMPAR, 1},
        [TK_LE]    = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_COMPAR, 1},
        [TK_GT]    = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_COMPAR, 1},
        [TK_GE]    = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_COMPAR, 1},
        /* 逻辑运算 */
        [TK_AND]   = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_LAND, 1},
        [TK_OR]    = {nullptr, &Parser<PT_Pratt>::_binary, nullptr, PREC_LOR, 1},
        [TK_NOT]   = {&Parser<PT_Pratt>::_unary, nullptr, nullptr, PREC_UNARY, 1},
        /* 保留词 */
        [TK_NIL]   = {&Parser<PT_Pratt>::_primary, nullptr, nullptr, PREC_NONE, 1},
        [TK_TRUE]  = {&Parser<PT_Pratt>::_primary, nullptr, nullptr, PREC_NONE, 1},
        [TK_FALSE] = {&Parser<PT_Pratt>::_primary, nullptr, nullptr, PREC_NONE, 1},
        [TK_DEF]   = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_VAR]   = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_LET]   = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_REF]   = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_TYPE_INT]      = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_TYPE_FLOAT]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_TYPE_STRING]   = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_PACKAGE]   = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_IMPORT]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_IF]        = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_ELIF]      = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_ELSE]      = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_LOOP]      = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_CONTINUE]  = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_BREAK]     = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_FUNC]      = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_RETURN]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_STRUCT]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_NAMESPACE] = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_GLOBAL]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_LOCAL]     = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_INTERFACE] = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_CLASS]     = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_THIS]      = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_SUPER]     = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_STATIC]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_PUBLIC]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_PROTECTED] = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_PRIVATE]   = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_VIRTUAL]   = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_OVERRIDE]  = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_IS]        = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_ENTITY]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_COMPONENT] = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_HAS]       = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_NEW]       = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_DEL]       = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_PRINT]     = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_PRINTLN]   = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        /* 区域 */
        [TK_LPAREN]    = {&Parser<PT_Pratt>::_grouping, nullptr, nullptr, PREC_NONE, 1},
        [TK_RPAREN]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_LBRACKET]  = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_RBRACKET]  = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_LBRACE]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_RBRACE]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        /* 其他 */
        [TK_DOT]       = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_COMMA]     = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_SEMICOLON] = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_COLON]     = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_DCOLON]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_RARROW]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_LARROW]    = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        /* 结束 */
        [TK_EOF]   = {nullptr, nullptr, nullptr, PREC_NONE}
    };
    
    void Parser<PT_Pratt>::start()
    {
        _advance();
        _program();
    }
    void Parser<PT_Pratt>::_program()
    {
        while (!_isAtEnd())
            _statement();
    }
    void Parser<PT_Pratt>::_statement()
    {
        if (_match({TK_VAR, TK_LET}))
        {
        }
        else if (_match({TK_IF}))
        {
        }
        else if (_match({TK_LOOP}))
        {
        }
        else if (_match({TK_RETURN}))
        {
            if (!_match({TK_SEMICOLON}))
            {
                _expression();
            }
        }
        else
        {
            _expression();
        }
    }
    void Parser<PT_Pratt>::_expression()
    {
        _parsePrecedence(PREC_ASSIGN);
    }
    void Parser<PT_Pratt>::_parsePrecedence(Precedence::Level precedence)
    {
        _advance();
        _Fnuc prefixRule = _rules_s[_prev().type].prefix;
        if (prefixRule == nullptr)
        {
            _error_reporter->report("期望前缀表达式", _prev().pos);
            return;
        }
        (this->*prefixRule)();
        while (precedence <= _rules_s[_peek().type].precedence)
        {
            _advance();
            _Fnuc infixRule = _rules_s[_prev().type].infix;
            if (infixRule == nullptr) break;
            (this->*infixRule)();
        }
    }
#pragma region Expr
    void Parser<PT_Pratt>::_assign()
    {
        Token::Type operatorType = _prev().type;
        switch (operatorType)
        {
            case TK_ASSIGN:
            {
                _parsePrecedence(PREC_ASSIGN);
                _chunk->codes.push_back(Opcode::OP_SET_GLOBAL);
                break;
            }
            case TK_WALRUS:
            {
                _error_reporter->report("定义赋值暂未实现", _prev().pos);
                break;
            }
            case TK_SELF_ADD:
            case TK_SELF_SUB:
            case TK_SELF_MUL:
            case TK_SELF_DIV:
            case TK_SELF_MOD:
            case TK_SELF_POW:
            case TK_SELF_BIT_AND:
            case TK_SELF_BIT_OR:
            case TK_SELF_BIT_XOR:
            case TK_SELF_BIT_XNOR:
            case TK_SELF_BIT_NOT:
            case TK_SELF_BIT_SHL:
            case TK_SELF_BIT_SHR:
            {
                _error_reporter->report("复合赋值暂未实现", _prev().pos);
                break;
            }
            default:
                _error_reporter->report("未知的赋值运算符", _prev().pos);
        }
    }
    void Parser<PT_Pratt>::_grouping()
    {
        _expression();
        _consume(TK_RPAREN, "期望')'结束分组表达式");
    }
    void Parser<PT_Pratt>::_unary()
    {
        Token::Type op = _prev().type;
        _parsePrecedence(PREC_UNARY);
        _expression();
        switch (op)
        {
            case TK_SUB:        _emit(Opcode::OP_NEG); break;
            case TK_BIT_NOT:    _emit(Opcode::OP_BIT_NOT); break;
            case TK_NOT:        _emit(Opcode::OP_NOT); break;
            default: _error_reporter->report("未知的一元运算符", _prev().pos);
        }
    }
    void Parser<PT_Pratt>::_binary()
    {
        Token::Type op = _prev().type;
        _Rule& rule = _rules_s[op];
        _parsePrecedence(static_cast<Level>(rule.precedence + rule.is_left_assoc));
        switch (op)
        {
            case TK_ADD:    _emit(Opcode::OP_ADD); break;
            case TK_SUB:    _emit(Opcode::OP_SUB); break;
            case TK_MUL:    _emit(Opcode::OP_MUL); break;
            case TK_DIV:    _emit(Opcode::OP_DIV); break;
            case TK_MOD:    _emit(Opcode::OP_MOD); break;
            case TK_POW:    _emit(Opcode::OP_POW); break;
            case TK_BIT_AND:    _emit(Opcode::OP_BIT_AND); break;
            case TK_BIT_OR:     _emit(Opcode::OP_BIT_OR); break;
            case TK_BIT_XOR:    _emit(Opcode::OP_BIT_XOR); break;
            case TK_BIT_XNOR:   _emit(Opcode::OP_BIT_XNOR); break;
            case TK_BIT_SHL:    _emit(Opcode::OP_BIT_SHL); break;
            case TK_BIT_SHR:    _emit(Opcode::OP_BIT_SHR); break;
            case TK_EQ:     _emit(Opcode::OP_EQ); break;
            case TK_NE:     _emit(Opcode::OP_NE); break;
            case TK_LT:     _emit(Opcode::OP_LT); break;
            case TK_LE:     _emit(Opcode::OP_LE); break;
            case TK_GT:     _emit(Opcode::OP_GT); break;
            case TK_GE:     _emit(Opcode::OP_GE); break;
            case TK_AND:    _emit(Opcode::OP_AND); break;
            case TK_OR:     _emit(Opcode::OP_OR); break;
            default: _error_reporter->report("未知的二元运算符", _prev().pos);
        }
    }
    void Parser<PT_Pratt>::_primary()
    {
        switch (_prev().type)
        {
            case TK_LIT_INT:
            case TK_LIT_FLOAT:
            case TK_LIT_STRING:
                _emit(Opcode::OP_CONSTANT);
                _emit(static_cast<Opcode>(_emit(_prev().literal)));
                break;
            case TK_NIL:    _emit(Opcode::OP_NIL); break;
            case TK_TRUE:   _emit(Opcode::OP_TRUE); break;
            case TK_FALSE:  _emit(Opcode::OP_FALSE); break;
            case TK_IDENT:
                _error_reporter->report("变量取值暂未实现", _prev().pos);
                break;
            default:
                _error_reporter->report("非法的初级表达式", _prev().pos);
        }
    }
#pragma endregion
#pragma region Kan/Move
    bool Parser<PT_Pratt>::_isAtEnd() const
    {
        return _cursor.curr == nullptr || _cursor.curr->type == TK_EOF;
    }
    Token::Unit& Parser<PT_Pratt>::_advance()
    {
        if (_cursor.curr == nullptr)
        {
            _cursor.prev = nullptr;
            _cursor.curr = &_tokens.front();
        }
        else if (_cursor.curr->type != TK_EOF)
        {
            _cursor.prev = _cursor.curr;
            _cursor.curr += 1;
        }
        return *_cursor.prev;
    }
    void Parser<PT_Pratt>::_pass()
    {
        if (!_isAtEnd())
            _advance();
    }
    Token::Unit& Parser<PT_Pratt>::_prev()
    {
        return *_cursor.prev;
    }
    Token::Unit& Parser<PT_Pratt>::_peek()
    {
        return *_cursor.curr;
    }
    bool Parser<PT_Pratt>::_check(Token::Type type)
    {
        if (_isAtEnd()) return false;
        return _cursor.curr->type == type;
    }
    bool Parser<PT_Pratt>::_check(std::initializer_list<Token::Type> types)
    {
        if (_isAtEnd()) return false;
        for (auto type : types)
        {
            if (_cursor.curr->type == type) return true;
        }
        return false;
    }
    bool Parser<PT_Pratt>::_match(std::initializer_list<Token::Type> types)
    {
        if (!_check(types)) return false;
        _advance();
        return true;
    }
    Token::Unit& Parser<PT_Pratt>::_consume(Token::Type type, const std::string& message)
    {
        if (_check(type)) return _advance();
        _error_reporter->report(message, _cursor.curr->pos);
        return *_cursor.curr;
    }
#pragma endregion
#pragma region Emit
    void Parser<PT_Pratt>::_emit(Opcode op)
    {
        _chunk->codes.push_back(op);
    }
    size_t Parser<PT_Pratt>::_emit(Value::Data value)
    {
        _chunk->constants.push_back(value);
        return _chunk->constants.size() - 1;
    }
#pragma endregion
}
