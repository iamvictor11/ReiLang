#pragma once
#include "node.hpp"
#include "core/base/environment.hpp"

namespace luna::ast
{
    template <class... Ts>
    struct LambdaOverloaded : Ts...
    {
        using Ts::operator()...;
    };
    template <class... Ts>
    LambdaOverloaded(Ts...) -> LambdaOverloaded<Ts...>;

    struct Evaluator final
    {
    private:
        Environment* _env;
    public:
        Evaluator(Environment* e) : _env(e) {}
    public:
        using ResType = Value::Data;
    public:
        ResType operator()(const Node& node);
    private:
        ResType _execute(const Program& n);
        ResType _execute(const Expr::VarName& n);
        ResType _execute(const Expr::Assign& n);
        ResType _execute(const Expr::Unary& n);
        ResType _execute(const Expr::Binary& n);
        ResType _execute(const Expr::Grouping& n);
        ResType _execute(const Stmt::Expression& n);
        ResType _execute(const Stmt::Print& n);
        ResType _execute(const Stmt::VarDecl& n);
        ResType _execute(const Stmt::Block& n);
        ResType _execute(const Stmt::Ifelse& n);
        ResType _execute(const Stmt::Loop& n);
    };
    struct Printer final
    {
    private:
        static constexpr char _indent_char_s = '\t';
    private:
        int _indent_level = 0;
    public:
        Value::Data operator()(const Node& node);
    private:
        void _printIndent() const;
    };
}