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
        Bytecode clvl = _nested.empty() ? 0 : _nested.back().close_level;
        _nested.emplace_back().close_level = clvl;
    }
    void Env::enter(bool is_closed)
    {
        if (is_closed)
        {
            Bytecode clvl = (_nested.empty() ? 0 : _nested.back().close_level) + 1;
            _nested.emplace_back().close_level = clvl;
        }
        else
        {
            Bytecode clvl = _nested.empty() ? 0 : _nested.back().close_level;
            _nested.emplace_back().close_level = clvl;
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
    Bytecode Env::currClosedLevel()
    {
        return _nested.empty() ? 0 : _nested.back().close_level;
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
            return {depth - scope.close_level, it->second, scope.close_level};
        Bytecode slot = static_cast<Bytecode>(scope.stack.size());
        scope.map[name] = slot;
        scope.stack.push_back(value);
        return {depth - scope.close_level, slot, scope.close_level};
    }
    Value::Data Env::get(Coord c)
    {
        if (c.relative_depth == REI_BYTECODE_MAX) return Nil{};
        if (c.slot == REI_BYTECODE_MAX) return Nil{};
        Bytecode depth = c.close_level + c.relative_depth;
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
        Bytecode depth = c.close_level + c.relative_depth;
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
                return {depth - scope.close_level, it->second, scope.close_level};
            if (depth == 0)
                break;
            depth--;
        }
        return {};
    }
}
