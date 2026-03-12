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
        [TK_NOTE]  = {NULL, NULL, NULL, PREC_NONE},
        /* 基础 */
        [TK_ASSIGN]    = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_NONE},
        [TK_WALRUS]    = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_NONE},
        /* 标识符 */
        [TK_IDENT] = {NULL, NULL, NULL, PREC_NONE},
        /* 字面量 */
        [TK_LIT_INT]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_LIT_FLOAT]  = {NULL, NULL, NULL, PREC_NONE},
        [TK_LIT_STRING] = {NULL, NULL, NULL, PREC_NONE},
        /* 数学运算 */
        [TK_ADD]   = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_TERM},
        [TK_SUB]   = {Parser<PT_Pratt>::_unary, Parser<PT_Pratt>::_binary, NULL, PREC_TERM},
        [TK_MUL]   = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_FACTOR},
        [TK_DIV]   = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_FACTOR},
        [TK_MOD]   = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_FACTOR},
        [TK_POW]   = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_POW},
        [TK_SELF_ADD]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_SUB]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_MUL]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_DIV]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_MOD]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_POW]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        /* 位运算 */
        [TK_BIT_AND]   = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_BAND},
        [TK_BIT_OR]    = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_BOR},
        [TK_BIT_XOR]   = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_BXOR},
        [TK_BIT_XNOR]  = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_BXOR},
        [TK_BIT_NOT]   = {Parser<PT_Pratt>::_unary, NULL, NULL, PREC_UNARY},
        [TK_BIT_SHL]   = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_SHIFT},
        [TK_BIT_SHR]   = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_SHIFT},
        [TK_SELF_BIT_AND]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_BIT_OR]   = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_BIT_XOR]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_BIT_XNOR] = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_BIT_NOT]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_BIT_SHL]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        [TK_SELF_BIT_SHR]  = {NULL, Parser<PT_Pratt>::_assign, NULL, PREC_ASSIGN},
        /* 比较运算 */
        [TK_EQ]    = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_EQUAL},
        [TK_NE]    = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_EQUAL},
        [TK_LT]    = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_COMPAR},
        [TK_LE]    = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_COMPAR},
        [TK_GT]    = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_COMPAR},
        [TK_GE]    = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_COMPAR},
        /* 逻辑运算 */
        [TK_AND]   = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_LAND},
        [TK_OR]    = {NULL, Parser<PT_Pratt>::_binary, NULL, PREC_LOR},
        [TK_NOT]   = {Parser<PT_Pratt>::_unary, NULL, NULL, PREC_UNARY},
        /* 保留词 */
        [TK_NIL]   = {Parser<PT_Pratt>::_primary, NULL, NULL, PREC_NONE},
        [TK_TRUE]  = {Parser<PT_Pratt>::_primary, NULL, NULL, PREC_NONE},
        [TK_FALSE] = {Parser<PT_Pratt>::_primary, NULL, NULL, PREC_NONE},
        [TK_DEF]   = {NULL, NULL, NULL, PREC_NONE},
        [TK_VAR]   = {NULL, NULL, NULL, PREC_NONE},
        [TK_LET]   = {NULL, NULL, NULL, PREC_NONE},
        [TK_REF]   = {NULL, NULL, NULL, PREC_NONE},
        [TK_TYPE_INT]      = {NULL, NULL, NULL, PREC_NONE},
        [TK_TYPE_FLOAT]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_TYPE_STRING]   = {NULL, NULL, NULL, PREC_NONE},
        [TK_PACKAGE]   = {NULL, NULL, NULL, PREC_NONE},
        [TK_IMPORT]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_IF]        = {NULL, NULL, NULL, PREC_NONE},
        [TK_ELIF]      = {NULL, NULL, NULL, PREC_NONE},
        [TK_ELSE]      = {NULL, NULL, NULL, PREC_NONE},
        [TK_LOOP]      = {NULL, NULL, NULL, PREC_NONE},
        [TK_CONTINUE]  = {NULL, NULL, NULL, PREC_NONE},
        [TK_BREAK]     = {NULL, NULL, NULL, PREC_NONE},
        [TK_FUNC]      = {NULL, NULL, NULL, PREC_NONE},
        [TK_RETURN]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_STRUCT]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_NAMESPACE] = {NULL, NULL, NULL, PREC_NONE},
        [TK_GLOBAL]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_LOCAL]     = {NULL, NULL, NULL, PREC_NONE},
        [TK_INTERFACE] = {NULL, NULL, NULL, PREC_NONE},
        [TK_CLASS]     = {NULL, NULL, NULL, PREC_NONE},
        [TK_THIS]      = {NULL, NULL, NULL, PREC_NONE},
        [TK_SUPER]     = {NULL, NULL, NULL, PREC_NONE},
        [TK_STATIC]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_PUBLIC]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_PROTECTED] = {NULL, NULL, NULL, PREC_NONE},
        [TK_PRIVATE]   = {NULL, NULL, NULL, PREC_NONE},
        [TK_VIRTUAL]   = {NULL, NULL, NULL, PREC_NONE},
        [TK_OVERRIDE]  = {NULL, NULL, NULL, PREC_NONE},
        [TK_IS]        = {NULL, NULL, NULL, PREC_NONE},
        [TK_ENTITY]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_COMPONENT] = {NULL, NULL, NULL, PREC_NONE},
        [TK_HAS]       = {NULL, NULL, NULL, PREC_NONE},
        [TK_NEW]       = {NULL, NULL, NULL, PREC_NONE},
        [TK_DEL]       = {NULL, NULL, NULL, PREC_NONE},
        [TK_PRINT]     = {NULL, NULL, NULL, PREC_NONE},
        [TK_PRINTLN]   = {NULL, NULL, NULL, PREC_NONE},
        /* 区域 */
        [TK_LPAREN]    = {Parser<PT_Pratt>::_grouping, NULL, NULL, PREC_NONE},
        [TK_RPAREN]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_LBRACKET]  = {NULL, NULL, NULL, PREC_NONE},
        [TK_RBRACKET]  = {NULL, NULL, NULL, PREC_NONE},
        [TK_LBRACE]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_RBRACE]    = {NULL, NULL, NULL, PREC_NONE},
        /* 其他 */
        [TK_DOT]       = {NULL, NULL, NULL, PREC_NONE},
        [TK_COMMA]     = {NULL, NULL, NULL, PREC_NONE},
        [TK_SEMICOLON] = {NULL, NULL, NULL, PREC_NONE},
        [TK_COLON]     = {NULL, NULL, NULL, PREC_NONE},
        [TK_DCOLON]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_RARROW]    = {NULL, NULL, NULL, PREC_NONE},
        [TK_LARROW]    = {NULL, NULL, NULL, PREC_NONE},
        /* 结束 */
        [TK_EOF]   = {NULL, NULL, NULL, PREC_NONE}
    };
    
    void Parser<PT_Pratt>::start()
    {
        _advance(); // 初始化光标
        _program();
    }
    void Parser<PT_Pratt>::_program()
    {
        while (!_isAtEnd())
        {
            _statement();
            // 可选的分号分隔
            if (_match({TK_SEMICOLON}))
            {
                // 消耗分号
            }
        }
    }
    void Parser<PT_Pratt>::_statement()
    {
        if (_match({TK_VAR, TK_LET}))
        {
            // 变量声明
            // _varDeclaration();
        }
        else if (_match({TK_IF}))
        {
            // if 语句
        }
        else if (_match({TK_LOOP}))
        {
            // loop 语句
        }
        else if (_match({TK_RETURN}))
        {
            // return 语句
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
            _error_reporter->report("期望表达式", _prev().pos);
            return;
        }
        reinterpret_cast<void(*)()>(prefixRule)();
        while (precedence <= _rules_s[_peek().type].precdence)
        {
            _advance();
            _Fnuc infixRule = _rules_s[_prev().type].infix;
            if (infixRule == nullptr) break;
            reinterpret_cast<void(*)()>(infixRule)();
        }
        if (_rules_s[_peek().type].suffix != nullptr && precedence <= _rules_s[_peek().type].precdence)
        {
            _advance();
            _Fnuc suffixRule = _rules_s[_prev().type].suffix;
            reinterpret_cast<void(*)()>(suffixRule)();
        }
    }
