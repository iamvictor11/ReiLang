#pragma once
#include <cstdint>
#include <vector>

namespace rei
{
    struct Obj;

    enum class ValueType
    {
        Nil,
        Bool,
        Int,
        Float,
        Obj
    };
    struct Value final
    {
    public:
        ValueType type;
        union
        {
        bool vBool;
        int64_t vInt;
        double vFloat;
        Obj* pObj;
        };
    public:
        static auto mkNil() -> Value;
        static auto mkBool(bool v) -> Value;
        static auto mkInt(int64_t v) -> Value;
        static auto mkFloat(double v) -> Value;
        static auto mkNumber(double v) -> Value;
        static auto mkObj(Obj* p) -> Value;
    public:
        bool isNil() const { return type == rei::ValueType::Nil; };
        bool isBool() const { return type == rei::ValueType::Bool; };
        bool isInt() const { return type == rei::ValueType::Int; };
        bool isFloat() const { return type == rei::ValueType::Float; };
        bool isNumber() const { return type == rei::ValueType::Bool || type == rei::ValueType::Int || type == rei::ValueType::Float; };
        bool isObj() const { return type == rei::ValueType::Obj; };
    public:
        auto asBool() const -> bool { return vBool; };
        auto asInt() const -> int64_t { return vInt; };
        auto asFloat() const -> double { return vFloat; };
        auto asNumber() const -> double { return vFloat; };
        auto asObj() -> Obj* { return pObj; };
        auto asObj() const -> const Obj* { return pObj; };
    };
    inline auto Value::mkNil() -> Value { return {rei::ValueType::Nil,   {.vBool = false}}; }
    inline auto Value::mkBool(bool v) -> Value { return {rei::ValueType::Bool,  {.vBool = v}}; }
    inline auto Value::mkInt(int64_t v) -> Value { return {rei::ValueType::Int,   {.vInt = v}}; }
    inline auto Value::mkFloat(double v) -> Value { return {rei::ValueType::Float, {.vFloat = v}}; }
    inline auto Value::mkNumber(double v) -> Value { return {rei::ValueType::Float, {.vFloat = v}}; }
    inline auto Value::mkObj(Obj* p) -> Value { return {rei::ValueType::Obj,   {.pObj = p}}; }
    using ValueArray = std::vector<Value>;
}
