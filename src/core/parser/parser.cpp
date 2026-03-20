#include "parser.hpp"
#include <format>
#include <iostream>

namespace rei
{
    using namespace Token;
    Parser::Rule_ Parser::rules_s_[] =
    {
        /* 注释 */
        [TK_NOTE]  = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        /* 基础 */
        [TK_ASSIGN]    = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_WALRUS]    = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        /* 标识符 */
        [TK_IDENT] = {&Parser::varExpr_, nullptr, nullptr, PREC_PRIMARY, 0},
        /* 字面量 */
        [TK_LIT_INT]    = {&Parser::primaryExpr_, nullptr, nullptr, PREC_PRIMARY, 0},
        [TK_LIT_FLOAT]  = {&Parser::primaryExpr_, nullptr, nullptr, PREC_PRIMARY, 0},
        [TK_LIT_STRING] = {&Parser::primaryExpr_, nullptr, nullptr, PREC_PRIMARY, 0},
        /* 数学运算 */
        [TK_ADD]   = {nullptr, &Parser::binaryExpr_, nullptr, PREC_TERM, 1},
        [TK_SUB]   = {&Parser::unaryExpr_, &Parser::binaryExpr_, nullptr, PREC_TERM, 1},
        [TK_MUL]   = {nullptr, &Parser::binaryExpr_, nullptr, PREC_FACTOR, 1},
        [TK_DIV]   = {nullptr, &Parser::binaryExpr_, nullptr, PREC_FACTOR, 1},
        [TK_MOD]   = {nullptr, &Parser::binaryExpr_, nullptr, PREC_FACTOR, 1},
        [TK_POW]   = {nullptr, &Parser::binaryExpr_, nullptr, PREC_POW, 1},
        [TK_SELF_ADD]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_SUB]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_MUL]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_DIV]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_MOD]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_POW]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        /* 位运算 */
        [TK_BIT_AND]   = {nullptr, &Parser::binaryExpr_, nullptr, PREC_BAND, 1},
        [TK_BIT_OR]    = {nullptr, &Parser::binaryExpr_, nullptr, PREC_BOR, 1},
        [TK_BIT_XOR]   = {nullptr, &Parser::binaryExpr_, nullptr, PREC_BXOR, 1},
        [TK_BIT_XNOR]  = {nullptr, &Parser::binaryExpr_, nullptr, PREC_BXOR, 1},
        [TK_BIT_NOT]   = {&Parser::unaryExpr_, nullptr, nullptr, PREC_UNARY, 1},
        [TK_BIT_SHL]   = {nullptr, &Parser::binaryExpr_, nullptr, PREC_SHIFT, 1},
        [TK_BIT_SHR]   = {nullptr, &Parser::binaryExpr_, nullptr, PREC_SHIFT, 1},
        [TK_SELF_BIT_AND]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_OR]   = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_XOR]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_XNOR] = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_NOT]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_SHL]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_SELF_BIT_SHR]  = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        /* 比较运算 */
        [TK_EQ]    = {nullptr, &Parser::binaryExpr_, nullptr, PREC_EQUAL, 1},
        [TK_NE]    = {nullptr, &Parser::binaryExpr_, nullptr, PREC_EQUAL, 1},
        [TK_LT]    = {nullptr, &Parser::binaryExpr_, nullptr, PREC_COMPAR, 1},
        [TK_LE]    = {nullptr, &Parser::binaryExpr_, nullptr, PREC_COMPAR, 1},
        [TK_GT]    = {nullptr, &Parser::binaryExpr_, nullptr, PREC_COMPAR, 1},
        [TK_GE]    = {nullptr, &Parser::binaryExpr_, nullptr, PREC_COMPAR, 1},
        /* 逻辑运算 */
        [TK_AND]   = {nullptr, &Parser::binaryExpr_, nullptr, PREC_LAND, 1},
        [TK_OR]    = {nullptr, &Parser::binaryExpr_, nullptr, PREC_LOR, 1},
        [TK_NOT]   = {&Parser::unaryExpr_, nullptr, nullptr, PREC_UNARY, 1},
        /* 保留词 */
        [TK_NIL]   = {&Parser::primaryExpr_, nullptr, nullptr, PREC_NONE, 1},
        [TK_TRUE]  = {&Parser::primaryExpr_, nullptr, nullptr, PREC_NONE, 1},
        [TK_FALSE] = {&Parser::primaryExpr_, nullptr, nullptr, PREC_NONE, 1},
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
        [TK_LPAREN]    = {&Parser::groupingExpr_, nullptr, &Parser::callExpr_, PREC_CALL, 1},
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
        advance_();
        program_();
    }
    void Parser::program_()
    {
        while (!isAtEnd_() && _error_reporter->empty())
            declaration_();
        emitB_(OP_HALT);
    }
    void Parser::parsePrecedence_(Precedence precedence)
    {
        advance_();
        MyFnuc_ prefixRule = rules_s_[prev_().type].prefix;
        if (prefixRule == nullptr)
        {
            reporterError_(std::format("{} 不具有前缀表达式", Token::toString(prev_().type)));
            return;
        }
        (this->*prefixRule)();
        do
        {
            auto& rule = rules_s_[peek_().type];
            if (rule.precedence < precedence)
                break;
            MyFnuc_ suffixRule = rule.suffix;
            if (suffixRule != nullptr)
            {
                pass_();
                (this->*suffixRule)();
                continue;
            }
            MyFnuc_ infixRule = rule.infix;
            if (infixRule != nullptr)
            {
                pass_();
                (this->*infixRule)();
                continue;
            }
            break;
        }
        while (true);
    }
