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
        Bytecode flvl = _nested.empty() ? 0 : _nested.back().free_level;
        _nested.emplace_back().free_level = flvl;
    }
    void Env::enter(bool is_free)
    {
        if (is_free)
        {
            Bytecode flvl = (_nested.empty() ? 0 : _nested.back().free_level) + 1;
            _nested.emplace_back().free_level = flvl;
        }
        else
        {
            Bytecode flvl = _nested.empty() ? 0 : _nested.back().free_level;
            _nested.emplace_back().free_level = flvl;
        }
    }
    void Env::exit()
    {
        _nested.pop_back();
    }
    Bytecode Env::currDepth()
    {
        return _nested.size() - 1;
    }
    Bytecode Env::currFreeLevel()
    {
        return _nested.empty() ? 0 : _nested.back().free_level;
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
            return {depth - scope.free_level, it->second, scope.free_level};
        Bytecode slot = static_cast<Bytecode>(scope.stack.size());
        scope.map[name] = slot;
        scope.stack.push_back(value);
        return {depth - scope.free_level, slot, scope.free_level};
    }
    void Env::def(Coord c, Value::Data value)
    {
        _Scope& scope = _nested.at(c.relative_depth + c.free_level);
        if (c.slot < scope.stack.size())
            return;
        scope.stack.push_back(value);
    }
    Value::Data Env::get(Coord c)
    {
        if (c.relative_depth == REI_BYTECODE_MAX) return Nil{};
        if (c.slot == REI_BYTECODE_MAX) return Nil{};
        if (c.free_level == REI_BYTECODE_MAX) return Nil{};
        Bytecode depth = c.free_level + c.relative_depth;
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
        if (c.relative_depth == REI_BYTECODE_MAX) return;
        if (c.slot == REI_BYTECODE_MAX) return;
        if (c.free_level == REI_BYTECODE_MAX) return;
        Bytecode depth = c.free_level + c.relative_depth;
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
                return {depth - scope.free_level, it->second, scope.free_level};
            if (depth == 0)
                break;
            depth--;
        }
        return {};
    }
}
