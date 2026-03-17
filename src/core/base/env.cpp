#include "env.hpp"
#include <iostream>

namespace rei
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
        _staticBlock.emplace_back();
    }
    void Env::exit()
    {
        _staticBlock.pop_back();
    }
    Bytecode Env::curr()
    {
        return _staticBlock.size() - 1;
    }
    void Env::clear()
    {
        _staticBlock.clear();
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
        _Scope& scope = _staticBlock.at(depth);
        if (auto it = scope.map.find(name); it != scope.map.end())
            return {depth, it->second};
        Bytecode slot = static_cast<Bytecode>(scope.stack.size());
        scope.map[name] = slot;
        scope.stack.push_back(value);
        return {depth, slot};
    }
    Value::Data Env::get(const std::string& name)
    {
        return get(name, curr());
    }
    Value::Data Env::get(const std::string& name, Bytecode depth)
    {
        for (;;)
        {
            if (depth >= _staticBlock.size())
                break;
            _Scope& scope = _staticBlock.at(depth);
            if (auto it = scope.map.find(name); it != scope.map.end())
                return scope.stack.at(it->second);
            if (depth == 0)
                break;
            depth--;
        }
        return Nil{};
    }
    Value::Data Env::get(Coord c)
    {
        if (c.depth == REI_BYTECODE_MAX) return Nil{};
        if (c.slot == REI_BYTECODE_MAX) return Nil{};
        Bytecode depth = c.depth;
        Bytecode slot = c.slot;
        for (;;)
        {
            if (depth >= _staticBlock.size())
                break;
            _Scope& scope = _staticBlock.at(depth);
            if (slot < scope.stack.size())
                return scope.stack.at(slot);
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
            if (depth >= _staticBlock.size())
                break;
            _Scope& scope = _staticBlock.at(depth);
            if (auto it = scope.map.find(name); it != scope.map.end())
            {
                Bytecode slot = it->second;
                scope.stack.at(slot) = value;
                return;
            }
            if (depth == 0)
                break;
            depth--;
        }
    }
    void Env::set(Coord c, Value::Data value)
    {
        if (c.depth == REI_BYTECODE_MAX) return;
        if (c.slot == REI_BYTECODE_MAX) return;
        Bytecode depth = c.depth;
        Bytecode slot = c.slot;
        for (;;)
        {
            if (depth >= _staticBlock.size())
                break;
            _Scope& scope = _staticBlock.at(depth);
            if (slot < scope.stack.size())
            {
                scope.stack.at(slot) = value;
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
            _Scope& scope = _staticBlock.at(depth);
            if (auto it = scope.map.find(name); it != scope.map.end())
                return {depth, it->second};
            if (depth == 0)
                break;
            depth--;
        }
        return {};
    }
}
