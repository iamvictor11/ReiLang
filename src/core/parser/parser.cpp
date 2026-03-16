#include "parser.hpp"
#include <format>
#include <iostream>

namespace rei
{
    using namespace Token;
    Parser::_Rule Parser::_rules_s[] =
    {
        /* 注释 */
        [TK_NOTE]  = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        /* 基础 */
        [TK_ASSIGN]    = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_WALRUS]    = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        /* 标识符 */
        [TK_IDENT] = {&Parser::_varExpr, nullptr, nullptr, PREC_NONE, 0},
        /* 字面量 */
        [TK_LIT_INT]    = {&Parser::_primaryExpr, nullptr, nullptr, PREC_NONE, 0},
        [TK_LIT_FLOAT]  = {&Parser::_primaryExpr, nullptr, nullptr, PREC_NONE, 0},
        [TK_LIT_STRING] = {&Parser::_primaryExpr, nullptr, nullptr, PREC_NONE, 0},
        /* 数学运算 */
        [TK_ADD]   = {nullptr, &Parser::_binaryExpr, nullptr, PREC_TERM, 1},
        [TK_SUB]   = {&Parser::_unaryExpr, &Parser::_binaryExpr, nullptr, PREC_TERM, 1},
        [TK_MUL]   = {nullptr, &Parser::_binaryExpr, nullptr, PREC_FACTOR, 1},
        [TK_DIV]   = {nullptr, &Parser::_binaryExpr, nullptr, PREC_FACTOR, 1},
        [TK_MOD]   = {nullptr, &Parser::_binaryExpr, nullptr, PREC_FACTOR, 1},
        [TK_POW]   = {nullptr, &Parser::_binaryExpr, nullptr, PREC_POW, 1},
        [TK_SELF_ADD]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_SUB]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_MUL]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_DIV]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_MOD]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_POW]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        /* 位运算 */
        [TK_BIT_AND]   = {nullptr, &Parser::_binaryExpr, nullptr, PREC_BAND, 1},
        [TK_BIT_OR]    = {nullptr, &Parser::_binaryExpr, nullptr, PREC_BOR, 1},
        [TK_BIT_XOR]   = {nullptr, &Parser::_binaryExpr, nullptr, PREC_BXOR, 1},
        [TK_BIT_XNOR]  = {nullptr, &Parser::_binaryExpr, nullptr, PREC_BXOR, 1},
        [TK_BIT_NOT]   = {&Parser::_unaryExpr, nullptr, nullptr, PREC_UNARY, 1},
        [TK_BIT_SHL]   = {nullptr, &Parser::_binaryExpr, nullptr, PREC_SHIFT, 1},
        [TK_BIT_SHR]   = {nullptr, &Parser::_binaryExpr, nullptr, PREC_SHIFT, 1},
        [TK_SELF_BIT_AND]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_OR]   = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_XOR]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_XNOR] = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_NOT]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_SHL]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_SHR]  = {nullptr, &Parser::_assignExpr, nullptr, PREC_ASSIGN, 0},
        /* 比较运算 */
        [TK_EQ]    = {nullptr, &Parser::_binaryExpr, nullptr, PREC_EQUAL, 1},
        [TK_NE]    = {nullptr, &Parser::_binaryExpr, nullptr, PREC_EQUAL, 1},
        [TK_LT]    = {nullptr, &Parser::_binaryExpr, nullptr, PREC_COMPAR, 1},
        [TK_LE]    = {nullptr, &Parser::_binaryExpr, nullptr, PREC_COMPAR, 1},
        [TK_GT]    = {nullptr, &Parser::_binaryExpr, nullptr, PREC_COMPAR, 1},
        [TK_GE]    = {nullptr, &Parser::_binaryExpr, nullptr, PREC_COMPAR, 1},
        /* 逻辑运算 */
        [TK_AND]   = {nullptr, &Parser::_binaryExpr, nullptr, PREC_LAND, 1},
        [TK_OR]    = {nullptr, &Parser::_binaryExpr, nullptr, PREC_LOR, 1},
        [TK_NOT]   = {&Parser::_unaryExpr, nullptr, nullptr, PREC_UNARY, 1},
        /* 保留词 */
        [TK_NIL]   = {&Parser::_primaryExpr, nullptr, nullptr, PREC_NONE, 1},
        [TK_TRUE]  = {&Parser::_primaryExpr, nullptr, nullptr, PREC_NONE, 1},
        [TK_FALSE] = {&Parser::_primaryExpr, nullptr, nullptr, PREC_NONE, 1},
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
        [TK_UNTIL]     = {nullptr, nullptr, nullptr, PREC_NONE, 0},
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
        [TK_LPAREN]    = {&Parser::_groupingExpr, nullptr, nullptr, PREC_NONE, 1},
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
        [TK_EOF]   = {nullptr, nullptr, nullptr, PREC_NONE, 0}
    };
    
    void Parser::start()
    {
        _advance();
        _program();
    }
    void Parser::_program()
    {
        while (!_isAtEnd())
            _declaration();
    }
    void Parser::_parsePrecedence(Precedence precedence)
    {
        _advance();
        _MyFnuc prefixRule = _rules_s[_prev().type].prefix;
        if (prefixRule == nullptr)
        {
            _reporterError(std::format("{} 期望前缀表达式", Token::toString(_prev().type)));
            return;
        }
        (this->*prefixRule)();
        while (_rules_s[_peek().type].precedence >= precedence)
        {
            _advance();
            _MyFnuc infixRule = _rules_s[_prev().type].infix;
            if (infixRule == nullptr) break;
            (this->*infixRule)();
        }
    }