#pragma region Expr
    void Parser::expression_()
    {
        parsePrecedence_(PREC_ASSIGN);
    }
    void Parser::assignExpr_()
    {
        Token::Type operatorType = prev_().type;
        parsePrecedence_(PREC_ASSIGN);
        switch (operatorType)
        {
            case TK_ASSIGN:
            {
                reporterError_("你怎么做到的，'='的逻辑不会走这里");
                break;
            }
            case TK_WALRUS:
            {
                reporterError_("你怎么做到的，':='的逻辑不会走这里");
                break;
            }
            case TK_SELF_ADD:   emitB_(Opcode::OP_ADD); break;
            case TK_SELF_SUB:   emitB_(Opcode::OP_SUB); break;
            case TK_SELF_MUL:   emitB_(Opcode::OP_MUL); break;
            case TK_SELF_DIV:   emitB_(Opcode::OP_DIV); break;
            case TK_SELF_MOD:   emitB_(Opcode::OP_MOD); break;
            case TK_SELF_POW:   emitB_(Opcode::OP_POW); break;
            case TK_SELF_BIT_AND:   emitB_(Opcode::OP_BIT_AND); break;
            case TK_SELF_BIT_OR:    emitB_(Opcode::OP_BIT_OR); break;
            case TK_SELF_BIT_XOR:   emitB_(Opcode::OP_BIT_XOR); break;
            case TK_SELF_BIT_XNOR:  emitB_(Opcode::OP_BIT_XNOR); break;
            case TK_SELF_BIT_NOT:   emitB_(Opcode::OP_BIT_NOT); break;
            case TK_SELF_BIT_SHL:   emitB_(Opcode::OP_BIT_SHL); break;
            case TK_SELF_BIT_SHR:   emitB_(Opcode::OP_BIT_SHR); break;
            default:
                reporterError_("未知的赋值运算符");
        }
    }
    void Parser::groupingExpr_()
    {
        expression_();
        consume_(TK_RPAREN, "分组表达式期望以')'结束");
    }
    void Parser::unaryExpr_()
    {
        Token::Type op = prev_().type;
        parsePrecedence_(PREC_UNARY);
        switch (op)
        {
            case TK_SUB:        emitB_(Opcode::OP_NEG); break;
            case TK_BIT_NOT:    emitB_(Opcode::OP_BIT_NOT); break;
            case TK_NOT:        emitB_(Opcode::OP_NOT); break;
            default: reporterError_("未知的一元运算符");
        }
    }
    void Parser::binaryExpr_()
    {
        Token::Type op = prev_().type;
        Rule_& rule = rules_s_[op];
        if (op == TK_AND || op == TK_OR)
        {
            Bytecode jump_pos = chunk_->codes.size();
            emitB_(op == TK_AND ? OP_AND : OP_OR);
            emitB_(0);
            parsePrecedence_(static_cast<Precedence>(rule.precedence + rule.is_left_assoc));
            patchB_(jump_pos + 1, chunk_->codes.size() - (jump_pos + 2));
            return;
        }
        parsePrecedence_(static_cast<Precedence>(rule.precedence + rule.is_left_assoc));
        switch (op)
        {
            case TK_ADD:    emitB_(Opcode::OP_ADD); break;
            case TK_SUB:    emitB_(Opcode::OP_SUB); break;
            case TK_MUL:    emitB_(Opcode::OP_MUL); break;
            case TK_DIV:    emitB_(Opcode::OP_DIV); break;
            case TK_MOD:    emitB_(Opcode::OP_MOD); break;
            case TK_POW:    emitB_(Opcode::OP_POW); break;
            case TK_BIT_AND:    emitB_(Opcode::OP_BIT_AND); break;
            case TK_BIT_OR:     emitB_(Opcode::OP_BIT_OR); break;
            case TK_BIT_XOR:    emitB_(Opcode::OP_BIT_XOR); break;
            case TK_BIT_XNOR:   emitB_(Opcode::OP_BIT_XNOR); break;
            case TK_BIT_SHL:    emitB_(Opcode::OP_BIT_SHL); break;
            case TK_BIT_SHR:    emitB_(Opcode::OP_BIT_SHR); break;
            case TK_EQ:     emitB_(Opcode::OP_EQ); break;
            case TK_NE:     emitB_(Opcode::OP_NE); break;
            case TK_LT:     emitB_(Opcode::OP_LT); break;
            case TK_LE:     emitB_(Opcode::OP_LE); break;
            case TK_GT:     emitB_(Opcode::OP_GT); break;
            case TK_GE:     emitB_(Opcode::OP_GE); break;
            default: reporterError_("未知的二元运算符");
        }
    }
    void Parser::callExpr_()
    {
        Bytecode argc = 0;
        while (!check_({TK_RPAREN}) && !isAtEnd_())
        {
            expression_();
            argc++;
            if (argc > REI_FUNC_UPVALUE_COUNT_MAX)
                reporterError_(std::format("调用表达式传参数量超过最大值 {}", REI_FUNC_UPVALUE_COUNT_MAX));
            if (match_({TK_COMMA}))
                continue;
            break;
        }
        consume_(TK_RPAREN, "调用表达式期望以')'结束");
        emitB_(OP_CALL);
        emitB_(argc);
    }
    void Parser::primaryExpr_()
    {
        switch (prev_().type)
        {
            case TK_LIT_INT:
            case TK_LIT_FLOAT:
            case TK_LIT_STRING:
                emitB_(Opcode::OP_CONSTANT);
                emitC_(prev_().literal);
                break;
            case TK_NIL:    emitB_(Opcode::OP_NIL); break;
            case TK_TRUE:   emitB_(Opcode::OP_TRUE); break;
            case TK_FALSE:  emitB_(Opcode::OP_FALSE); break;
            default:
                reporterError_("非法的初级表达式");
        }
    }
    void Parser::varExpr_()
    {
        Token::Unit& vu = prev_();
        if (match_({TK_ASSIGN}))
        {
            expression_();
            Value::Coord varc = env_->toCoord(std::string{vu.lexeme});
            emitB_(OP_SET_VAR);
            emitB_(varc.lifecycle);
            emitB_(varc.uplevel);
            emitB_(varc.slot);
        }
        else if (match_({TK_WALRUS}))
        {
            Value::Coord varc = env_->def(std::string{vu.lexeme});
            expression_();
            emitB_(OP_DEF_VAR);
            emitB_(varc.lifecycle);
            emitB_(varc.uplevel);
            emitB_(varc.slot);
        }
        else if (match_({
            TK_SELF_ADD, TK_SELF_SUB, TK_SELF_MUL, TK_SELF_DIV, TK_SELF_MOD, TK_SELF_POW,
            TK_SELF_BIT_AND, TK_SELF_BIT_OR, TK_SELF_BIT_XOR, TK_SELF_BIT_XNOR, TK_SELF_BIT_NOT, TK_SELF_BIT_SHL, TK_SELF_BIT_SHR
        }))
        {
            Value::Coord varc = env_->toCoord(std::string{vu.lexeme});
            emitB_(OP_GET_VAR);
            emitB_(varc.lifecycle);
            emitB_(varc.uplevel);
            emitB_(varc.slot);
            assignExpr_();
            emitB_(OP_SET_VAR);
            emitB_(varc.lifecycle);
            emitB_(varc.uplevel);
            emitB_(varc.slot);
        }
        else
        {
            Value::Coord varc = env_->toCoord(std::string{vu.lexeme});
            emitB_(OP_GET_VAR);
            emitB_(varc.lifecycle);
            emitB_(varc.uplevel);
            emitB_(varc.slot);
        }
    }
