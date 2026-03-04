#pragma once
#include "core/lexer/token.hpp"
#include "core/base/common.hpp"

namespace luna
{
    namespace ast
    {
        struct Node;
        using NRef = std::unique_ptr<Node>;
    }
    namespace Expr
    {
        struct Literal final
        {
            Value::Data value;
        };
        struct Unary final
        {
            Token::Type op;
            ast::NRef right;
        };
        struct Binary final
        {
            ast::NRef left;
            Token::Type op;
            ast::NRef right;
        };
        struct Grouping final
        {
            ast::NRef expression;
        };
    }
    namespace Stmt
    {
        
    }
    namespace ast
    {
        struct Node final
        {
        public:
            using Data = std::variant<
                Expr::Literal, Expr::Unary, Expr::Binary, Expr::Grouping
                // Stmt::
            >;
        public:
            Data data;
        public:
            Node(Data d) : data(std::move(d)) {}
            explicit Node() = default;
            ~Node() = default;
            Node(const Node&) = delete;
            auto operator=(const Node&) -> Node& = delete;
            Node(Node&&) = delete;
            auto operator=(Node &&) -> Node& = delete;
        public:
            Node& tempRef() { return *this; };
        };
        inline NRef make_ref(Node::Data d)
        {
            return std::make_unique<Node>(std::move(d));
        }
    }
}
