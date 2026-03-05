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
                for (size_t i = 0; i < p.nodes.size(); i++)
                    this->operator()(*p.nodes[i]);
            },
            [&](const Expr::Literal& e)
            {
                _printIndent();
                std::cout << "Literal: " <<
                    "\033[1m\033[36m" << Value::getDebugString(e.value) <<
                    "\033[0m" << std::endl;
            },
            [&](const Expr::Unary& e)
            {
                _printIndent();
                std::cout << "Unary: " <<
                    "\033[1m\033[36m" << Token::toSymbol(e.op) <<
                    "\033[0m" << std::endl;
                _indent_level++;
                this->operator()(*e.right);
                _indent_level--;
            },
            [&](const Expr::Binary& e)
            {
                _printIndent();
                std::cout << "Binary: " <<
                    "\033[1m\033[36m" << Token::toSymbol(e.op) <<
                    "\033[0m" << std::endl;
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
                _printIndent();
                if (s.kw == Token::TK_PRINT)
                    std::cout << "Print: " << std::endl;
                else
                    std::cout << "Println: " << std::endl;
                this->operator()(*s.value);
            }
        }, node.data);
        return Nil{};
    }
    void Printer::_printIndent() const
    {
        std::string result;
        for (int i = 0; i < _indent_level; ++i)
            result += _indent_char_s;
        std::cout << result << "\033[1m\033[92m" << _indent_level << "\033[0m";
    }
}
