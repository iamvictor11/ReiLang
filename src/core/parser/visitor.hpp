#pragma once
#include "node.hpp"

namespace luna::ast
{
    template <class... Ts>
    struct overloaded : Ts...
    {
        using Ts::operator()...;
    };
    template <class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    struct Evaluator final
    {
    public:
        Value::Data operator()(const ast::Node& node);
    };
    struct Printer final
    {
    private:
        int _indent_level = 0;
        char _indent_char = '\t';
    public:
        Value::Data operator()(const ast::Node& node);
    private:
        void _printIndent() const;
    };
}