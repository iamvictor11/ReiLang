#pragma once
#include "node.hpp"

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
    public:
        Value::Data operator()(const Node& node);
    private:
        Value::Data _execute(const Program& n);
        Value::Data _execute(const Expr::VarName& n);
        Value::Data _execute(const Expr::Assign& n);
        Value::Data _execute(const Expr::Unary& n);
        Value::Data _execute(const Expr::Binary& n);
        Value::Data _execute(const Expr::Grouping& n);
        Value::Data _execute(const Stmt::Expression& n);
        Value::Data _execute(const Stmt::Print& n);
        Value::Data _execute(const Stmt::VarDecl& n);
        Value::Data _execute(const Stmt::Block& n);
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