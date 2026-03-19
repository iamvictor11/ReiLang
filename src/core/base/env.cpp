#include "env.hpp"
#include <iostream>

namespace rei
{
    void Env::enter()
    {
        _Scope scope;
        scope.stack_start = _local_stack.size();
        _locals.push_back(std::move(scope));
    }
    void Env::exit()
    {
        _local_stack.resize(_locals.back().stack_start);
        _locals.pop_back();
    }
    Bytecode Env::currLocalDepth()
    {
        return _locals.size();
    }
    void Env::clear()
    {
        _global_stack.clear();
        _global.map.clear();
        _global.size = 0;
        _local_stack.clear();
        _locals.clear();
    }
#pragma region Def
    Env::Coord Env::def(const std::string& name)
    {
        if (_locals.empty())
            return _defGlobal(name);
        return _defLocal(name);
    }
    Env::Coord Env::_defGlobal(const std::string& name)
    {
        _Scope& scope = _global;
        if (auto it = scope.map.find(name); it != scope.map.end())
            return {true, 0, it->second};
        Bytecode slot = static_cast<Bytecode>(_global_stack.size());
        scope.map[name] = slot;
        scope.size++;
        _global_stack.push_back(Nil{});
        return {true, 0, slot};
    }
    Env::Coord Env::_defLocal(const std::string& name)
    {
        Bytecode depth = currLocalDepth() - 1;
        _Scope& scope = _locals.at(depth);
        if (auto it = scope.map.find(name); it != scope.map.end())
            return {false, 0, it->second};
        Bytecode slot = static_cast<Bytecode>(_local_stack.size()) - scope.stack_start;
        scope.map[name] = slot;
        scope.size++;
        _local_stack.push_back(Nil{});
        return {false, 0, slot};
    }
    void Env::def(Coord c, Value::Data value)
    {
        if (c.is_global || _locals.empty())
            _defGlobal(c, value);
        else
            _defLocal(c, value);
    }
    void Env::_defGlobal(Coord c, Value::Data value)
    {
        _Scope& scope = _global;
        if (c.slot < scope.size)
            return;
        scope.size++;
        _global_stack.push_back(value);
    }
    void Env::_defLocal(Coord c, Value::Data value)
    {
        _Scope& scope = _locals.at(currLocalDepth() - 1 - c.uplevel);
        if (c.slot < scope.size)
            return;
        scope.size++;
        _local_stack.push_back(value);
    }
#pragma endregion
#pragma get/set/overlap
    Value::Data Env::get(Coord c)
    {
        if (!c.is_valid()) return Nil{};
        if (c.is_global)
        {
            Bytecode slot = c.slot;
            _Scope& scope = _global;
            if (slot < scope.size)
                return _global_stack.at(slot);
        }
        else
        {
            if (_locals.empty()) return Nil{};
            Bytecode depth = currLocalDepth() - 1 - c.uplevel;
            Bytecode slot = c.slot;
            for (;;)
            {
                if (depth >= _locals.size())
                    break;
                _Scope& scope = _locals.at(depth);
                if (slot < scope.size)
                    return _local_stack.at(scope.stack_start + slot);
                if (depth == 0)
                    break;
                depth--;
            }
        }
        return Nil{};
    }
    void Env::set(Coord c, Value::Data value)
    {
        if (!c.is_valid()) return;
        if (c.is_global)
        {
            Bytecode slot = c.slot;
            _Scope& scope = _global;
            if (slot < scope.size)
                _global_stack.at(slot) = value;
        }
        else
        {
            if (_locals.empty()) return;
            Bytecode depth = currLocalDepth() - 1 - c.uplevel;
            Bytecode slot = c.slot;
            for (;;)
            {
                if (depth >= _locals.size())
                    break;
                _Scope& scope = _locals.at(depth);
                if (slot < scope.size)
                {
                    _local_stack.at(scope.stack_start + slot) = value;
                    return;
                }
                if (depth == 0)
                    break;
                depth--;
            }
        }
    }
    bool Env::overlap(const std::string& name)
    {
        if (_locals.empty())
            return _global.map.contains(name);
        _Scope& scope = _locals.at(currLocalDepth() - 1);
        return scope.map.contains(name);
    }
#pragma endregion
    Env::Coord Env::toCoord(const std::string& name)
    {
        if (!_locals.empty())
        {
            Bytecode depth = currLocalDepth() - 1;
            Bytecode uplevel = 0;
            for (;;)
            {
                _Scope& scope = _locals.at(depth);
                if (auto it = scope.map.find(name); it != scope.map.end())
                    return {false, uplevel, it->second};
                if (depth == 0)
                    break;
                depth--;
                uplevel++;
            }
        }
        if (auto it = _global.map.find(name); it != _global.map.end())
            return {true, 0, it->second};
        return {};
    }
}
