#include "env.hpp"
#include <iostream>

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
    Env::Coord Env::def(const std::string& name)
    {
        return def(name, Nil{}, curr());
    }
    Env::Coord Env::def(const std::string& name, Value::Data value)
    {
        return def(name, value, curr());
    }
    Env::Coord Env::def(const std::string& name, Value::Data value, Bytecode depth)
    {
        _Scope& scope = _scopes.at(depth);
        if (auto it = scope.map.find(name); it != scope.map.end())
            return {depth, it->second};
        Bytecode index = static_cast<Bytecode>(scope.values.size());
        scope.map[name] = index;
        scope.values.push_back(value);
        return {depth, index};
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
    Value::Data Env::get(Coord c)
    {
        if (c.depth == VIC_BYTECODE_MAX) return Nil{};
        if (c.index == VIC_BYTECODE_MAX) return Nil{};
        Bytecode depth = c.depth;
        Bytecode index = c.index;
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
    void Env::set(Coord c, Value::Data value)
    {
        if (c.depth == VIC_BYTECODE_MAX) return;
        if (c.index == VIC_BYTECODE_MAX) return;
        Bytecode depth = c.depth;
        Bytecode index = c.index;
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
    Env::Coord Env::toCoord(const std::string& name)
    {
        return toCoord(name, curr());
    }
    Env::Coord Env::toCoord(const std::string& name, Bytecode depth)
    {
        for (;;)
        {
            _Scope& scope = _scopes.at(depth);
            if (auto it = scope.map.find(name); it != scope.map.end())
                return {depth, it->second};
            if (depth == 0)
                break;
            depth--;
        }
        return {};
    }
}
