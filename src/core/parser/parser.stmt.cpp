#include "parser.hpp"

namespace rei
{
    using namespace Token;
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
            level = prev_().literal.toInteger();
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
            level = prev_().literal.toInteger();
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
        if (call_ctxs_.empty())
        {
            reporterError_("返回语句不在函数中");
            return;
        }
        auto& func_ctx = call_ctxs_.back();
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
}
