#pragma once
#include "core/lexer/token.hpp"
#include "core/base/env.complie.hpp"
#include "util/error.hpp"
#include "precedence.hpp"

namespace rei
{
    class Parser
    {
    private:
        using MyFnuc_ = void(Parser::*)();
        struct Rule_ final
        {
            MyFnuc_ prefix;
            MyFnuc_ infix;
            MyFnuc_ suffix;
            Precedence precedence;
            uint8_t is_left_assoc;
        };
    private:
        struct LoopCtx_ final
        {
            Bytecode start;
            Bytecode depth;
            std::vector<Bytecode> breaks;
        };
        struct FuncCtx_ final
        {
        public:
            Bytecode depth;
            Ref<Function> func_ref;
            std::vector<Bytecode> returns;
            std::unordered_map<std::string, Bytecode> onces;
        public:
            FuncCtx_(Bytecode d, Ref<Function> f) : depth(d), func_ref(f) {}
        };
    private:
        static Rule_ rules_s_[];
    private:
        std::vector<LoopCtx_> loop_ctxs_ {};
        std::vector<FuncCtx_> func_ctxs_ {};
    private:
        Token::List tokens_;
        Chunk* chunk_;
        Env<PS_COMPILE>* env_;
        struct
        {
            Token::Unit* prev = nullptr;
            Token::Unit* curr = nullptr;
        } cursor_;
        Error::Reporter* error_reporter_;
    public:
        Parser(Token::List&& ts, Chunk* ck, Env<PS_COMPILE>* ev, Error::Reporter* er) : tokens_(std::move(ts)), chunk_(ck), env_(ev), error_reporter_(er) {};
        ~Parser() = default;
    public:
        void start();
    private:
        void program_();
        void parsePrecedence_(Precedence precedence);
    private:
        void expression_();
        void assignExpr_();
        void groupingExpr_();
        void unaryExpr_();
        void binaryExpr_();
        void callExpr_();
        void indexExpr_();
        void primaryExpr_();
        void varExpr_();
        void arrayInitExpr_();
    private:
        void varDef_(const std::string& name);
        void varGet_(const std::string& name);
        void varSet_(const std::string& name);
        void varSSet_(const std::string& name);
    private:
        void statement_();
        void exprStmt_();
        void blockStmt_();
        void bodyStmt_(Token::Type beg, Token::Type end, const std::string& message);
        void ifStmt_();
        void loopStmt_();
        void breakStmt_();
        void continueStmt_();
        void returnStmt_();
        void printStmt_();
        void midiStmt_();
    private:
        void declaration_();
        void varDecl_();
        void funcDecl_();
    private:
        bool isAtEnd_() const;
        Token::Unit& advance_();
        void pass_();
        Token::Unit& prev_();
        Token::Unit& peek_();
        bool check_(Token::Type type);
        bool check_(std::initializer_list<Token::Type> types);
        bool match_(Token::Type type);
        bool match_(std::initializer_list<Token::Type> types);
        void consume_(Token::Type type, const std::string& message);
    private:
        void emitB_(Bytecode op);
        void emitC_(Value::Data value);
        void patchB_(Bytecode pos, Bytecode op);
    private:
        void reporterError_(const std::string& msg);
    };
}
