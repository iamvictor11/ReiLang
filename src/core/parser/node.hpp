#pragma once
#include "core/lexer/token.hpp"
#include "core/base/common.hpp"

namespace luna::ast
{
    struct Node;
    using NRef = std::unique_ptr<Node>;
    struct Program final
    {
        std::vector<NRef> nodes;
    };
    namespace Expr
    {
        struct Literal final
        {
            Value::Data value;
        };
        struct VarName final
        {
            std::string name;
            bool is_assigned;
        };
        struct Assign final
        {
            NRef left;
            Token::Type op;
            NRef right;
        };
        struct Unary final
        {
            Token::Type op;
            NRef right;
        };
        struct Binary final
        {
            NRef left;
            Token::Type op;
            NRef right;
        };
        struct Grouping final
        {
            NRef expression;
        };
        struct Call final
        {
            NRef callee;
            std::vector<NRef> arguments;
        };
    }
    namespace Stmt
    {
        struct Expression final
        {
            NRef expression;
        };
        struct Print final
        {
            Token::Type kw;
            NRef value;
        };
        struct VarDecl final
        {
            std::string name;
            bool is_const;
            NRef initializer;
        };
        struct FuncDecl final
        {
            // std::
        };
        struct Block final
        {
            std::vector<NRef> statements;
        };
        struct Ifelse final
        {
            NRef condition;
            NRef thenBlock;
            NRef elseBlock;
        };
        struct Loop final
        {
            NRef condition;
            NRef doBlock;
        };
    }
    struct Node final
    {
    public:
        using Data = std::variant<
            Program,
            Expr::Literal, Expr::VarName, Expr::Assign, Expr::Unary, Expr::Binary, Expr::Grouping, Expr::Call,
            Stmt::Expression, Stmt::Print, Stmt::VarDecl, Stmt::Block, Stmt::Ifelse, Stmt::Loop
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
    public:
        static NRef make_ref(Node::Data d) { return std::make_unique<Node>(std::move(d)); }
    };
}
