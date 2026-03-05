#include "visitor.hpp"
#include <iostream>

namespace luna::ast
{
    Value::Data Printer::operator()(const Node& node)
    {
        std::visit(overloaded
        {
            [&](const Program& p)
            {
                std::cout << "Program:" << std::endl;
                std::cout << "BEG" << std::endl;
                for (size_t i = 0; i < p.nodes.size(); i++)
                    this->operator()(*p.nodes[i]);
                std::cout << "END" << std::endl;
            },
            [&](const Expr::Literal& e)
            {
                _printIndent();
                std::cout << "Literal: " << Value::getDebugString(e.value) << std::endl;
            },
            [&](const Expr::Unary& e)
            {
                _printIndent();
                std::cout << "Unary: " << Token::toSymbol(e.op) << std::endl;
                _indent_level++;
                this->operator()(*e.right);
                _indent_level--;
            },
            [&](const Expr::Binary& e)
            {
                _printIndent();
                std::cout << "Binary: " << Token::toSymbol(e.op) << std::endl;
                _indent_level++;
                this->operator()(*e.left);
                this->operator()(*e.right);
                _indent_level--;
            },
            [&](const Expr::Grouping& e)
            {
                _printIndent();
                std::cout << "Grouping:" << std::endl;
                _indent_level++;
                this->operator()(*e.expression);
                _indent_level--;
            },
            [&](const Stmt::Print& s)
            {
                if (s.kw == Token::TK_PRINT)
                    std::cout << "Print:";
                else
                    std::cout << "Println";
                _indent_level++;
                this->operator()(*s.value);
                _indent_level--;
            }
        }, node.data);
        return Nil{};
    }
    void Printer::_printIndent() const
    {
        std::string result;
        for (int i = 0; i < _indent_level; ++i)
            result += _indent_char;
        std::cout << result;
    }
}
