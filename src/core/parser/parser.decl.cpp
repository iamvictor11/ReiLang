#include "parser.hpp"
#include <format>

namespace rei
{
    using namespace Token;
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
        switch (varc.lifetime)
        {
        case Value::VLT_GLOBAL:
            emitB_(OP_DEF_GLOBAL);
            break;
        case Value::VLT_LOCAL:
            emitB_(OP_DEF_LOCAL);
            break;
        }
        emitB_(OP_POP);
        consume_(TK_SEMICOLON, "变量声明语句期望以';'结束");
    }
    void Parser::funcDecl_()
    {
        if (!call_ctxs_.empty())
        {
            closDecl_();
            return;
        }
        Chunk* savedChunk = chunk_;
        consume_(TK_IDENT, "函数声明期望函数名");
        std::string fname = std::string(prev_().lexeme);
        auto func_ref = std::make_shared<Function>();
        Value::Coord fc = env_->def(fname);
        if (fc.slot == REI_BYTECODE_NULL)
        {
            reporterError_(std::format("变量 {} 重定义", fname));
            return;
        }
        chunk_ = &(func_ref->chunk);
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
            func_ref->argc++;
            if (func_ref->argc > REI_FUNC_UPVALUE_COUNT_MAX)
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
        auto& func_ctx = call_ctxs_.emplace_back(env_->currLocalDepth(), Callee{func_ref});
        statement_();
        emitB_(OP_NIL);
        emitB_(OP_RETURN);
        }
        call_ctxs_.pop_back();
        env_->exit();
        chunk_ = savedChunk;
        emitB_(OP_CONSTANT);
        emitC_(func_ref);
        switch (fc.lifetime)
        {
        case Value::VLT_GLOBAL:
            emitB_(OP_DEF_GLOBAL);
            break;
        case Value::VLT_LOCAL:
            emitB_(OP_DEF_LOCAL);
            break;
        }
        emitB_(OP_POP);
    }
    void Parser::closDecl_()
    {
        Chunk* savedChunk = chunk_;
        consume_(TK_IDENT, "闭包声明期望闭包名");
        std::string cname = std::string(prev_().lexeme);
        auto clos_ref = std::make_shared<Closure>();
        Value::Coord cc = env_->def(cname);
        if (cc.slot == REI_BYTECODE_NULL)
        {
            reporterError_(std::format("变量 {} 重定义", cname));
            return;
        }
        chunk_ = &(clos_ref->func.chunk);
        env_->enter();
        consume_(TK_LPAREN, "闭包声明期望有'('");
        while (match_({TK_IDENT}))
        {
            std::string upname = std::string(prev_().lexeme);
            if (env_->overlap(upname))
            {
                reporterError_(std::format("闭包参数名重复 {}", upname));
                return;
            }
            env_->def(upname);
            clos_ref->func.argc++;
            if (clos_ref->func.argc > REI_FUNC_UPVALUE_COUNT_MAX)
            {
                reporterError_(std::format("闭包可传参数量超过最大值 {}", REI_FUNC_UPVALUE_COUNT_MAX));
                return;
            }
            if (match_({TK_COMMA}))
                continue;
            break;
        }
        consume_(TK_RPAREN, "闭包声明期望有')'");
        {
        auto& clos_ctx = call_ctxs_.emplace_back(env_->currLocalDepth(), Callee{clos_ref});
        statement_();
        emitB_(OP_NIL);
        emitB_(OP_RETURN);
        }
        call_ctxs_.pop_back();
        env_->exit();
        chunk_ = savedChunk;
        emitB_(OP_CONSTANT);
        emitC_(clos_ref);
        switch (cc.lifetime)
        {
        case Value::VLT_GLOBAL:
            emitB_(OP_DEF_GLOBAL);
            break;
        case Value::VLT_LOCAL:
            emitB_(OP_DEF_LOCAL);
            break;
        }
        emitB_(OP_POP);
    }
    void Parser::structDecl_()
    {
    }
    void Parser::classDecl_()
    {
    }
}
