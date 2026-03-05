#include "visitor.hpp"
#include <iostream>
#include <cmath>
#include <limits>

namespace luna::ast
{
    template<typename L, typename R>
    Value::Data Evaluator::_binaryDist(L&& left, R&& right, Token::Type op)
    {
        if constexpr (IsNil<L> && IsNil<R>)
            return _numberBinary(Integer(0), Integer(0), op);
        // Number
        else if constexpr (IsNil<L> && IsNumber<R>)
            return _numberBinary(Integer(0), std::forward<R>(right), op);
        else if constexpr (IsNumber<L> && IsNil<R>)
            return _numberBinary(std::forward<L>(left), Integer(0), op);
        else if constexpr (IsNumber<L> && IsNumber<R>)
            return _numberBinary(std::forward<L>(left), std::forward<R>(right), op);
        else if constexpr (IsReference<L> && IsReference<R>)
            return _referenceBinary(std::forward<L>(left), std::forward<R>(right), op);
        else if constexpr (IsNumber<L> && IsReference<R>)
            return _mixedBinary(std::forward<R>(right), std::forward<L>(left), op);
        else if constexpr (IsReference<L> && IsNumber<R>)
            return _mixedBinary(std::forward<L>(left), std::forward<R>(right), op);
        // String
        else if constexpr (IsString<L> && IsString<R>)
            return _stringBinary(left, right, op);
        else if constexpr (!IsString<L> && IsString<R>)
            return _stringBinary(Value::toString(left), right, op);
        else if constexpr (IsString<L> && !IsString<R>)
            return _stringBinary(left, Value::toString(right), op);
        else
            return Nil{};
    }
    template<typename L, typename R>
    Value::Data Evaluator::_numberBinary(L&& left, R&& right, Token::Type op)
    {
        using namespace Token;
        switch (op)
        {
        case TK_ADD:
            if constexpr (IsInteger<L> && IsInteger<R>)
                return Integer(left + right);
            else
                return Float(static_cast<Float>(left) + static_cast<Float>(right));
        case TK_SUB:
            if constexpr (IsInteger<L> && IsInteger<R>)
                return Integer(left - right);
            else
                return Float(static_cast<Float>(left) - static_cast<Float>(right));
        case TK_MUL:
            if constexpr (IsInteger<L> && IsInteger<R>)
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
            else
                return Float(std::fmod(static_cast<Float>(left), static_cast<Float>(right)));
        case TK_POW:
            if constexpr (IsInteger<L> && IsInteger<R>)
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
    Value::Data Evaluator::_stringBinary(L&& left, R&& right, Token::Type op)
    {
        using namespace Token;
        if (op == Token::TK_ADD)
            return left + right;
        return Nil{};
    }
    template<typename L, typename R>
    Value::Data Evaluator::_referenceBinary(L&& left, R&& right, Token::Type op)
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
    Value::Data Evaluator::_mixedBinary(L&& obj, R&& num, Token::Type op)
    {
        Integer addi = Value::toInteger(obj);
        return _numberBinary(addi, std::forward<R>(num), op);
    }

    Value::Data Evaluator::operator()(const Node& node)
    {
        using namespace Token;
        return std::visit(overloaded
        {
            [](const Expr::Literal& e) -> Value::Data { return e.value; },
            [&](const Expr::Unary& e) -> Value::Data
            {
                Value::Data right = this->operator()(*e.right);
                switch (e.op)
                {
                case TK_SUB: return -Value::toFloat(right);
                case TK_NOT: return !Value::toBoolean(right);
                case TK_BIT_NOT: return ~Value::toInteger(right);
                default: break;
                }
                return right;
            },
            [&](const Expr::Binary& e) -> Value::Data
            {
                if (e.op == TK_ASSIGN)
                {
                    Value::Data val = this->operator()(*e.right);
                    return val;
                }
                if (e.op == TK_AND || e.op == TK_OR)
                {
                    Value::Data left_val = this->operator()(*e.left);
                    bool left_true = Value::toBoolean(left_val);
                    if (e.op == TK_AND && !left_true) return left_val;
                    else if (e.op == TK_OR && left_true)   return left_val;
                    return this->operator()(*e.right);
                }
                Value::Data left = this->operator()(*e.left);
                Value::Data right = this->operator()(*e.right);
                return std::visit([&](auto&& l, auto&& r) ->
                    Value::Data { return _binaryDist(std::forward<decltype(l)>(l), std::forward<decltype(r)>(r), e.op); },
                    left, right);
                // case TK_ASSIGN: break;
                // case TK_WALRUS: break;
                // case TK_SELF_ADD: break;
                // case TK_SELF_SUB: break;
                // case TK_SELF_MUL: break;
                // case TK_SELF_DIV: break;
                // case TK_SELF_MOD: break;
                // case TK_SELF_POW: break;
                // case TK_SELF_BIT_AND: break;
                // case TK_SELF_BIT_OR: break;
                // case TK_SELF_BIT_XOR: break;
                // case TK_SELF_BIT_XNOR: break;
                // case TK_SELF_BIT_NOT: break;
                // case TK_SELF_BIT_SHL: break;
                // case TK_SELF_BIT_SHR: break;
            },
            [&](const Expr::Grouping& e) -> Value::Data
            {
                return this->operator()(*e.expression);
            }
        }, node.data);
    }
}
