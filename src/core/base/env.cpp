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
        bool is_free = _nested.empty() ? false : _nested.back().is_free;
        _nested.emplace_back().is_free = is_free;
    }
    void Env::enter(bool is_free)
    {
        _nested.emplace_back().is_free = is_free;
    }
    void Env::exit()
    {
        _nested.pop_back();
    }
    Bytecode Env::currDepth()
    {
        return _nested.size() - 1;
    }
    void Env::clear()
    {
        _nested.clear();
        enter();
    }
    Env::Coord Env::def(const std::string& name)
    {
        return def(name, Nil{}, currDepth());
    }
    Env::Coord Env::def(const std::string& name, Value::Data value)
    {
        return def(name, value, currDepth());
    }
    Env::Coord Env::def(const std::string& name, Value::Data value, Bytecode depth)
    {
        _Scope& scope = _nested.at(depth);
        if (auto it = scope.map.find(name); it != scope.map.end())
            return {depth, it->second, scope.is_free};
        Bytecode slot = static_cast<Bytecode>(scope.stack.size());
        scope.map[name] = slot;
        scope.stack.push_back(value);
        return {depth, slot, scope.is_free};
    }
    void Env::def(Coord c, Value::Data value)
    {
        _Scope& scope = _nested.at(c.in_free ? currDepth() : c.depth);
        if (c.slot < scope.stack.size())
            return;
        scope.stack.push_back(value);
    }
    Value::Data Env::get(Coord c)
    {
        if (c.depth == REI_BYTECODE_MAX) return Nil{};
        if (c.slot == REI_BYTECODE_MAX) return Nil{};
        Bytecode depth = c.in_free ? currDepth() : c.depth;
        Bytecode slot = c.slot;
        for (;;)
        {
            if (depth >= _nested.size())
                break;
            _Scope& scope = _nested.at(depth);
            if (slot < scope.stack.size())
                return scope.stack.at(slot);
            if (depth == 0)
                break;
            depth--;
        }
        return Nil{};
    }
    void Env::set(Coord c, Value::Data value)
    {
        if (c.depth == REI_BYTECODE_MAX) return;
        if (c.slot == REI_BYTECODE_MAX) return;
        Bytecode depth = c.in_free ? currDepth() : c.depth;
        Bytecode slot = c.slot;
        for (;;)
        {
            if (depth >= _nested.size())
                break;
            _Scope& scope = _nested.at(depth);
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
    bool Env::overlap(const std::string& name)
    {
        _Scope& scope = _nested.at(currDepth());
        return scope.map.contains(name);
    }
    Env::Coord Env::toCoord(const std::string& name)
    {
        return toCoord(name, currDepth());
    }
    Env::Coord Env::toCoord(const std::string& name, Bytecode depth)
    {
        for (;;)
        {
            _Scope& scope = _nested.at(depth);
            if (auto it = scope.map.find(name); it != scope.map.end())
                return {depth, it->second, scope.is_free};
            if (depth == 0)
                break;
            depth--;
        }
        return {};
    }
}
