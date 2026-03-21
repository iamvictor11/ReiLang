#include "core/base/common.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

namespace rei
{
    static Value::Data _dispatchUnary(const Value::Data& v, Opcode op);
    static Value::Data _dispatchBinary(const Value::Data& l, const Value::Data& r, Opcode op);
    static Value::Data _dispatchUnary(const Value::Data& v, Opcode op)
    {
        using namespace Value;
        switch (v.tag)
        {
        case VT_NIL:
            switch (op)
            {
            case OP_NEG:        return Integer(0);
            case OP_BIT_NOT:    return Integer(-1);
            case OP_NOT:        return Boolean(true);
            default:            return Nil{};
            }
        case VT_INTEGER:
            switch (op)
            {
            case OP_NEG:        return Integer(-v.i);
            case OP_BIT_NOT:    return Integer(~v.i);
            case OP_NOT:        return Boolean(!v.i);
            default:            return Nil{};
            }
        case VT_FLOAT:
            switch (op)
            {
            case OP_NEG:        return Float(-v.f);
            case OP_BIT_NOT:    return Integer(~(Integer)v.f);
            case OP_NOT:        return Boolean(!v.f);
            default:            return Nil{};
            }
        case VT_BOOLEAN:
            switch (op)
            {
            case OP_NEG:
            case OP_BIT_NOT:
            case OP_NOT: return Boolean(!v.b);
            default:     return Nil{};
            }
        case VT_STRING:
            switch (op)
            {
            case OP_NEG:
            case OP_BIT_NOT:
            case OP_NOT:
            {
                std::string str = *v.str;
                std::reverse(str.begin(), str.end());
                return str;
            }
            default:    return Nil{};
            }
        default:
            return Nil{};
        }
    }
    static Value::Data _dispatchBinary(const Value::Data& a, const Value::Data& b, Opcode op)
    {
        using namespace Value;
        if (a.tag == VT_INTEGER && b.tag == VT_INTEGER)
        {
            Integer l = a.i;
            Integer r = b.i;

            switch (op)
            {
            case OP_ADD: return Integer(l + r);
            case OP_SUB: return Integer(l - r);
            case OP_MUL: return Integer(l * r);
            case OP_DIV:
                if (r == 0) return Nil{};
                if (r == -1 && l == std::numeric_limits<Integer>::min())
                    return Float((Float)l / (Float)r);
                return Integer(l / r);
            case OP_MOD:
                if (r == 0) return Nil{};
                if (r == -1 && l == std::numeric_limits<Integer>::min())
                    return Nil{};
                return Integer(l % r);
            case OP_POW:
                return Integer((Integer)std::pow(l, r));
            case OP_BIT_AND: return Integer(l & r);
            case OP_BIT_OR:  return Integer(l | r);
            case OP_BIT_XOR: return Integer(l ^ r);
            case OP_BIT_XNOR:return Integer(~(l ^ r));
            case OP_BIT_SHL: return Integer(l << (r & 63));
            case OP_BIT_SHR: return Integer(l >> (r & 63));
            case OP_EQ: return Boolean(l == r);
            case OP_NE: return Boolean(l != r);
            case OP_LT: return Boolean(l <  r);
            case OP_LE: return Boolean(l <= r);
            case OP_GT: return Boolean(l >  r);
            case OP_GE: return Boolean(l >= r);
            default: return Nil{};
            }
        }
        if ((a.tag == VT_FLOAT || a.tag == VT_INTEGER) && (b.tag == VT_FLOAT || b.tag == VT_INTEGER))
        {
            Float l = a.toFloat();
            Float r = b.toFloat();
            switch (op)
            {
            case OP_ADD: return Float(l + r);
            case OP_SUB: return Float(l - r);
            case OP_MUL: return Float(l * r);
            case OP_DIV: return r != 0.0 ? Float(l / r) : Nil{};
            case OP_MOD: return Float(std::fmod(l, r));
            case OP_POW: return Float(std::pow(l, r));
            case OP_EQ: return Boolean(l == r);
            case OP_NE: return Boolean(l != r);
            case OP_LT: return Boolean(l <  r);
            case OP_LE: return Boolean(l <= r);
            case OP_GT: return Boolean(l >  r);
            case OP_GE: return Boolean(l >= r);
            default: return Nil{};
            }
        }
        if (a.tag == VT_STRING || b.tag == VT_STRING)
        {
            if (op == OP_ADD)
                return String(a.toString() + b.toString());
            return Nil{};
        }
        if (a.isFunction() && b.isNumber())
        {
            Integer l = a.toInteger();
            return _dispatchBinary(Integer(l), b, op);
        }
        if (a.isNumber() && b.isFunction())
        {
            Integer r = b.toInteger();
            return _dispatchBinary(a, Integer(r), op);
        }
        return Nil{};
    }
}