#pragma region Expr
    void Parser::_expression()
    {
        _parsePrecedence(PREC_ASSIGN);
    }
    void Parser::_assignExpr()
    {
        Token::Type operatorType = _prev().type;
        _parsePrecedence(PREC_ASSIGN);
        switch (operatorType)
        {
            case TK_ASSIGN: break;
            case TK_WALRUS:
            {
                _reporterError("定义赋值暂未实现");
                break;
            }
            case TK_SELF_ADD:   _emitB(Opcode::OP_ADD); break;
            case TK_SELF_SUB:   _emitB(Opcode::OP_SUB); break;
            case TK_SELF_MUL:   _emitB(Opcode::OP_MUL); break;
            case TK_SELF_DIV:   _emitB(Opcode::OP_DIV); break;
            case TK_SELF_MOD:   _emitB(Opcode::OP_MOD); break;
            case TK_SELF_POW:   _emitB(Opcode::OP_POW); break;
            case TK_SELF_BIT_AND:   _emitB(Opcode::OP_BIT_AND); break;
            case TK_SELF_BIT_OR:    _emitB(Opcode::OP_BIT_OR); break;
            case TK_SELF_BIT_XOR:   _emitB(Opcode::OP_BIT_XOR); break;
            case TK_SELF_BIT_XNOR:  _emitB(Opcode::OP_BIT_XNOR); break;
            case TK_SELF_BIT_NOT:   _emitB(Opcode::OP_BIT_NOT); break;
            case TK_SELF_BIT_SHL:   _emitB(Opcode::OP_BIT_SHL); break;
            case TK_SELF_BIT_SHR:   _emitB(Opcode::OP_BIT_SHR); break;
            default:
                _reporterError("未知的赋值运算符");
        }
    }
    void Parser::_groupingExpr()
    {
        _expression();
        _consume(TK_RPAREN, "分组表达式期望以')'结束");
    }
    void Parser::_unaryExpr()
    {
        Token::Type op = _prev().type;
        _parsePrecedence(PREC_UNARY);
        switch (op)
        {
            case TK_SUB:        _emitB(Opcode::OP_NEG); break;
            case TK_BIT_NOT:    _emitB(Opcode::OP_BIT_NOT); break;
            case TK_NOT:        _emitB(Opcode::OP_NOT); break;
            default: _reporterError("未知的一元运算符");
        }
    }
    void Parser::_binaryExpr()
    {
        Token::Type op = _prev().type;
        _Rule& rule = _rules_s[op];
        if (op == TK_AND || op == TK_OR)
        {
            Bytecode jump_pos = _chunk->codes.size();
            _emitB(op == TK_AND ? OP_AND : OP_OR);
            _emitB(0);
            _parsePrecedence(static_cast<Precedence>(rule.precedence + rule.is_left_assoc));
            _patchB(jump_pos + 1, _chunk->codes.size() - (jump_pos + 2));
            return;
        }
        _parsePrecedence(static_cast<Precedence>(rule.precedence + rule.is_left_assoc));
        switch (op)
        {
            case TK_ADD:    _emitB(Opcode::OP_ADD); break;
            case TK_SUB:    _emitB(Opcode::OP_SUB); break;
            case TK_MUL:    _emitB(Opcode::OP_MUL); break;
            case TK_DIV:    _emitB(Opcode::OP_DIV); break;
            case TK_MOD:    _emitB(Opcode::OP_MOD); break;
            case TK_POW:    _emitB(Opcode::OP_POW); break;
            case TK_BIT_AND:    _emitB(Opcode::OP_BIT_AND); break;
            case TK_BIT_OR:     _emitB(Opcode::OP_BIT_OR); break;
            case TK_BIT_XOR:    _emitB(Opcode::OP_BIT_XOR); break;
            case TK_BIT_XNOR:   _emitB(Opcode::OP_BIT_XNOR); break;
            case TK_BIT_SHL:    _emitB(Opcode::OP_BIT_SHL); break;
            case TK_BIT_SHR:    _emitB(Opcode::OP_BIT_SHR); break;
            case TK_EQ:     _emitB(Opcode::OP_EQ); break;
            case TK_NE:     _emitB(Opcode::OP_NE); break;
            case TK_LT:     _emitB(Opcode::OP_LT); break;
            case TK_LE:     _emitB(Opcode::OP_LE); break;
            case TK_GT:     _emitB(Opcode::OP_GT); break;
            case TK_GE:     _emitB(Opcode::OP_GE); break;
            default: _reporterError("未知的二元运算符");
        }
    }
    void Parser::_primaryExpr()
    {
        switch (_prev().type)
        {
            case TK_LIT_INT:
            case TK_LIT_FLOAT:
            case TK_LIT_STRING:
                _emitB(Opcode::OP_CONSTANT);
                _emitB(_emitC(_prev().literal));
                break;
            case TK_NIL:    _emitB(Opcode::OP_NIL); break;
            case TK_TRUE:   _emitB(Opcode::OP_TRUE); break;
            case TK_FALSE:  _emitB(Opcode::OP_FALSE); break;
            default:
                _reporterError("非法的初级表达式");
        }
    }
    void Parser::_varExpr()
    {
        Token::Unit& vu = _prev();
        if (_match({TK_ASSIGN}))
        {
            _assignExpr();
            Env::Coord varc = _env->toCoord(std::string{vu.lexeme});
            _emitB(OP_SET_VAR);
            _emitB(varc.depth);
            _emitB(varc.index);
        }
        else if (_match({TK_WALRUS}))
        {
            _env->def(std::string{vu.lexeme});
            _expression();
            _emitB(OP_DEF_VAR);
            _emitB(_emitC(std::string{vu.lexeme}));
        }
        else if (_match({
            TK_SELF_ADD, TK_SELF_SUB, TK_SELF_MUL, TK_SELF_DIV, TK_SELF_MOD, TK_SELF_POW,
            TK_SELF_BIT_AND, TK_SELF_BIT_OR, TK_SELF_BIT_XOR, TK_SELF_BIT_XNOR, TK_SELF_BIT_NOT, TK_SELF_BIT_SHL, TK_SELF_BIT_SHR
        }))
        {
            Env::Coord varc = _env->toCoord(std::string{vu.lexeme});
            _emitB(OP_GET_VAR);
            _emitB(varc.depth);
            _emitB(varc.index);
            _assignExpr();
            _emitB(OP_SET_VAR);
            _emitB(varc.depth);
            _emitB(varc.index);
        }
        else
        {
            Env::Coord varc = _env->toCoord(std::string{vu.lexeme});
            _emitB(OP_GET_VAR);
            _emitB(varc.depth);
            _emitB(varc.index);
        }
    }
