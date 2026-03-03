#pragma once
#include <cstdint>
#include <variant>
#include <memory>
#include <unordered_map>
#include <vector>

namespace luna
{
    struct Position final
    {
    public:
        uint64_t line = 1, column = 1;
    public:
        std::string toString() const
        {
            return std::to_string(line) + ":" + std::to_string(column);
        }
    };

    struct Table;
    struct Function;

    using Nil = std::monostate;
    using Bool = bool;
    using Int = int64_t;
    using Float = double;
    using String = std::string;
    template<typename T>
    using Ref = std::shared_ptr<T>;

    namespace Value
    {
        using Data = std::variant<Nil, Bool, Int, Float, String, Ref<Table>, Ref<Function>>;
        std::string toString(Value::Data data);
    }

    struct Variable final
    {
    public:
        Value::Data value;
        bool is_const = false;
    public:
        Variable() = default;
        Variable(Value::Data v) : value(std::move(v)) {}
        Variable(Value::Data v, bool ic) : value(std::move(v)), is_const(ic) {}
    public:
        template<typename T>
        bool is() const
        {
            return std::holds_alternative<T>(value);
        }
        template<typename T>
        T& as()
        {
            return std::get<T>(value);
        }
        template<typename T>
        const T& as() const
        {
            return std::get<T>(value);
        }
    };
    struct Table final : public std::enable_shared_from_this<Table>
    {
    public:
        static constexpr size_t npos = static_cast<size_t>(-1);
    public:
        std::vector<Value::Data> data;
        std::unordered_map<String, size_t> map;
    public:
        size_t size() const;
        size_t capacity() const;
        Value::Data get(size_t index) const;
        void set(size_t index, Value::Data value);
        void remove(size_t index);
        void erase(size_t index);
        void insert(size_t index, Value::Data value);
        Value::Data get(const String& key) const;
        void set(const String& key, Value::Data value);
        void remove(const String& key);
        void erase(const String& key);
        void affix(size_t index, const String& key);
        void detach(const String& key);
        void clear();
        size_t find(const Value::Data& value) const;
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
        Kind kind = Kind::NATIVE;
    };
}
