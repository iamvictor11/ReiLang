#include "env.hpp"

namespace vic
{
    Env::Env()
    {
        enter();
    }
    Env::~Env()
    {
        exit();
    }
    void Env::enter()
    {
        _scopes.emplace_back();
    }
    void Env::exit()
    {
        _scopes.pop_back();
    }
    Bytecode Env::curr()
    {
        return _scopes.size() - 1;
    }
    void Env::clear()
    {
        _scopes.clear();
        enter();
    }
    Bytecode Env::def(const std::string& name)
    {
        return def(name, Nil{}, curr());
    }
    Bytecode Env::def(const std::string& name, Value::Data value)
    {
        return def(name, value, curr());
    }
    Bytecode Env::def(const std::string& name, Value::Data value, Bytecode depth)
    {
        _Scope& curr = _scopes.at(depth);
        if (auto it = curr.map.find(name); it != curr.map.end())
            return it->second;
        Bytecode index = static_cast<Bytecode>(curr.values.size());
        curr.map[name] = index;
        curr.values.push_back(value);
        return index;
    }
    void Env::def(Bytecode index)
    {
        def(index, Nil{}, curr());
    }
    void Env::def(Bytecode index, Value::Data value)
    {
        def(index, value, curr());
    }
    void Env::def(Bytecode index, Value::Data value, Bytecode depth)
    {
        _Scope& scope = _scopes.at(depth);
        if (index < scope.values.size())
            scope.values.at(index) = value;
        else
            scope.values.push_back(value);
    }
    Value::Data Env::get(const std::string& name)
    {
        return get(name, curr());
    }
    Value::Data Env::get(const std::string& name, Bytecode depth)
    {
        for (;;)
        {
            _Scope& scope = _scopes.at(depth);
            if (auto it = scope.map.find(name); it != scope.map.end())
                return scope.values.at(it->second);
            if (depth == 0)
                break;
            depth--;
        }
        return Nil{};
    }
    Value::Data Env::get(Bytecode index)
    {
        return get(index, curr());
    }
    Value::Data Env::get(Bytecode index, Bytecode depth)
    {
        for (;;)
        {
            _Scope& scope = _scopes.at(depth);
            if (index < scope.values.size())
                return scope.values.at(index);
            if (depth == 0)
                break;
            depth--;
        }
        return Nil{};
    }
    void Env::set(const std::string& name, Value::Data value)
    {
        set(name, value, curr());
    }
    void Env::set(const std::string& name, Value::Data value, Bytecode depth)
    {
        for (;;)
        {
            _Scope& scope = _scopes.at(depth);
            if (auto it = scope.map.find(name); it != scope.map.end())
            {
                Bytecode index = it->second;
                scope.values.at(index) = value;
                return;
            }
            if (depth == 0)
                break;
            depth--;
        }
    }
    void Env::set(Bytecode index, Value::Data value)
    {
        set(index, value, curr());
    }
    void Env::set(Bytecode index, Value::Data value, Bytecode depth)
    {
        for (;;)
        {
            _Scope& scope = _scopes.at(depth);
            if (index < scope.values.size())
            {
                scope.values.at(index) = value;
                return;
            }
            if (depth == 0)
                break;
            depth--;
        }
    }
    Bytecode Env::toIndex(const std::string& name)
    {
        return toIndex(name, curr());
    }
    Bytecode Env::toIndex(const std::string& name, Bytecode depth)
    {
        _Scope& curr = _scopes.at(depth);
        auto it = curr.map.find(name);
        if (it == curr.map.end())
            return VIC_BYTECODE_MAX;
        return it->second;
    }
}