#pragma endregion
#pragma region Stmt
    void Parser::statement_()
    {
        if (match_({TK_LBRACE}))
            _blockStmt();
        else if (match_({TK_IF}))
            ifStmt_();
        else if (match_({TK_LOOP, TK_UNTIL}))
            loopStmt_();
        else if (match_({TK_BREAK}))
            breakStmt_();
        else if (match_({TK_CONTINUE}))
            continueStmt_();
        else if (match_({TK_RETURN}))
            returnStmt_();
        else if (match_({TK_PRINT, TK_PRINTLN}))
            printStmt_();
        else if (match_({TK_SEMICOLON}))
        {
        }
        else
        {
            exprStmt_();
        }
    }
    void Parser::exprStmt_()
    {
        expression_();
        consume_(TK_SEMICOLON, "表达式语句期望以';'结束");
        emitB_(OP_POP);
    }
    void Parser::_blockStmt()
    {
        env_->enter();
        emitB_(OP_BEG);
        while (!check_(TK_RBRACE) && !isAtEnd_())
            declaration_();
        consume_(TK_RBRACE, "语句块期望以'}'结束");
        env_->exit();
        emitB_(OP_END);
    }
    void Parser::ifStmt_()
    {
        expression_();
        Bytecode if_jump_pos = chunk_->codes.size();
        emitB_(OP_JMPF);
        emitB_(0);
        env_->enter();
        emitB_(OP_BEG);
        statement_();
        env_->exit();
        emitB_(OP_END);
        if (match_({TK_ELSE}))
        {
            Bytecode else_jump_pos = chunk_->codes.size();
            emitB_(OP_JUMP);
            emitB_(0);
            patchB_(if_jump_pos + 1, chunk_->codes.size() - (if_jump_pos + 2));
            env_->enter();
            emitB_(OP_BEG);
            statement_();
            env_->exit();
            emitB_(OP_END);
            patchB_(else_jump_pos + 1, chunk_->codes.size() - (else_jump_pos + 2));
        }
        else if (match_({TK_ELIF}))
        {
            Bytecode else_jump_pos = chunk_->codes.size();
            emitB_(OP_JUMP);
            emitB_(0);
            patchB_(if_jump_pos + 1, chunk_->codes.size() - (if_jump_pos + 2));
            ifStmt_();
            patchB_(else_jump_pos + 1, chunk_->codes.size() - (else_jump_pos + 2));
        }
        else
        {
            patchB_(if_jump_pos + 1, chunk_->codes.size() - (if_jump_pos + 2));
        }
    }
    void Parser::loopStmt_()
    {
        Token::Type loop_tk = prev_().type;
        Bytecode start_pos = chunk_->codes.size();
        auto& loop_ctx = loop_ctxs_.emplace_back();
        loop_ctx.start = start_pos;
        expression_();
        Bytecode loop_jump_pos = chunk_->codes.size();
        emitB_(loop_tk == TK_LOOP ? OP_JMPF : OP_JMPT);
        emitB_(0);
        loop_ctx.depth = env_->currLocalDepth();
        env_->enter();
        emitB_(OP_BEG);
        statement_();
        env_->exit();
        emitB_(OP_END);
        emitB_(OP_JUMP);
        emitB_(start_pos - (chunk_->codes.size() + 1));
        Bytecode end_pos = chunk_->codes.size();
        patchB_(loop_jump_pos + 1, end_pos - (loop_jump_pos + 2));
        for (Bytecode bpos : loop_ctxs_.back().breaks)
            patchB_(bpos + 1, end_pos - (bpos + 2));
        loop_ctxs_.pop_back();
    }
    void Parser::breakStmt_()
    {
        if (loop_ctxs_.empty())
        {
            reporterError_("中断语句不在循环中");
            return;
        }
        Integer level = 1;
        if (match_({TK_LIT_INT}))
            level = Value::toInteger(prev_().literal);
        if (level <= 0 || level > loop_ctxs_.size())
        {
            reporterError_("循环层级超出范围");
            return;
        }
        auto& loop_ctx = loop_ctxs_.at(loop_ctxs_.size() - level);
        Bytecode diff = env_->currLocalDepth() - loop_ctx.depth;
        for (Bytecode i = 0; i < diff; i++)
            emitB_(OP_END);
        Bytecode pos = chunk_->codes.size();
        emitB_(OP_JUMP);
        emitB_(0);
        loop_ctx.breaks.push_back(pos);
        consume_(TK_SEMICOLON, "中断语句期望以';'结束");
    }
    void Parser::continueStmt_()
    {
        if (loop_ctxs_.empty())
        {
            reporterError_("继续语句不在循环中");
            return;
        }
        Integer level = 1;
        if (match_({TK_LIT_INT}))
            level = Value::toInteger(prev_().literal);
        if (level <= 0 || level > loop_ctxs_.size())
        {
            reporterError_("循环层级超出范围");
            return;
        }
        auto& loop_ctx = loop_ctxs_.at(loop_ctxs_.size() - level);
        Bytecode diff = env_->currLocalDepth() - loop_ctx.depth;
        for (Bytecode i = 0; i < diff; i++)
            emitB_(OP_END);
        Bytecode start = loop_ctx.start;
        emitB_(OP_JUMP);
        emitB_(start - (chunk_->codes.size() + 1));
        consume_(TK_SEMICOLON, "继续语句期望以';'结束");
    }
    void Parser::returnStmt_()
    {
        if (func_ctxs_.empty())
        {
            reporterError_("返回语句不在函数中");
            return;
        }
        auto& func_ctx = func_ctxs_.back();
        if (match_({TK_COLON}))
            expression_();
        else
            emitB_(OP_NIL);
        Bytecode diff = env_->currLocalDepth() - func_ctx.depth;
        for (Bytecode i = 0; i < diff; i++)
            emitB_(OP_END);
        emitB_(OP_RETURN);
        consume_(TK_SEMICOLON, "返回语句期望以';'结束");
    }
    void Parser::printStmt_()
    {
        Opcode type = prev_().type == TK_PRINT ? OP_PRINT : OP_PRINTLN;
        expression_();
        consume_(TK_SEMICOLON, "打印语句期望以';'结束");
        emitB_(type);
    }
