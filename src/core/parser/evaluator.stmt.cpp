#include "visitor.hpp"
#include <iostream>

namespace luna::ast
{
    using namespace Token;
#pragma region Impl
#pragma endregion
#pragma region Visit
    Evaluator::ResType Evaluator::_execute(const Stmt::Expression& n)
    {
        return operator()(*n.expression);
    }
    Evaluator::ResType Evaluator::_execute(const Stmt::Print& n)
    {
        Value::Data vd = operator()(*n.value);
        if (n.kw == TK_PRINT)
            std::cout << Value::toString(vd);
        else
            std::cout << Value::toString(vd) << std::endl;
        return vd;
    }
    Evaluator::ResType Evaluator::_execute(const Stmt::VarDecl& n)
    {
        if (n.initializer)
            _env->def(n.name, {operator()(*n.initializer), n.is_const});
        else
            _env->decl(n.name, n.is_const);
        return Nil{};
    }
    Evaluator::ResType Evaluator::_execute(const Stmt::Block& n)
    {
        _env->nest();
        for (size_t i = 0; i < n.statements.size(); i++)
            this->operator()(*n.statements[i]);
        _env->uest();
        return Nil{};
    }
    Evaluator::ResType Evaluator::_execute(const Stmt::Ifelse& n)
    {
        if (Value::toBoolean(this->operator()(*n.condition)))
            return this->operator()(*n.thenBlock);
        if (n.elseBlock)
            return this->operator()(*n.elseBlock);
        return Nil{};
    }
    Evaluator::ResType Evaluator::_execute(const Stmt::Loop& n)
    {
        if (n.doBlock)
            while (Value::toBoolean(this->operator()(*n.condition)))
                this->operator()(*n.doBlock);
        else
            while (Value::toBoolean(this->operator()(*n.condition)));
        return Nil{};
    }
#pragma endregion
}
