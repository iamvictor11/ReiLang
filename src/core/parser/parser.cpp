#include "parser.hpp"
#include <format>

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
        [TK_CLONE]     = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
        [TK_WCLONE]    = {nullptr, &Parser::assignExpr_, nullptr, PREC_ASSIGN, 0},
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
        [TK_ONCE]      = {nullptr, nullptr, nullptr, PREC_NONE, 0},
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
        [TK_THEN]      = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_DO]        = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_BEG]       = {nullptr, nullptr, nullptr, PREC_NONE, 0},
        [TK_END]       = {nullptr, nullptr, nullptr, PREC_NONE, 0},
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
        while (!isAtEnd_() && error_reporter_->empty())
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
#pragma region Kan/Move
    bool Parser::isAtEnd_() const
    {
        return cursor_.curr == nullptr || cursor_.curr->type == TK_EOF;
    }
    Token::Unit& Parser::advance_()
    {
        if (cursor_.curr == nullptr)
        {
            cursor_.prev = nullptr;
            cursor_.curr = &tokens_.front();
        }
        else if (cursor_.curr->type != TK_EOF)
        {
            cursor_.prev = cursor_.curr;
            cursor_.curr += 1;
        }
        return *cursor_.prev;
    }
    void Parser::pass_()
    {
        if (!isAtEnd_())
            advance_();
    }
    Token::Unit& Parser::prev_()
    {
        return *cursor_.prev;
    }
    Token::Unit& Parser::peek_()
    {
        return *cursor_.curr;
    }
    bool Parser::check_(Token::Type type)
    {
        if (isAtEnd_()) return false;
        return cursor_.curr->type == type;
    }
    bool Parser::check_(std::initializer_list<Token::Type> types)
    {
        if (isAtEnd_()) return false;
        for (auto type : types)
            if (cursor_.curr->type == type)
                return true;
        return false;
    }
    bool Parser::match_(Token::Type type)
    {
        if (!check_(type)) return false;
        pass_();
        return true;
    }
    bool Parser::match_(std::initializer_list<Token::Type> types)
    {
        if (!check_(types)) return false;
        pass_();
        return true;
    }
    void Parser::consume_(Token::Type type, const std::string& message)
    {
        if (match_(type))
            return;
    #if REI_OMIT_SEMICOLON_ENABLE == 1
        if (type == TK_SEMICOLON)
            return;
    #endif
        reporterError_(message);
    }
#pragma endregion
#pragma region Emit
    void Parser::emitB_(Bytecode op)
    {
        if (chunk_->codes.size() >= REI_BYTECODE_MAX - 1)
        {
            reporterError_("字节码块溢出");
            chunk_->codes.push_back(OP_HALT);
            return;
        }
        chunk_->codes.push_back(op);
    }
    void Parser::emitC_(Value::Data value)
    {
        if (chunk_->constants.size() >= REI_BYTECODE_MAX - 1)
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
        error_reporter_->report(std::format("{}: {}", Token::toString(prev_().type), msg), prev_().pos);
    }
}
