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
                reporterError_("你怎么做到的，'='的解析不会走这里");
                break;
            }
            case TK_WALRUS:
            {
                reporterError_("你怎么做到的，':='的解析不会走这里");
                break;
            }
            case TK_CLONE:
            {
                reporterError_("你怎么做到的，'$='的解析不会走这里");
                break;
            }
            case TK_WCLONE:
            {
                reporterError_("你怎么做到的，'$:='的解析不会走这里");
                break;
            }
            case TK_SELF_ADD:   emitB_(OP_KEEP); emitB_(OP_ADD); break;
            case TK_SELF_SUB:   emitB_(OP_KEEP); emitB_(OP_SUB); break;
            case TK_SELF_MUL:   emitB_(OP_KEEP); emitB_(OP_MUL); break;
            case TK_SELF_DIV:   emitB_(OP_KEEP); emitB_(OP_DIV); break;
            case TK_SELF_MOD:   emitB_(OP_KEEP); emitB_(OP_MOD); break;
            case TK_SELF_POW:   emitB_(OP_KEEP); emitB_(OP_POW); break;
            case TK_SELF_BIT_AND:   emitB_(OP_KEEP); emitB_(OP_BIT_AND); break;
            case TK_SELF_BIT_OR:    emitB_(OP_KEEP); emitB_(OP_BIT_OR); break;
            case TK_SELF_BIT_XOR:   emitB_(OP_KEEP); emitB_(OP_BIT_XOR); break;
            case TK_SELF_BIT_XNOR:  emitB_(OP_KEEP); emitB_(OP_BIT_XNOR); break;
            case TK_SELF_BIT_NOT:   emitB_(OP_KEEP); emitB_(OP_BIT_NOT); break;
            case TK_SELF_BIT_SHL:   emitB_(OP_KEEP); emitB_(OP_BIT_SHL); break;
            case TK_SELF_BIT_SHR:   emitB_(OP_KEEP); emitB_(OP_BIT_SHR); break;
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
            case TK_SUB:        emitB_(OP_NEG); break;
            case TK_BIT_NOT:    emitB_(OP_BIT_NOT); break;
            case TK_NOT:        emitB_(OP_NOT); break;
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
            case TK_ADD:    emitB_(OP_ADD); break;
            case TK_SUB:    emitB_(OP_SUB); break;
            case TK_MUL:    emitB_(OP_MUL); break;
            case TK_DIV:    emitB_(OP_DIV); break;
            case TK_MOD:    emitB_(OP_MOD); break;
            case TK_POW:    emitB_(OP_POW); break;
            case TK_BIT_AND:    emitB_(OP_BIT_AND); break;
            case TK_BIT_OR:     emitB_(OP_BIT_OR); break;
            case TK_BIT_XOR:    emitB_(OP_BIT_XOR); break;
            case TK_BIT_XNOR:   emitB_(OP_BIT_XNOR); break;
            case TK_BIT_SHL:    emitB_(OP_BIT_SHL); break;
            case TK_BIT_SHR:    emitB_(OP_BIT_SHR); break;
            case TK_EQ:     emitB_(OP_EQ); break;
            case TK_NE:     emitB_(OP_NE); break;
            case TK_LT:     emitB_(OP_LT); break;
            case TK_LE:     emitB_(OP_LE); break;
            case TK_GT:     emitB_(OP_GT); break;
            case TK_GE:     emitB_(OP_GE); break;
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
            if (argc > REI_FUNC_ARG_COUNT_MAX)
                reporterError_(std::format("调用表达式传参数量超过最大值 {}", REI_FUNC_ARG_COUNT_MAX));
            if (match_(TK_COMMA))
                continue;
            break;
        }
        consume_(TK_RPAREN, "调用表达式期望以')'结束");
        emitB_(OP_CALL);
        emitB_(argc);
    }
    void Parser::indexExpr_()
    {
        if (check_(TK_RBRACKET))
        {
            reporterError_("索引表达式期望表达式");
            return;
        }
        expression_();
        consume_(TK_RBRACKET, "调用表达式期望以']'结束");
        if (match_(TK_ASSIGN))
        {
            expression_();
            emitB_(OP_INDEX_SET);
        }
        else if (match_(TK_CLONE))
        {
            expression_();
            emitB_(OP_CLONE);
            emitB_(OP_INDEX_SET);
        }
        else if (match_({
            TK_SELF_ADD, TK_SELF_SUB, TK_SELF_MUL, TK_SELF_DIV, TK_SELF_MOD, TK_SELF_POW,
            TK_SELF_BIT_AND, TK_SELF_BIT_OR, TK_SELF_BIT_XOR, TK_SELF_BIT_XNOR, TK_SELF_BIT_NOT, TK_SELF_BIT_SHL, TK_SELF_BIT_SHR
        }))
        {
            assignExpr_();
            emitB_(OP_INDEX_SSET);
        }
        else
            emitB_(OP_INDEX_GET);
    }
    void Parser::primaryExpr_()
    {
        switch (prev_().type)
        {
            case TK_LIT_INT:
            case TK_LIT_FLOAT:
            case TK_LIT_STRING:
                emitB_(OP_CONSTANT);
                emitC_(prev_().literal);
                break;
            case TK_NIL:    emitB_(OP_NIL); break;
            case TK_TRUE:   emitB_(OP_TRUE); break;
            case TK_FALSE:  emitB_(OP_FALSE); break;
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
            varSet_(std::string{vu.lexeme});
        }
        else if (match_(TK_WALRUS))
        {
            expression_();
            varDef_(std::string{vu.lexeme});
        }
        else if (match_(TK_CLONE))
        {
            expression_();
            emitB_(OP_CLONE);
            varSet_(std::string{vu.lexeme});
        }
        else if (match_(TK_WCLONE))
        {
            expression_();
            emitB_(OP_CLONE);
            varDef_(std::string{vu.lexeme});
        }
        else if (match_({
            TK_SELF_ADD, TK_SELF_SUB, TK_SELF_MUL, TK_SELF_DIV, TK_SELF_MOD, TK_SELF_POW,
            TK_SELF_BIT_AND, TK_SELF_BIT_OR, TK_SELF_BIT_XOR, TK_SELF_BIT_XNOR, TK_SELF_BIT_NOT, TK_SELF_BIT_SHL, TK_SELF_BIT_SHR
        }))
        {
            auto vname = std::string{vu.lexeme};
            assignExpr_();
            varSSet_(vname);
        }
        else
        {
            varGet_(std::string{vu.lexeme});
        }
    }
    void Parser::arrayInitExpr_()
    {
        Bytecode array_size = 0;
        if (!match_(TK_RBRACKET))
        {
            do
            {
                expression_();
                array_size++;
            }
            while (match_(TK_COMMA));
            consume_(TK_RBRACKET, "数组初始化需要']'封闭");
        }
        emitB_(OP_INIT_ARRAY);
        emitB_(array_size);
    }
