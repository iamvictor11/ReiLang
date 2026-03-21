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

    template<typename T>
    using Ptr = T*;
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T>
    using Obs = std::weak_ptr<T>;
    template<typename T>
    using Uno = std::unique_ptr<T>;
    
    namespace Value
    {
        struct Data;
    }
    using Nil = std::monostate;
    using Boolean = bool;
    using Integer = int64_t;
    using Float = double;
    using String = std::string;
    using Array = std::vector<struct Value::Data>;
    using Map = std::unordered_map<String, struct Value::Data>;
    struct Function;
    using Native = Value::Data(*)(REI_BYTECODE_TYPE argc, Value::Data argv[]);

    namespace Value
    {
        struct Data : std::variant<
            Nil,
            Boolean, Integer, Float,
            String,
            // Ref<Array>,
            // Ref<Map>,
            Ref<Function>,
            Native
        >
        {
            using variant::variant;
            using variant::operator=;
            Data() = default;
            Data(const Data&) = default;
            Data(Data&&) = default;
            Data& operator=(const Data&) = default;
            Data& operator=(Data&&) = default;
            template<typename T>
            Data(T&& value) : variant(std::forward<T>(value)) {}
        };
        bool toBoolean(const Value::Data& data);
        Integer toInteger(const Value::Data& data);
        Float toFloat(const Value::Data& data);
        String toString(const Value::Data& data);
        String getDebugString(const Value::Data& data);
        template<typename T>
        bool is(const Value::Data& data)
        {
            return std::holds_alternative<T>(data);
        }
        template<typename T>
        T& as(Value::Data& data)
        {
            return std::get<T>(data);
        }
        template<typename T>
        const T& as(const Value::Data& data)
        {
            return std::get<T>(data);
        }
    }

    template<typename T, typename U>
    constexpr bool is_type = std::is_same_v<T, U>;
    template<typename T>
    concept IsNil = is_type<std::decay_t<T>, Nil>;
    template<typename T>
    concept IsInteger = is_type<std::decay_t<T>, Integer>;
    template<typename T>
    concept IsFloat = is_type<std::decay_t<T>, Float>;
    template<typename T>
    concept IsBoolean = is_type<std::decay_t<T>, Boolean>;
    template<typename T>
    concept IsNumber = is_type<std::decay_t<T>, Integer> || is_type<std::decay_t<T>, Float> || is_type<std::decay_t<T>, Boolean>;
    template<typename T>
    concept IsString = is_type<std::decay_t<T>, String>;
    template<typename T>
    concept IsObject = is_type<std::decay_t<T>, Ref<Function>> || is_type<std::decay_t<T>, Native>;
    template<typename T, typename U>
    concept IsX = is_type<std::decay_t<T>, U>;

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
        Chunk chunk {};
        Bytecode argc = 0;
    };
}
