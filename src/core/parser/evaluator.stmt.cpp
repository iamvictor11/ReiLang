#include "visitor.hpp"
#include <iostream>

namespace luna::ast
{
    using namespace Token;
#pragma region Impl
#pragma endregion
#pragma region Visit
    Value::Data Evaluator::_execute(const Stmt::Expression& n)
    {
        return operator()(*n.expression);
    }
    Value::Data Evaluator::_execute(const Stmt::Print& n)
    {
        Value::Data vd = operator()(*n.value);
        if (n.kw == TK_PRINT)
            std::cout << Value::toString(vd);
        else
            std::cout << Value::toString(vd) << std::endl;
        return vd;
    }
    Value::Data Evaluator::_execute(const Stmt::VarDecl& n)
    {
        return Nil{};
    }
    Value::Data Evaluator::_execute(const Stmt::Block& n)
    {
        return Nil{};
    }
#pragma endregion
}