#pragma region Helper
    void Parser::varDef_(const std::string& name)
    {
        Value::Coord varc = env_->def(name);
        if (varc.slot == REI_BYTECODE_NULL)
        {
            reporterError_(std::format("变量 {} 重定义", name));
            return;
        }
        switch (varc.lifetime)
        {
        case Value::VLT_GLOBAL:
            emitB_(OP_GLOBAL_DEF);
            break;
        case Value::VLT_LOCAL:
            emitB_(OP_LOCAL_DEF);
            break;
        }
    }
    void Parser::varGet_(const std::string& name)
    {
        if (!func_ctxs_.empty())
        {
            auto& func_ctx = func_ctxs_.back();
            if (auto it = func_ctx.onces.find(name); it != func_ctx.onces.end())
            {
                emitB_(OP_ONCE_GET);
                emitB_(it->second);
                return;
            }
        }
        Value::Coord varc = env_->toCoord(name);
        if (!varc.isValid())
        {
            reporterError_(std::format("变量 {} 未定义", name));
            return;
        }
        switch (varc.lifetime)
        {
        case Value::VLT_HOST:
            emitB_(OP_HOST_GET);
            emitB_(varc.slot);
            break;
        case Value::VLT_GLOBAL:
            emitB_(OP_GLOBAL_GET);
            emitB_(varc.slot);
            break;
        case Value::VLT_LOCAL:
            emitB_(OP_LOCAL_GET);
            emitB_(varc.uplevel);
            emitB_(varc.slot);
            break;
        }
    }
    void Parser::varSet_(const std::string& name)
    {
        if (!func_ctxs_.empty())
        {
            auto& func_ctx = func_ctxs_.back();
            if (auto it = func_ctx.onces.find(name); it != func_ctx.onces.end())
            {
                emitB_(OP_ONCE_SET);
                emitB_(it->second);
                return;
            }
        }
        Value::Coord varc = env_->toCoord(name);
        if (!varc.isValid())
        {
            reporterError_(std::format("变量 {} 未定义", name));
            return;
        }
        switch (varc.lifetime)
        {
        case Value::VLT_HOST:
            emitB_(OP_HOST_SET);
            emitB_(varc.slot);
            break;
        case Value::VLT_GLOBAL:
            emitB_(OP_GLOBAL_SET);
            emitB_(varc.slot);
            break;
        case Value::VLT_LOCAL:
            emitB_(OP_LOCAL_SET);
            emitB_(varc.uplevel);
            emitB_(varc.slot);
            break;
        }
    }
    void Parser::varSSet_(const std::string& name)
    {
        if (!func_ctxs_.empty())
        {
            auto& func_ctx = func_ctxs_.back();
            if (auto it = func_ctx.onces.find(name); it != func_ctx.onces.end())
            {
                emitB_(OP_ONCE_SSET);
                emitB_(it->second);
                return;
            }
        }
        Value::Coord varc = env_->toCoord(name);
        if (!varc.isValid())
        {
            reporterError_(std::format("变量 {} 未定义", name));
            return;
        }
        switch (varc.lifetime)
        {
        case Value::VLT_HOST:
            emitB_(OP_HOST_SSET);
            emitB_(varc.slot);
            break;
        case Value::VLT_GLOBAL:
            emitB_(OP_GLOBAL_SSET);
            emitB_(varc.slot);
            break;
        case Value::VLT_LOCAL:
            emitB_(OP_LOCAL_SSET);
            emitB_(varc.uplevel);
            emitB_(varc.slot);
            break;
        }
    }
#pragma endregion
}
