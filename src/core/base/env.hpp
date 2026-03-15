#pragma once
#include "common.hpp"

namespace luna
{
    class Env final
    {
    private:
        std::unordered_map<std::string, Bytecode> _map {};
        std::vector<Value::Data> _vals {};
    public:
        Env() = default;
        Bytecode defGlobal(const std::string& name);
        Value::Data getGlobal(const std::string& name);
        Value::Data getGlobal(Bytecode index);
        void setGlobal(const std::string& name, Value::Data value);
        void setGlobal(Bytecode index, Value::Data value);
        Bytecode toIndex(const std::string& name) const;
    };
}