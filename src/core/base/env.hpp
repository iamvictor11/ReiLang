#pragma once
#include "common.hpp"

namespace luna
{
    class Env final
    {
    private:
        struct _Scope final
        {
            std::unordered_map<std::string, Bytecode> map {};
            std::vector<Value::Data> values {};
        };
    private:
        std::vector<_Scope> _scopes {};
    public:
        Env();
        ~Env();
    public:
        void enter();
        void exit();
        Bytecode curr();
    public:
        void clear();
    public:
        Bytecode def(const std::string& name);
        Bytecode def(const std::string& name, Value::Data value);
        Bytecode def(const std::string& name, Value::Data value, Bytecode depth);
        void def(Bytecode index);
        void def(Bytecode index, Value::Data value);
        void def(Bytecode index, Value::Data value, Bytecode depth);
    public:
        Value::Data get(const std::string& name);
        Value::Data get(const std::string& name, Bytecode depth);
        Value::Data get(Bytecode index);
        Value::Data get(Bytecode index, Bytecode depth);
    public:
        void set(const std::string& name, Value::Data value);
        void set(const std::string& name, Value::Data value, Bytecode depth);
        void set(Bytecode index, Value::Data value);
        void set(Bytecode index, Value::Data value, Bytecode depth);
    public:
        Bytecode toIndex(const std::string& name);
        Bytecode toIndex(const std::string& name, Bytecode depth);
    };
}