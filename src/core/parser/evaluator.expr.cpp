#include "visitor.hpp"
#include <cmath>
#include <limits>

namespace luna::ast
{
    using namespace Token;
#pragma region Impl
    static Value::Data _dispatchBinary(const Value::Data& left, const Value::Data& right, Token::Type op);
    template<typename L, typename R>
    static Value::Data _numberBinary(L&& left, R&& right, Token::Type op);
    template<typename L, typename R>
    static Value::Data _stringBinary(L&& left, R&& right, Token::Type op);
    template<typename L, typename R>
    static Value::Data _referenceBinary(L&& left, R&& right, Token::Type op);
    template<typename L, typename R>
    static Value::Data _mixedBinary(L&& obj, R&& num, Token::Type op);
    static Value::Data _dispatchBinary(const Value::Data& left, const Value::Data& right, Token::Type op)
    {
        return std::visit(LambdaOverloaded
        {
            [&](auto&& l, auto&& r) -> Value::Data
            requires ((IsNumber<decltype(l)> || IsNil<decltype(l)>) && (IsNumber<decltype(r)> || IsNil<decltype(r)>))
            {
                auto amend = [](auto&& v) -> decltype(auto)
                {
                    if constexpr (IsNil<std::decay_t<decltype(v)>>)
                        return Integer(0);
                    else
                        return std::forward<decltype(v)>(v);
                };
                return {_numberBinary(amend(std::forward<decltype(l)>(l)), amend(std::forward<decltype(r)>(r)), op)};
            },
            [&](auto&& l, auto&& r) -> Value::Data
            requires (IsString<decltype(l)> || IsString<decltype(r)>)
            { return _stringBinary(Value::toString(l), Value::toString(r), op); },
            [&](auto&& l, auto&& r) -> Value::Data
            requires (IsReference<decltype(l)> && IsReference<decltype(r)>)
            { return _referenceBinary(l, r, op); },
            [&](auto&& l, auto&& r) -> Value::Data
            requires (IsReference<decltype(l)> && IsNumber<decltype(r)>)
            { return _mixedBinary(l, r, op); },
            [&](auto&& l, auto&& r) -> Value::Data
            requires (IsNumber<decltype(l)> && IsReference<decltype(r)>)
            { return _mixedBinary(r, l, op); },
            [](auto&&, auto&&) -> Value::Data { return Nil{}; }
        }, left, right);
    }
    template<typename L, typename R>
    static Value::Data _numberBinary(L&& left, R&& right, Token::Type op)
    {
        using namespace Token;
        switch (op)
        {
        case TK_ADD:
            if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
                return Integer(left + right);
            else
                return Float(static_cast<Float>(left) + static_cast<Float>(right));
        case TK_SUB:
            if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
                return Integer(left - right);
            else
                return Float(static_cast<Float>(left) - static_cast<Float>(right));
        case TK_MUL:
            if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
                return Integer(left * right);
            else
                return Float(static_cast<Float>(left) * static_cast<Float>(right));
        case TK_DIV:
            if (static_cast<Float>(right) == 0.0)
                return Nil{};
            if constexpr (IsInteger<L> && IsInteger<R>)
            {
                if (right == -1 && left == std::numeric_limits<Integer>::min())
                    return Integer(static_cast<Float>(left) / static_cast<Float>(right));
                return Integer(left / right);
            }
            else if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
                return Integer(left / right);
            else
                return Float(static_cast<Float>(left) / static_cast<Float>(right));
        case TK_MOD:
            if constexpr (IsInteger<L> && IsInteger<R>)
            {
                if (right == 0) return Nil{};
                if (right == -1 && left == std::numeric_limits<Integer>::min())
                    return Nil{};
                return Integer(left % right);
            }
            else if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
                return Integer(left % right);
            else
                return Float(std::fmod(static_cast<Float>(left), static_cast<Float>(right)));
        case TK_POW:
            if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
                return Integer(std::pow(static_cast<Integer>(left), static_cast<Integer>(right)));
            else
                return Float(std::pow(static_cast<Float>(left), static_cast<Float>(right)));
        case TK_BIT_AND: return static_cast<Integer>(left) & static_cast<Integer>(right);
        case TK_BIT_OR: return static_cast<Integer>(left) | static_cast<Integer>(right);
        case TK_BIT_XOR: return static_cast<Integer>(left) ^ static_cast<Integer>(right);
        case TK_BIT_XNOR: return ~(static_cast<Integer>(left) ^ static_cast<Integer>(right));
        case TK_BIT_SHL:
        {
            Integer l = static_cast<Integer>(left);
            Integer r = static_cast<Integer>(right);
            return Integer(l << (static_cast<uint64_t>(r) & 63));
        }
        case TK_BIT_SHR:
        {
            Integer l = static_cast<Integer>(left);
            Integer r = static_cast<Integer>(right);
            return Integer(l >> (static_cast<uint64_t>(r) & 63));
        }
        case TK_EQ: return Boolean(left == right);
        case TK_NE: return Boolean(left != right);
        case TK_LT: return Boolean(left < right);
        case TK_LE: return Boolean(left <= right);
        case TK_GT: return Boolean(left > right);
        case TK_GE: return Boolean(left >= right);
        default: return Nil{};
        }
    }
    template<typename L, typename R>
    static Value::Data _stringBinary(L&& left, R&& right, Token::Type op)
    {
        using namespace Token;
        if (op == Token::TK_ADD)
            return left + right;
        return Nil{};
    }
    template<typename L, typename R>
    static Value::Data _referenceBinary(L&& left, R&& right, Token::Type op)
    {
        using namespace Token;
        uintptr_t left_addr = Value::toAddress(left);
        uintptr_t right_addr = Value::toAddress(right);
        switch (op)
        {
        case TK_EQ: return Boolean(left_addr == right_addr);
        case TK_NE: return Boolean(left_addr != right_addr);
        case TK_LT: return Boolean(left_addr < right_addr);
        case TK_LE: return Boolean(left_addr <= right_addr);
        case TK_GT: return Boolean(left_addr > right_addr);
        case TK_GE: return Boolean(left_addr >= right_addr);
        default: return Nil{};
        }
    }
    template<typename L, typename R>
    static Value::Data _mixedBinary(L&& obj, R&& num, Token::Type op)
    {
        Integer addi = Value::toInteger(obj);
        return _numberBinary(addi, std::forward<R>(num), op);
    }
#pragma endregion
#pragma region Visit
Evaluator::ResType Evaluator::_execute(const Expr::VarName& n)
{
    if (n.is_assigned)
        return n.name;
    Variable* var = nullptr;
    if (!_env->get(n.name, &var))
        return Nil{};
    return var->value;
}
Evaluator::ResType Evaluator::_execute(const Expr::Assign& n)
{
    Value::Data val = operator()(*n.right);
    if (n.op == TK_WALRUS)
    {
        _env->def(std::get<std::string>(operator()(*n.left)), {val});
        return val;
    }
    Variable* var = nullptr;
    if (!_env->get(std::get<std::string>(operator()(*n.left)), &var))
        return Nil{};
    switch (n.op)
    {
    case TK_ASSIGN: var->value = val; break;
    case TK_SELF_ADD: var->value = _dispatchBinary(var->value, val, TK_ADD); break;
    case TK_SELF_SUB: var->value = _dispatchBinary(var->value, val, TK_SUB); break;
    case TK_SELF_MUL: var->value = _dispatchBinary(var->value, val, TK_MUL); break;
    case TK_SELF_DIV: var->value = _dispatchBinary(var->value, val, TK_DIV); break;
    case TK_SELF_MOD: var->value = _dispatchBinary(var->value, val, TK_MOD); break;
    case TK_SELF_POW: var->value = _dispatchBinary(var->value, val, TK_POW); break;
    case TK_SELF_BIT_AND: var->value = _dispatchBinary(var->value, val, TK_BIT_AND); break;
    case TK_SELF_BIT_OR: var->value = _dispatchBinary(var->value, val, TK_BIT_OR); break;
    case TK_SELF_BIT_XOR: var->value = _dispatchBinary(var->value, val, TK_BIT_XOR); break;
    case TK_SELF_BIT_XNOR: var->value = _dispatchBinary(var->value, val, TK_BIT_XNOR); break;
    case TK_SELF_BIT_NOT: var->value = _dispatchBinary(var->value, val, TK_BIT_NOT); break;
    case TK_SELF_BIT_SHL: var->value = _dispatchBinary(var->value, val, TK_BIT_SHL); break;
    case TK_SELF_BIT_SHR: var->value = _dispatchBinary(var->value, val, TK_BIT_SHR); break;
    default: break;
    }
    return var->value;
}
Evaluator::ResType Evaluator::_execute(const Expr::Unary& n)
{
    Value::Data right = operator()(*n.right);
    switch (n.op)
    {
    case TK_SUB: return -Value::toFloat(right);
    case TK_NOT: return !Value::toBoolean(right);
    case TK_BIT_NOT: return ~Value::toInteger(right);
    default: break;
    }
    return right;
}
Evaluator::ResType Evaluator::_execute(const Expr::Binary& n)
{
    if (n.op == TK_AND || n.op == TK_OR)
    {
        Value::Data left_val = operator()(*n.left);
        bool left_true = Value::toBoolean(left_val);
        if (n.op == TK_AND && !left_true) return left_val;
        else if (n.op == TK_OR && left_true)   return left_val;
        return operator()(*n.right);
    }
    Value::Data left = operator()(*n.left);
    Value::Data right = operator()(*n.right);
    return _dispatchBinary(left, right, n.op);
}
Evaluator::ResType Evaluator::_execute(const Expr::Grouping& n)
{
    return operator()(*n.expression);
}
#pragma endregion
}
