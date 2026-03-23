#include "parser.hpp"
#include <format>

namespace rei
{
    using namespace Token;
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
            if (match_(TK_COMMA))
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
        if (match_(TK_ASSIGN))
        {
            expression_();
            Value::Coord varc = env_->toCoord(std::string{vu.lexeme});
            if (!varc.isValid())
            {
                reporterError_("变量未定义");
                return;
            }
            switch (varc.lifetime)
            {
            case Value::VLT_HOST:
                emitB_(OP_SET_HOST);
                break;
            case Value::VLT_GLOBAL:
                emitB_(OP_SET_GLOBAL);
                break;
            case Value::VLT_LOCAL:
                emitB_(OP_SET_LOCAL);
                emitB_(varc.uplevel);
                break;
            }
            emitB_(varc.slot);
        }
        else if (match_(TK_WALRUS))
        {
            Value::Coord varc = env_->def(std::string{vu.lexeme});
            if (varc.slot == REI_BYTECODE_NULL)
            {
                reporterError_(std::format("变量 {} 重定义", std::string{vu.lexeme}));
                return;
            }
            expression_();
            switch (varc.lifetime)
            {
            case Value::VLT_GLOBAL:
                emitB_(OP_DEF_GLOBAL);
                break;
            case Value::VLT_LOCAL:
                emitB_(OP_DEF_LOCAL);
                break;
            }
        }
        else if (match_({
            TK_SELF_ADD, TK_SELF_SUB, TK_SELF_MUL, TK_SELF_DIV, TK_SELF_MOD, TK_SELF_POW,
            TK_SELF_BIT_AND, TK_SELF_BIT_OR, TK_SELF_BIT_XOR, TK_SELF_BIT_XNOR, TK_SELF_BIT_NOT, TK_SELF_BIT_SHL, TK_SELF_BIT_SHR
        }))
        {
            Value::Coord varc = env_->toCoord(std::string{vu.lexeme});
            if (!varc.isValid())
            {
                reporterError_(std::format("变量 {} 未定义", std::string{vu.lexeme}));
                return;
            }
            switch (varc.lifetime)
            {
            case Value::VLT_HOST:
                emitB_(OP_GET_HOST);
                emitB_(varc.slot);
                break;
            case Value::VLT_GLOBAL:
                emitB_(OP_GET_GLOBAL);
                emitB_(varc.slot);
                break;
            case Value::VLT_LOCAL:
                emitB_(OP_GET_LOCAL);
                emitB_(varc.uplevel);
                emitB_(varc.slot);
                break;
            }
            assignExpr_();
            switch (varc.lifetime)
            {
            case Value::VLT_HOST:
                emitB_(OP_SET_HOST);
                break;
            case Value::VLT_GLOBAL:
                emitB_(OP_SET_GLOBAL);
                break;
            case Value::VLT_LOCAL:
                emitB_(OP_SET_LOCAL);
                emitB_(varc.uplevel);
                break;
            }
            emitB_(varc.slot);
        }
        else
        {
            Value::Coord varc = env_->toCoord(std::string{vu.lexeme});
            if (!varc.isValid())
            {
                reporterError_(std::format("变量 {} 未定义", std::string{vu.lexeme}));
                return;
            }
            switch (varc.lifetime)
            {
            case Value::VLT_HOST:
                emitB_(OP_GET_HOST);
                emitB_(varc.slot);
                break;
            case Value::VLT_GLOBAL:
                emitB_(OP_GET_GLOBAL);
                emitB_(varc.slot);
                break;
            case Value::VLT_LOCAL:
                emitB_(OP_GET_LOCAL);
                emitB_(varc.uplevel);
                emitB_(varc.slot);
                break;
            }
        }
    }
}