#pragma endregion
#pragma region Decl
    void Parser::declaration_()
    {
        if (match_({TK_DEF, TK_VAR, TK_LET}))
            varDecl_();
        else if (match_({TK_FUNC}))
            funcDecl_();
        else if (match_({TK_STRUCT}))
            structDecl_();
        else if (match_({TK_CLASS}))
            classDecl_();
        else
            statement_();
    }
    void Parser::varDecl_()
    {
        consume_(TK_IDENT, "变量期望用标识符标记");
        std::string vname = std::string(prev_().lexeme);
        Value::Coord varc = env_->def(vname);
        if (match_({TK_ASSIGN}))
            expression_();
        else
            emitB_(OP_NIL);
        emitB_(OP_DEF_VAR);
        emitB_(varc.lifecycle);
        emitB_(varc.uplevel);
        emitB_(varc.slot);
        emitB_(OP_POP);
        consume_(TK_SEMICOLON, "变量声明语句期望以';'结束");
    }
    void Parser::funcDecl_()
    {
        Chunk* savedChunk = chunk_;
        consume_(TK_IDENT, "函数声明期望函数名");
        std::string fname = std::string(prev_().lexeme);
        auto func = std::make_shared<Function>();
        Value::Coord fc = env_->def(fname);
        chunk_ = &(func->chunk);
        env_->enter();
        consume_(TK_LPAREN, "函数声明期望有'('");
        while (match_({TK_IDENT}))
        {
            std::string upname = std::string(prev_().lexeme);
            if (env_->overlap(upname))
            {
                reporterError_(std::format("函数参数名重复 {}", upname));
                return;
            }
            env_->def(upname);
            func->argc++;
            if (func->argc > REI_FUNC_UPVALUE_COUNT_MAX)
            {
                reporterError_(std::format("函数可传参数量超过最大值 {}", REI_FUNC_UPVALUE_COUNT_MAX));
                return;
            }
            if (match_({TK_COMMA}))
                continue;
            break;
        }
        consume_(TK_RPAREN, "函数声明期望有')'");
        {
        auto& func_ctx = func_ctxs_.emplace_back();
        func_ctx.depth = env_->currLocalDepth();
        statement_();
        emitB_(OP_NIL);
        emitB_(OP_RETURN);
        }
        func_ctxs_.pop_back();
        env_->exit();
        chunk_ = savedChunk;
        emitB_(OP_CONSTANT);
        emitC_(func);
        emitB_(OP_DEF_VAR);
        emitB_(fc.lifecycle);
        emitB_(fc.uplevel);
        emitB_(fc.slot);
        emitB_(OP_POP);
    }
    void Parser::structDecl_()
    {
    }
    void Parser::classDecl_()
    {
    }
