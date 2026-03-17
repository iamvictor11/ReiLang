#pragma once
#include <cstdint>
#include <variant>
#include <memory>
#include <unordered_map>
#include <vector>
#include "bytecode.hpp"

namespace rei
{
    struct Position final
    {
    public:
        uint64_t line = 1, column = 1;
    public:
        std::string toString() const
        {
            return "[" + std::to_string(line) + ":" + std::to_string(column) + "]";
        }
    };

    class VM;

    struct Function;

    using Nil = std::monostate;
    using Boolean = bool;
    using Integer = int64_t;
    using Float = double;
    using String = std::string;
    template<typename T>
    using Ref = std::shared_ptr<T>;

    namespace Value
    {
        using Data = std::variant<Nil, Boolean, Integer, Float, String, Ref<Function>>;
        bool toBoolean(const Value::Data& data);
        Integer toInteger(const Value::Data& data);
        Float toFloat(const Value::Data& data);
        std::string toString(Value::Data data);
        template<typename T>
        uintptr_t toAddress(const Ref<T>& ref)
        {
            if (!ref) return 0;
            return reinterpret_cast<uintptr_t>(&(*ref));
        }
        std::string getDebugString(Value::Data data);
        // TODO: 声明周期暂时不考虑
        enum Lifetime : Bytecode
        {
            VL_STACK,
            VL_HEAP
        };
    }

    template<typename T>
    constexpr bool is_nil = std::is_same_v<T, Nil>;
    template<typename T>
    constexpr bool is_int = std::is_same_v<T, Integer>;
    template<typename T>
    constexpr bool is_float = std::is_same_v<T, Float>;
    template<typename T>
    constexpr bool is_bool = std::is_same_v<T, Boolean>;
    template<typename T>
    constexpr bool is_num = std::is_same_v<T, Integer> || std::is_same_v<T, Float> || std::is_same_v<T, Boolean>;
    template<typename T>
    constexpr bool is_str = std::is_same_v<T, String>;
    template<typename T>
    constexpr bool is_ref = std::is_same_v<T, Ref<Function>>;
    template<typename T>
    concept IsNil = is_nil<std::decay_t<T>>;
    template<typename T>
    concept IsInteger = is_int<std::decay_t<T>>;
    template<typename T>
    concept IsFloat = is_float<std::decay_t<T>>;
    template<typename T>
    concept IsBoolean = is_bool<std::decay_t<T>>;
    template<typename T>
    concept IsNumber = is_num<std::decay_t<T>>;
    template<typename T>
    concept IsString = is_str<std::decay_t<T>>;
    template<typename T>
    concept IsReference = is_ref<std::decay_t<T>>;

    struct Chunk final
    {
    public:
        std::vector<Value::Data> constants {};
        std::vector<Bytecode> codes {};
    public:
        void clear();
    };
    struct Function final : public std::enable_shared_from_this<Function>
    {
    public:
        enum class Kind : uint8_t
        {
            NATIVE,
            SCRIPT
        };
    public:
        Kind kind = Kind::SCRIPT;
        Chunk chunk {};
        Bytecode upvalue_count = 0;
    };
}
