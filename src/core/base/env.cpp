#include "env.hpp"
#include <iostream>

namespace rei
{
    void Env::enter()
    {
        Area_ scope;
        scope.stack_start = local_stack_.size();
        locals_area_.push_back(std::move(scope));
    }
    void Env::exit()
    {
        local_stack_.resize(locals_area_.back().stack_start);
        locals_area_.pop_back();
    }
    Bytecode Env::currLocalDepth()
    {
        return locals_area_.size();
    }
    void Env::clear()
    {
        global_stack_.clear();
        global_area_.map.clear();
        global_area_.size = 0;
        local_stack_.clear();
        locals_area_.clear();
    }
#pragma region Def
    Value::Coord Env::def(const std::string& name)
    {
        if (locals_area_.empty())
            return defGlobal_(name);
        return defLocal_(name);
    }
    Value::Coord Env::defGlobal_(const std::string& name)
    {
        Area_& scope = global_area_;
        if (auto it = scope.map.find(name); it != scope.map.end())
            return {Value::VLC_GLOBAL, 0, it->second};
        Bytecode slot = static_cast<Bytecode>(global_stack_.size());
        scope.map[name] = slot;
        scope.size++;
        global_stack_.push_back(Nil{});
        return {Value::VLC_GLOBAL, 0, slot};
    }
    Value::Coord Env::defLocal_(const std::string& name)
    {
        Bytecode depth = currLocalDepth() - 1;
        Area_& scope = locals_area_.at(depth);
        if (auto it = scope.map.find(name); it != scope.map.end())
            return {Value::VLC_LOCAL, 0, it->second};
        Bytecode slot = static_cast<Bytecode>(local_stack_.size()) - scope.stack_start;
        scope.map[name] = slot;
        scope.size++;
        local_stack_.push_back(Nil{});
        return {Value::VLC_LOCAL, 0, slot};
    }
    void Env::def(Value::Coord c, Value::Data value)
    {
        if (c.lifecycle == Value::VLC_GLOBAL || locals_area_.empty())
            defGlobal_(c, value);
        else
            defLocal_(c, value);
    }
    void Env::defGlobal_(Value::Coord c, Value::Data value)
    {
        Area_& scope = global_area_;
        if (c.slot < scope.size)
            return;
        scope.size++;
        global_stack_.push_back(value);
    }
    void Env::defLocal_(Value::Coord c, Value::Data value)
    {
        Area_& scope = locals_area_.at(currLocalDepth() - 1 - c.uplevel);
        if (c.slot < scope.size)
            return;
        scope.size++;
        local_stack_.push_back(value);
    }
#pragma endregion
#pragma get/set/overlap
    Value::Data Env::get(Value::Coord c)
    {
        if (!c.isValid()) return Nil{};
        if (c.lifecycle == Value::VLC_GLOBAL)
        {
            Bytecode slot = c.slot;
            Area_& scope = global_area_;
            if (slot < scope.size)
                return global_stack_.at(slot);
        }
        else
        {
            if (locals_area_.empty()) return Nil{};
            Bytecode depth = currLocalDepth() - 1 - c.uplevel;
            Bytecode slot = c.slot;
            for (;;)
            {
                if (depth >= locals_area_.size())
                    break;
                Area_& scope = locals_area_.at(depth);
                if (slot < scope.size)
                    return local_stack_.at(scope.stack_start + slot);
                if (depth == 0)
                    break;
                depth--;
            }
        }
        return Nil{};
    }
    void Env::set(Value::Coord c, Value::Data value)
    {
        if (!c.isValid()) return;
        if (c.lifecycle == Value::VLC_GLOBAL)
        {
            Bytecode slot = c.slot;
            Area_& scope = global_area_;
            if (slot < scope.size)
                global_stack_.at(slot) = value;
        }
        else
        {
            if (locals_area_.empty()) return;
            Bytecode depth = currLocalDepth() - 1 - c.uplevel;
            Bytecode slot = c.slot;
            for (;;)
            {
                if (depth >= locals_area_.size())
                    break;
                Area_& scope = locals_area_.at(depth);
                if (slot < scope.size)
                {
                    local_stack_.at(scope.stack_start + slot) = value;
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
        if (locals_area_.empty())
            return global_area_.map.contains(name);
        Area_& scope = locals_area_.at(currLocalDepth() - 1);
        return scope.map.contains(name);
    }
#pragma endregion
    Value::Coord Env::toCoord(const std::string& name)
    {
        if (!locals_area_.empty())
        {
            Bytecode depth = currLocalDepth() - 1;
            Bytecode uplevel = 0;
            for (;;)
            {
                Area_& scope = locals_area_.at(depth);
                if (auto it = scope.map.find(name); it != scope.map.end())
                    return {Value::VLC_LOCAL, uplevel, it->second};
                if (depth == 0)
                    break;
                depth--;
                uplevel++;
            }
        }
        if (auto it = global_area_.map.find(name); it != global_area_.map.end())
            return {Value::VLC_GLOBAL, 0, it->second};
        return {};
    }
}
