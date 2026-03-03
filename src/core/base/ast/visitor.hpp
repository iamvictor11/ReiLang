#pragma once
#include "node.hpp"

namespace luna
{
    struct Visitor
    {
        virtual void visit(const Expr::Literal& expr) = 0;
        virtual void visit(const Expr::Unary& expr) = 0;
        virtual void visit(const Expr::Binary& expr) = 0;
        virtual void visit(const Expr::Grouping& expr) = 0;
    };
}