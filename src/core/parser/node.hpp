#pragma once
#include "core/lexer/token.hpp"
#include "core/base/common.hpp"

namespace luna
{
    struct Node;
    namespace Expr
    {
        struct Literal final
        {
            Value::Data value;
        };
        struct Unary final
        {
            Token::Type op;
            std::unique_ptr<Node> right;
        };
        struct Binary final
        {
            std::unique_ptr<Node> left;
            Token::Type op;
            std::unique_ptr<Node> right;
        };
        struct Grouping final
        {
            std::unique_ptr<Node> expression;
        };
    }
    struct Node final
    {
        using Data = std::variant<
            Expr::Literal, Expr::Unary, Expr::Binary, Expr::Grouping
        >;
        Data data;
        Node(Data d) : data(std::move(d)) {}
    };
}