#pragma endregion
#pragma region Kan/Move
    bool Parser::isAtEnd_() const
    {
        return _cursor.curr == nullptr || _cursor.curr->type == TK_EOF;
    }
    Token::Unit& Parser::advance_()
    {
        if (_cursor.curr == nullptr)
        {
            _cursor.prev = nullptr;
            _cursor.curr = &tokens_.front();
        }
        else if (_cursor.curr->type != TK_EOF)
        {
            _cursor.prev = _cursor.curr;
            _cursor.curr += 1;
        }
        return *_cursor.prev;
    }
    void Parser::pass_()
    {
        if (!isAtEnd_())
            advance_();
    }
    Token::Unit& Parser::prev_()
    {
        return *_cursor.prev;
    }
    Token::Unit& Parser::peek_()
    {
        return *_cursor.curr;
    }
    bool Parser::check_(Token::Type type)
    {
        if (isAtEnd_()) return false;
        return _cursor.curr->type == type;
    }
    bool Parser::check_(std::initializer_list<Token::Type> types)
    {
        if (isAtEnd_()) return false;
        for (auto type : types)
            if (_cursor.curr->type == type)
                return true;
        return false;
    }
    bool Parser::match_(std::initializer_list<Token::Type> types)
    {
        if (!check_(types)) return false;
        advance_();
        return true;
    }
    void Parser::consume_(Token::Type type, const std::string& message)
    {
        if (check_(type))
        {
            pass_();
            return;
        }
    #ifdef REI_OMIT_SEMICOLON_ENABLE
        if (type == TK_SEMICOLON)
            return;
    #endif
        reporterError_(message);
    }
#pragma endregion
#pragma region Emit
    void Parser::emitB_(Bytecode op)
    {
        chunk_->codes.push_back(op);
    }
    void Parser::emitC_(Value::Data value)
    {
        if (chunk_->constants.size() >= REI_BYTECODE_MAX)
        {
            reporterError_("常数块溢出");
            chunk_->codes.push_back(0);
            return;
        }
        chunk_->constants.push_back(value);
        chunk_->codes.push_back(static_cast<Bytecode>(chunk_->constants.size() - 1));
    }
    void Parser::patchB_(Bytecode pos, Bytecode op)
    {
        if (pos >= chunk_->codes.size())
        {
            reporterError_("修补位置超出字节码块范围");
            return;
        }
        chunk_->codes[pos] = static_cast<Bytecode>(op);
    }
#pragma endregion
    void Parser::reporterError_(const std::string& msg)
    {
        _error_reporter->report(std::format("{}: {}", Token::toString(prev_().type), msg), prev_().pos);
    }
}