#pragma endregion
#pragma region Stmt
    void Parser::_statement()
    {
        if (_match({TK_LBRACE}))
            _blockStmt();
        else if (_match({TK_IF}))
            _ifStmt();
        else if (_match({TK_LOOP, TK_UNTIL}))
            _loopStmt();
        else if (_match({TK_BREAK}))
            _breakStmt();
        else if (_match({TK_CONTINUE}))
            _continueStmt();
        else if (_match({TK_PRINT, TK_PRINTLN}))
            _printStmt();
        else if (_match({TK_SEMICOLON}))
        {
        }
        else
        {
            _exprStmt();
        }
    }
    void Parser::_exprStmt()
    {
        _expression();
        _consume(TK_SEMICOLON, "表达式语句期望以';'结束");
        _emitB(OP_POP);
    }
    void Parser::_blockStmt()
    {
        _env->enter();
        _emitB(OP_BEG);
        while (!_check(TK_RBRACE) && !_isAtEnd())
            _declaration();
        _consume(TK_RBRACE, "语句块期望以'}'结束");
        _env->exit();
        _emitB(OP_END);
    }
    void Parser::_ifStmt()
    {
        _expression();
        Bytecode if_jump_pos = _chunk->codes.size();
        _emitB(OP_JMPF);
        _emitB(0);
        _statement();
        if (_match({TK_ELSE}))
        {
            Bytecode else_jump_pos = _chunk->codes.size();
            _emitB(OP_JUMP);
            _emitB(0);
            _patchB(if_jump_pos + 1, _chunk->codes.size() - (if_jump_pos + 2));
            _statement();
            _patchB(else_jump_pos + 1, _chunk->codes.size() - (else_jump_pos + 2));
        }
        else if (_match({TK_ELIF}))
        {
            Bytecode else_jump_pos = _chunk->codes.size();
            _emitB(OP_JUMP);
            _emitB(0);
            _patchB(if_jump_pos + 1, _chunk->codes.size() - (if_jump_pos + 2));
            _ifStmt();
            _patchB(else_jump_pos + 1, _chunk->codes.size() - (else_jump_pos + 2));
        }
        else
        {
            _patchB(if_jump_pos + 1, _chunk->codes.size() - (if_jump_pos + 2));
        }
    }
    void Parser::_loopStmt()
    {
        Token::Type loop_tk = _prev().type;
        Bytecode start_pos = _chunk->codes.size();
        auto& loop = _loops.emplace_back();
        loop.start = start_pos;
        _expression();
        Bytecode loop_jump_pos = _chunk->codes.size();
        _emitB(loop_tk == TK_LOOP ? OP_JMPF : OP_JMPT);
        _emitB(0);
        loop.depth = _env->curr();
        _statement();
        _emitB(OP_JUMP);
        _emitB(start_pos - (_chunk->codes.size() + 1));
        Bytecode end_pos = _chunk->codes.size();
        _patchB(loop_jump_pos + 1, end_pos - (loop_jump_pos + 2));
        for (Bytecode bpos : _loops.back().breaks)
            _patchB(bpos + 1, end_pos - (bpos + 2));
        _loops.pop_back();
    }
    void Parser::_breakStmt()
    {
        if (_loops.empty())
        {
            _reporterError("中断语句不在循环中");
            return;
        }
        auto& loop = _loops.back();
        Bytecode diff = _env->curr() - loop.depth;
        for (Bytecode i = 0; i < diff; i++)
            _emitB(OP_END);
        Bytecode pos = _chunk->codes.size();
        _emitB(OP_JUMP);
        _emitB(0);
        loop.breaks.push_back(pos);
        _consume(TK_SEMICOLON, "中断语句期望以';'结束");
    }
    void Parser::_continueStmt()
    {
        if (_loops.empty())
        {
            _reporterError("继续语句不在循环中");
            return;
        }
        auto& loop = _loops.back();
        Bytecode diff = _env->curr() - loop.depth;
        for (Bytecode i = 0; i < diff; i++)
            _emitB(OP_END);
        Bytecode start = loop.start;
        _emitB(OP_JUMP);
        _emitB(start - _chunk->codes.size());
        _consume(TK_SEMICOLON, "继续语句期望以';'结束");
    }
    void Parser::_printStmt()
    {
        Opcode type = _prev().type == TK_PRINT ? OP_PRINT : OP_PRINTLN;
        _expression();
        _consume(TK_SEMICOLON, "打印语句期望以';'结束");
        _emitB(type);
    }
