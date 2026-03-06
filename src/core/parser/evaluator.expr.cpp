#include "visitor.hpp"
#include <cmath>
#include <limits>

namespace luna::ast
{
    using namespace Token;
#pragma region Impl
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
            if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
            {
                if (right == -1 && left == std::numeric_limits<Integer>::min())
                    return Integer(static_cast<Float>(left) / static_cast<Float>(right));
                return Integer(left / right);
            }
            else
                return Float(static_cast<Float>(left) / static_cast<Float>(right));
        case TK_MOD:
            if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
            {
                if (right == 0) return Nil{};
                if (right == -1 && left == std::numeric_limits<Integer>::min())
                    return Nil{};
                return Integer(left % right);
            }
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
Value::Data Evaluator::_execute(const Expr::VarName& n)
{
    return Nil{};
}
Value::Data Evaluator::_execute(const Expr::Assign& n)
{
    switch (n.op)
    {
    case TK_ASSIGN: break;
    case TK_WALRUS: break;
    case TK_SELF_ADD: break;
    case TK_SELF_SUB: break;
    case TK_SELF_MUL: break;
    case TK_SELF_DIV: break;
    case TK_SELF_MOD: break;
    case TK_SELF_POW: break;
    case TK_SELF_BIT_AND: break;
    case TK_SELF_BIT_OR: break;
    case TK_SELF_BIT_XOR: break;
    case TK_SELF_BIT_XNOR: break;
    case TK_SELF_BIT_NOT: break;
    case TK_SELF_BIT_SHL: break;
    case TK_SELF_BIT_SHR: break;
    default: break;
    }
    return operator()(*n.right);
}
Value::Data Evaluator::_execute(const Expr::Unary& n)
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
Value::Data Evaluator::_execute(const Expr::Binary& n)
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
            return _numberBinary(amend(std::forward<decltype(l)>(l)), amend(std::forward<decltype(r)>(r)), n.op);
        },
        [&](auto&& l, auto&& r) -> Value::Data
        requires (IsString<decltype(l)> || IsString<decltype(r)>)
        { return _stringBinary(Value::toString(l), Value::toString(r), n.op); },
        [&](auto&& l, auto&& r) -> Value::Data
        requires (IsReference<decltype(l)> && IsReference<decltype(r)>)
        { return _referenceBinary(l, r, n.op); },
        [&](auto&& l, auto&& r) -> Value::Data
        requires (IsReference<decltype(l)> && IsNumber<decltype(r)>)
        { return _mixedBinary(l, r, n.op); },
        [&](auto&& l, auto&& r) -> Value::Data
        requires (IsNumber<decltype(l)> && IsReference<decltype(r)>)
        { return _mixedBinary(r, l, n.op); },
        [](auto&&, auto&&) -> Value::Data { return Nil{}; }
    }, left, right);
}
Value::Data Evaluator::_execute(const Expr::Grouping& n)
{
    return operator()(*n.expression);
}
#pragma endregion
}