#pragma region Expr
    void Parser<PT_Pratt>::_primary()
    {
        switch (_prev().type)
        {
            case TK_LIT_INT:
            case TK_LIT_FLOAT:
            case TK_LIT_STRING:
            case TK_NIL:
            case TK_TRUE:
            case TK_FALSE:
                _chunk->constants.push_back(_prev().literal);
                _chunk->codes.push_back(Opcode::OP_CONSTANT);
                _chunk->codes.push_back(static_cast<Opcode>(_chunk->constants.size() - 1));
                break;
            case TK_IDENT:
                _chunk->constants.push_back(_prev().literal);
                _chunk->codes.push_back(Opcode::OP_GET_GLOBAL);
                _chunk->codes.push_back(static_cast<Opcode>(_chunk->constants.size() - 1));
                break;
            default:
                _error_reporter->report("非法的初级表达式", _prev().pos);
        }
    }
    void Parser<PT_Pratt>::_grouping()
    {
        _expression();
        _consume(TK_RPAREN, "期望 ')' 结束分组表达式");
    }
    void Parser<PT_Pratt>::_unary()
    {
        Token::Type operatorType = _prev().type;
        _parsePrecedence(PREC_UNARY);
        switch (operatorType)
        {
            case TK_SUB:        _chunk->codes.push_back(Opcode::OP_NEG); break;
            case TK_BIT_NOT:    _chunk->codes.push_back(Opcode::OP_BIT_NOT); break;
            case TK_NOT:        _chunk->codes.push_back(Opcode::OP_NOT); break;
            default:
                _error_reporter->report("未知的一元运算符", _prev().pos);
        }
    }
    void Parser<PT_Pratt>::_binary()
    {
        Token::Type operatorType = _prev().type;
        Level currentPrec = _rules_s[operatorType].precdence;
        _parsePrecedence(static_cast<Level>(currentPrec + 1));
        switch (operatorType)
        {
            case TK_ADD:    _chunk->codes.push_back(Opcode::OP_ADD); break;
            case TK_SUB:    _chunk->codes.push_back(Opcode::OP_SUB); break;
            case TK_MUL:    _chunk->codes.push_back(Opcode::OP_MUL); break;
            case TK_DIV:    _chunk->codes.push_back(Opcode::OP_DIV); break;
            case TK_MOD:    _chunk->codes.push_back(Opcode::OP_MOD); break;
            case TK_POW:    _chunk->codes.push_back(Opcode::OP_POW); break;
            case TK_BIT_AND:    _chunk->codes.push_back(Opcode::OP_BIT_AND); break;
            case TK_BIT_OR:     _chunk->codes.push_back(Opcode::OP_BIT_OR); break;
            case TK_BIT_XOR:    _chunk->codes.push_back(Opcode::OP_BIT_XOR); break;
            case TK_BIT_XNOR:   _chunk->codes.push_back(Opcode::OP_BIT_XNOR); break;
            case TK_BIT_SHL:    _chunk->codes.push_back(Opcode::OP_BIT_SHL); break;
            case TK_BIT_SHR:    _chunk->codes.push_back(Opcode::OP_BIT_SHR); break;
            case TK_EQ:     _chunk->codes.push_back(Opcode::OP_EQ); break;
            case TK_NE:     _chunk->codes.push_back(Opcode::OP_NE); break;
            case TK_LT:     _chunk->codes.push_back(Opcode::OP_LT); break;
            case TK_LE:     _chunk->codes.push_back(Opcode::OP_LE); break;
            case TK_GT:     _chunk->codes.push_back(Opcode::OP_GT); break;
            case TK_GE:     _chunk->codes.push_back(Opcode::OP_GE); break;
            case TK_AND:    _chunk->codes.push_back(Opcode::OP_AND); break;
            case TK_OR:     _chunk->codes.push_back(Opcode::OP_OR); break;
            default: _error_reporter->report("未知的二元运算符", _prev().pos);
        }
    }
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
            _cursor.curr = &_tokens.front();
        }
        else if (_cursor.curr->type != TK_EOF)
        {
            _cursor.prev = _cursor.curr;
            _cursor.curr = _cursor.next;
        }

        if (_cursor.curr && _cursor.next)
        {
            _cursor.next = _cursor.next;
        }
        else
        {
            _cursor.next = nullptr;
        }

        return *_cursor.curr;
    }
    void Parser<PT_Pratt>::_pass()
    {
        if (_cursor.curr)
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
    Token::Unit& Parser<PT_Pratt>::_next()
    {
        return *_cursor.next;
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
}
