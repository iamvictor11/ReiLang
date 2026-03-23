#pragma once
#include <cstdint>
#include <cstring>
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
        enum Tag : uint8_t
        {
            VT_NIL,
            VT_BOOLEAN,
            VT_INTEGER,
            VT_FLOAT,
            VT_STRING,
            VT_FUNCTION,
            VT_NATIVE
        };
        struct Data final
        {
        public:
            Tag tag;
            union
            {
                Boolean b;
                Integer i;
                Float f;
                Ref<String> str;
                Ref<Function> func;
                Native native;
            };
        public:
            Data() : tag(VT_NIL) {}
            Data(Boolean v) : tag(VT_BOOLEAN), b(v) {}
            Data(Integer v) : tag(VT_INTEGER), i(v) {}
            Data(Float v)   : tag(VT_FLOAT), f(v) {}
            Data(const Ref<String>& s)  : tag(VT_STRING), str(s) {}
            Data(const std::string& s)  : tag(VT_STRING), str(std::make_shared<String>(s)) {}
            Data(const char* s)         : tag(VT_STRING), str(std::make_shared<String>(s)) {}
            Data(const Ref<Function>& f)    : tag(VT_FUNCTION), func(f) {}
            Data(Native n) : tag(VT_NATIVE), native(n) {}
            Data(const Data& other) : tag(other.tag)
            {
                switch (tag)
                {
                case VT_STRING:     new (&str) Ref<String>(other.str); break;
                case VT_FUNCTION:   new (&func) Ref<Function>(other.func); break;
                default:            std::memcpy(this, &other, sizeof(Data)); break;
                }
            }
            Data& operator=(const Data& other)
            {
                if (this == &other) return *this;
                this->~Data();
                new (this) Data(other);
                return *this;
            }
            ~Data()
            {
                switch (tag)
                {
                case VT_STRING:   str.~shared_ptr(); break;
                case VT_FUNCTION: func.~shared_ptr(); break;
                default: break;
                }
            }
        public:
            bool isBoolean() const { return tag == VT_BOOLEAN; }
            bool isInteger() const { return tag == VT_INTEGER; }
            bool isFloat() const { return tag == VT_FLOAT; }
            bool isNumber() const { return tag == VT_INTEGER || tag == VT_FLOAT || tag == VT_BOOLEAN; }
            bool isString() const { return tag == VT_STRING; }
            bool isFunction() const { return tag == VT_FUNCTION; }
            bool isNative() const { return tag == VT_NATIVE; }
        public:
            Boolean asBoolean() const { return b; }
            Integer asInteger() const { return i; }
            Float asFloat() const { return f; }
            const String& asString() const { return *(str.get()); }
            Ref<Function> asFunction() const { return func; }
            Native asNative() const { return native; }
        public:
            Boolean toBoolean() const;
            Integer toInteger() const;
            Float toFloat() const;
            String toString() const;
        };
        std::string dump(const Data& data);
    }
    using Nil = Value::Data;

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
        std::vector<Value::Data> onces;
    };
}
