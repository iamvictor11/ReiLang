#include "core/base/common.hpp"
#include "util/visitor.hpp"
#include <cmath>
#include <limits>

namespace vic
{
    static Value::Data _dispatchUnary(const Value::Data& value, Opcode op);
    static Value::Data _dispatchBinary(const Value::Data& left, const Value::Data& right, Opcode op);
    template<typename L, typename R>
    static Value::Data _numberBinary(L&& left, R&& right, Opcode op);
    template<typename L, typename R>
    static Value::Data _stringBinary(L&& left, R&& right, Opcode op);
    template<typename L, typename R>
    static Value::Data _referenceBinary(L&& left, R&& right, Opcode op);
    template<typename L, typename R>
    static Value::Data _mixedBinary(L&& obj, R&& num, Opcode op);
    static Value::Data _dispatchUnary(const Value::Data& value, Opcode op)
    {
        return std::visit(LambdaOverloaded
        {
            [&](auto&& v) -> Value::Data
            requires (IsNil<decltype(v)>)
            {
                switch (op)
                {
                case OP_NEG: return Integer(0);
                case OP_BIT_NOT: return Integer(-1);
                case OP_NOT: return Boolean(true);
                default: break;
                }
                return Nil{};
            },
            [&](auto&& v) -> Value::Data
            requires (IsInteger<decltype(v)>)
            {
                switch (op)
                {
                case OP_NEG: return -v;
                case OP_BIT_NOT: return ~v;
                case OP_NOT: return !v;
                default: break;
                }
                return Nil{};
            },
            [&](auto&& v) -> Value::Data
            requires (IsFloat<decltype(v)>)
            {
                switch (op)
                {
                case OP_NEG: return -v;
                case OP_BIT_NOT: return Float(~Integer(v));
                case OP_NOT: return !v;
                default: break;
                }
                return Nil{};
            },
            [&](auto&& v) -> Value::Data
            requires (IsBoolean<decltype(v)>)
            {
                switch (op)
                {
                case OP_NEG:
                case OP_BIT_NOT:
                case OP_NOT: return !v;
                default: break;
                }
                return Nil{};
            },
            [&](auto&& v) -> Value::Data
            requires (IsString<decltype(v)>)
            { return Nil{}; },
            [&](auto&& v) -> Value::Data
            requires (IsReference<decltype(v)>)
            { return Nil{}; },
            [](auto&&, auto&&) -> Value::Data { return Nil{}; }
        }, value);
    }
    static Value::Data _dispatchBinary(const Value::Data& left, const Value::Data& right, Opcode op)
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
    static Value::Data _numberBinary(L&& left, R&& right, Opcode op)
    {
        switch (op)
        {
        case OP_ADD:
            if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
                return Integer(left + right);
            else
                return Float(static_cast<Float>(left) + static_cast<Float>(right));
        case OP_SUB:
            if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
                return Integer(left - right);
            else
                return Float(static_cast<Float>(left) - static_cast<Float>(right));
        case OP_MUL:
            if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
                return Integer(left * right);
            else
                return Float(static_cast<Float>(left) * static_cast<Float>(right));
        case OP_DIV:
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
        case OP_MOD:
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
        case OP_POW:
            if constexpr ((IsInteger<L> || IsBoolean<L>) && (IsInteger<R> || IsBoolean<R>))
                return Integer(std::pow(static_cast<Integer>(left), static_cast<Integer>(right)));
            else
                return Float(std::pow(static_cast<Float>(left), static_cast<Float>(right)));
        case OP_BIT_AND: return static_cast<Integer>(left) & static_cast<Integer>(right);
        case OP_BIT_OR: return static_cast<Integer>(left) | static_cast<Integer>(right);
        case OP_BIT_XOR: return static_cast<Integer>(left) ^ static_cast<Integer>(right);
        case OP_BIT_XNOR: return ~(static_cast<Integer>(left) ^ static_cast<Integer>(right));
        case OP_BIT_SHL:
        {
            Integer l = static_cast<Integer>(left);
            Integer r = static_cast<Integer>(right);
            return Integer(l << (static_cast<uint64_t>(r) & 63));
        }
        case OP_BIT_SHR:
        {
            Integer l = static_cast<Integer>(left);
            Integer r = static_cast<Integer>(right);
            return Integer(l >> (static_cast<uint64_t>(r) & 63));
        }
        case OP_EQ: return Boolean(left == right);
        case OP_NE: return Boolean(left != right);
        case OP_LT: return Boolean(left < right);
        case OP_LE: return Boolean(left <= right);
        case OP_GT: return Boolean(left > right);
        case OP_GE: return Boolean(left >= right);
        default: return Nil{};
        }
    }
    template<typename L, typename R>
    static Value::Data _stringBinary(L&& left, R&& right, Opcode op)
    {
        if (op == OP_ADD)
            return left + right;
        return Nil{};
    }
    template<typename L, typename R>
    static Value::Data _referenceBinary(L&& left, R&& right, Opcode op)
    {
        uintptr_t left_addr = Value::toAddress(left);
        uintptr_t right_addr = Value::toAddress(right);
        switch (op)
        {
        case OP_EQ: return Boolean(left_addr == right_addr);
        case OP_NE: return Boolean(left_addr != right_addr);
        case OP_LT: return Boolean(left_addr < right_addr);
        case OP_LE: return Boolean(left_addr <= right_addr);
        case OP_GT: return Boolean(left_addr > right_addr);
        case OP_GE: return Boolean(left_addr >= right_addr);
        default: return Nil{};
        }
    }
    template<typename L, typename R>
    static Value::Data _mixedBinary(L&& obj, R&& num, Opcode op)
    {
        Integer addi = Value::toInteger(obj);
        return _numberBinary(addi, std::forward<R>(num), op);
    }
}
