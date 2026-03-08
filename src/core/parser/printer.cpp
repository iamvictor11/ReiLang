#include "visitor.hpp"
#include <iostream>

namespace luna::ast
{
    Value::Data Printer::operator()(const Node& node)
    {
        std::visit(LambdaOverloaded
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
            [&](const Expr::VarName& e)
            {
                _printIndent();
                std::cout << "VarName: " <<
                    "\033[1m\033[36m" << Value::getDebugString(e.name) <<
                    "\033[0m" << std::endl;
            },
            [&](const Expr::Assign& e)
            {
                _printIndent();
                std::cout << "Assign: " <<
                    "\033[1m\033[36m" << Token::toSymbol(e.op) <<
                    "\033[0m" << std::endl;
                _indent_level++;
                this->operator()(*e.left);
                this->operator()(*e.right);
                _indent_level--;
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
                std::cout << "Grouping: " <<
                    "\033[1m\033[36m" << "(" <<
                    "\033[0m" << std::endl;
                _indent_level++;
                this->operator()(*e.expression);
                _printIndent();
                std::cout <<
                    "\033[1m\033[36m" << ")" <<
                    "\033[0m" << std::endl;
                _indent_level--;
            },
            [&](const Stmt::Expression& s)
            {
                _printIndent();
                std::cout << "Expression:" << std::endl;
                this->operator()(*s.expression);
            },
            [&](const Stmt::Print& s)
            {
                _printIndent();
                if (s.kw == Token::TK_PRINT)
                    std::cout << "Print:" << std::endl;
                else
                    std::cout << "Println:" << std::endl;
                this->operator()(*s.value);
            },
            [&](const Stmt::VarDecl& s)
            {
                _printIndent();
                std::cout << "VarDecl: " <<
                    "\033[1m\033[36m" << Value::getDebugString(s.name) <<
                    "\033[0m" << std::endl;
                _indent_level++;
                this->operator()(*s.initializer);
                _indent_level--;
            },
            [&](const Stmt::Block& s)
            {
                _printIndent();
                std::cout << "Block: " <<
                    "\033[1m\033[36m" << "{" <<
                    "\033[0m" << std::endl;
                _indent_level++;
                for (size_t i = 0; i < s.statements.size(); i++)
                    this->operator()(*s.statements[i]);
                _printIndent();
                std::cout <<
                    "\033[1m\033[36m" << "}" <<
                    "\033[0m" << std::endl;
                _indent_level--;
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