#pragma endregion
#pragma region Decl

    void Parser::_declaration()
    {
        if (_match({TK_VAR, TK_LET}))
            _varDecl();
        else if (_match({TK_FUNC}))
            _funcDecl();
        else if (_match({TK_STRUCT}))
            _structDecl();
        else if (_match({TK_CLASS}))
            _classDecl();
        else
            _statement();
    }
    void Parser::_varDecl()
    {
        _consume(TK_IDENT, "变量期望用标识符标记");
        Token::Unit& vu = _prev();
        _env->def(std::string{vu.lexeme});
        if (_match({TK_ASSIGN}))
            _expression();
        else
            _emitB(OP_NIL);
        _emitB(OP_DEF_VAR);
        _emitB(_emitC(std::string{vu.lexeme}));
        _emitB(OP_POP);
        _consume(TK_SEMICOLON, "变量声明语句期望以';'结束");
    }
    void Parser::_funcDecl()
    {
    }
    void Parser::_structDecl()
    {
    }
    void Parser::_classDecl()
    {
    }
#pragma endregion
#pragma region Kan/Move
    bool Parser::_isAtEnd() const
    {
        return _cursor.curr == nullptr || _cursor.curr->type == TK_EOF;
    }
    Token::Unit& Parser::_advance()
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
    void Parser::_pass()
    {
        if (!_isAtEnd())
            _advance();
    }
    Token::Unit& Parser::_prev()
    {
        return *_cursor.prev;
    }
    Token::Unit& Parser::_peek()
    {
        return *_cursor.curr;
    }
    bool Parser::_check(Token::Type type)
    {
        if (_isAtEnd()) return false;
        return _cursor.curr->type == type;
    }
    bool Parser::_check(std::initializer_list<Token::Type> types)
    {
        if (_isAtEnd()) return false;
        for (auto type : types)
            if (_cursor.curr->type == type)
                return true;
        return false;
    }
    bool Parser::_match(std::initializer_list<Token::Type> types)
    {
        if (!_check(types)) return false;
        _advance();
        return true;
    }
    Token::Unit& Parser::_consume(Token::Type type, const std::string& message)
    {
        if (_check(type)) return _advance();
        _reporterError(message);
        return *_cursor.curr;
    }
#pragma endregion
#pragma region Emit
    void Parser::_emitB(Bytecode op)
    {
        _chunk->codes.push_back(op);
    }
    Bytecode Parser::_emitC(Value::Data value)
    {
        if (_chunk->constants.size() >= REI_BYTECODE_MAX)
        {
            _reporterError("常数块溢出");
            return 0;
        }
        _chunk->constants.push_back(value);
        return static_cast<Bytecode>(_chunk->constants.size() - 1);
    }
    void Parser::_patchB(Bytecode pos, Bytecode op)
    {
        if (pos >= _chunk->codes.size())
        {
            _reporterError("修补位置超出字节码块范围");
            return;
        }
        _chunk->codes[pos] = static_cast<Bytecode>(op);
    }
#pragma endregion
    void Parser::_reporterError(const std::string& msg)
    {
        _error_reporter->report(std::format("{}: {}", Token::toString(_prev().type), msg), _prev().pos);
    }
}
