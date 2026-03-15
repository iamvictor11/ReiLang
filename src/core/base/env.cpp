#include "env.hpp"

namespace luna
{
    Bytecode Env::defGlobal(const std::string& name)
    {
        if (auto it = _map.find(name); it != _map.end())
            return it->second;
        Bytecode index = static_cast<Bytecode>(_vals.size());
        _map[name] = index;
        _vals.push_back(Nil{});
        return index;
    }
    Value::Data Env::getGlobal(const std::string& name)
    {
        auto it = _map.find(name);
        if (it == _map.end())
            return Nil{};
        Bytecode index = it->second;
        return _vals[index];
    }
    Value::Data Env::getGlobal(Bytecode index)
    {
        if (index >= _vals.size())
            return Nil{};
        return _vals[index];
    }
    void Env::setGlobal(const std::string& name, Value::Data value)
    {
        auto it = _map.find(name);
        if (it == _map.end())
            return;
        Bytecode index = it->second;
        if (index < _vals.size())
            _vals[index] = value;
    }
    void Env::setGlobal(Bytecode index, Value::Data value)
    {
        if (index < _vals.size())
            _vals[index] = value;
    }
    Bytecode Env::toIndex(const std::string& name) const
    {
        auto it = _map.find(name);
        if (it == _map.end())
            return LUNA_BYTECODE_MAX;
        return it->second;
    }
}
