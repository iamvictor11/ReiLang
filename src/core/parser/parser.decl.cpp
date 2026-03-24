#include "parser.hpp"
#include <format>

namespace rei
{
    using namespace Token;
    void Parser::declaration_()
    {
        if (match_({TK_DEF, TK_VAR, TK_LET}))
            varDecl_();
        else if (match_(TK_FUNC))
            funcDecl_();
        else if (match_(TK_STRUCT))
            structDecl_();
        else if (match_(TK_CLASS))
            classDecl_();
        else
            statement_();
    }
    void Parser::varDecl_()
    {
        consume_(TK_IDENT, "变量期望用标识符标记");
        std::string vname = std::string(prev_().lexeme);
        Value::Coord varc = env_->def(vname);
        if (match_(TK_ASSIGN))
            expression_();
        else
            emitB_(OP_NIL);
        switch (varc.lifetime)
        {
        case Value::VLT_GLOBAL:
            emitB_(OP_GLOBAL_DEF);
            break;
        case Value::VLT_LOCAL:
            emitB_(OP_LOCAL_DEF);
            break;
        }
        emitB_(OP_POP);
        consume_(TK_SEMICOLON, "变量声明语句期望以';'结束");
    }
    void Parser::funcDecl_()
    {
        Chunk* savedChunk = chunk_;
        consume_(TK_IDENT, "函数声明期望函数名");
        std::string fname = std::string(prev_().lexeme);
        if (env_->currLocalDepth() != 0)
        {
            reporterError_(std::format("函数 {} 必须在全局作用域定义", fname));
            return;
        }
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
        while (match_(TK_IDENT))
        {
            std::string arg_name = std::string(prev_().lexeme);
            if (env_->overlap(arg_name))
            {
                reporterError_(std::format("函数参数名重复 {}", arg_name));
                return;
            }
            env_->def(arg_name);
            func_ref->argc++;
            if (func_ref->argc > REI_FUNC_ARG_COUNT_MAX)
            {
                reporterError_(std::format("函数可传参数量超过最大值 {}", REI_FUNC_ARG_COUNT_MAX));
                return;
            }
            if (match_(TK_COMMA))
                continue;
            break;
        }
        auto& func_ctx = func_ctxs_.emplace_back(env_->currLocalDepth(), func_ref);
        consume_(TK_RPAREN, "函数声明期望有')'");
        if (match_(TK_ONCE))
        {
            do
            {
                consume_(TK_IDENT, "函数保留数期望有标识符声明");
                std::string once_name = std::string(prev_().lexeme);
                if (func_ctx.onces.contains(once_name))
                {
                    reporterError_(std::format("函数保留数名重复 {}", once_name));
                    return;
                }
                func_ctx.onces[once_name] = func_ref->onces.size();
                if (match_(TK_ASSIGN))
                {
                    if (match_({TK_LIT_INT, TK_LIT_FLOAT, TK_LIT_STRING}))
                        func_ref->onces.emplace_back(prev_().literal);
                    else if (match_(TK_NIL))
                        func_ref->onces.emplace_back(Nil{});
                    else if (match_(TK_TRUE))
                        func_ref->onces.emplace_back(true);
                    else if (match_(TK_FALSE))
                        func_ref->onces.emplace_back(false);
                    else
                    {
                        reporterError_(std::format("函数保留数 {} 必须使用字面量赋值", once_name));
                        return;
                    }
                }
                else
                {
                    func_ref->onces.emplace_back(Nil{});
                }
                if (func_ref->onces.size() > REI_FUNC_ONCE_COUNT_MAX)
                {
                    reporterError_(std::format("函数保留数数量超过最大值 {}", REI_FUNC_ONCE_COUNT_MAX));
                    return;
                }
            } while (match_(TK_COMMA));
        }
        {
        bodyStmt_(TK_BEG, TK_END, "函数体需要 end 封闭");
        emitB_(OP_NIL);
        emitB_(OP_RETURN);
        }
        func_ctxs_.pop_back();
        env_->exit();
        chunk_ = savedChunk;
        emitB_(OP_CONSTANT);
        emitC_(func_ref);
        switch (fc.lifetime)
        {
        case Value::VLT_GLOBAL:
            emitB_(OP_GLOBAL_DEF);
            break;
        case Value::VLT_LOCAL:
            emitB_(OP_LOCAL_DEF);
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
