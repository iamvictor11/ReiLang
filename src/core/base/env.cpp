#include "env.hpp"
#include <iostream>
#include "debug/log.hpp"

namespace rei
{
    void Env::enter()
    {
        Area_ area;
        area.stack_start = local_stack_.size();
        locals_area_.push_back(std::move(area));
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
    void Env::clearHost()
    {
        host_stack_.clear();
        host_area_.map.clear();
        host_area_.size = 0;
    }
    void Env::clearCache()
    {
        global_stack_.clear();
        global_area_.map.clear();
        global_area_.size = 0;
        local_stack_.clear();
        locals_area_.clear();
    }
    Value::Coord Env::bind(const std::string& name, const Value::Data& val)
    {
        Area_& area = host_area_;
        if (auto it = area.map.find(name); it != area.map.end())
        {
            Bytecode slot = it->second;
            auto& ov = host_stack_[slot];
            REI_DEBUG_LOG_WARNING("{}: {} 覆盖原绑定 {}", name, Value::getDebugString(val), Value::getDebugString(ov));
            ov = val;
            return {Value::VLC_HOST, 0, slot};
        }
        Bytecode slot = static_cast<Bytecode>(host_stack_.size());
        area.map[name] = slot;
        area.size++;
        host_stack_.push_back(val);
        return {Value::VLC_HOST, 0, slot};
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
        Area_& area = global_area_;
        if (auto it = area.map.find(name); it != area.map.end())
            return {Value::VLC_GLOBAL, 0, it->second};
        Bytecode slot = static_cast<Bytecode>(global_stack_.size());
        area.map[name] = slot;
        area.size++;
        global_stack_.push_back(Nil{});
        return {Value::VLC_GLOBAL, 0, slot};
    }
    Value::Coord Env::defLocal_(const std::string& name)
    {
        Bytecode depth = currLocalDepth() - 1;
        Area_& area = locals_area_.at(depth);
        if (auto it = area.map.find(name); it != area.map.end())
            return {Value::VLC_LOCAL, 0, it->second};
        Bytecode slot = static_cast<Bytecode>(local_stack_.size()) - area.stack_start;
        area.map[name] = slot;
        area.size++;
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
        Area_& area = global_area_;
        if (c.slot < area.size)
            return;
        area.size++;
        global_stack_.push_back(value);
    }
    void Env::defLocal_(Value::Coord c, Value::Data value)
    {
        Area_& area = locals_area_.at(currLocalDepth() - 1 - c.uplevel);
        if (c.slot < area.size)
            return;
        area.size++;
        local_stack_.push_back(value);
    }
#pragma endregion
#pragma region Get/Set/Overlap
    Value::Data Env::get(Value::Coord c)
    {
        if (!c.isValid()) return Nil{};
        if (c.lifecycle == Value::VLC_HOST)
        {
            Bytecode slot = c.slot;
            if (slot < host_area_.size)
                return host_stack_.at(slot);
        }
        else if (c.lifecycle == Value::VLC_GLOBAL)
        {
            Bytecode slot = c.slot;
            if (slot < global_area_.size)
                return global_stack_.at(slot);
        }
        else
        {
            if (locals_area_.empty()) return Nil{};
            return local_stack_.at(locals_area_.at(currLocalDepth() - 1 - c.uplevel).stack_start + c.slot);
        }
        return Nil{};
    }
    void Env::set(Value::Coord c, Value::Data value)
    {
        if (!c.isValid()) return;
        if (c.lifecycle == Value::VLC_HOST)
        {
            Bytecode slot = c.slot;
            if (slot < host_area_.size)
                host_stack_.at(slot) = value;
        }
        else if (c.lifecycle == Value::VLC_GLOBAL)
        {
            Bytecode slot = c.slot;
            if (slot < global_area_.size)
                global_stack_.at(slot) = value;
        }
        else
        {
            if (locals_area_.empty()) return;
            Bytecode depth = currLocalDepth() - 1 - c.uplevel;
            Bytecode slot = c.slot;
            local_stack_.at(locals_area_.at(currLocalDepth() - 1 - c.uplevel).stack_start + c.slot) = value;
        }
    }
    bool Env::overlap(const std::string& name)
    {
        if (locals_area_.empty())
            return global_area_.map.contains(name);
        Area_& area = locals_area_.at(currLocalDepth() - 1);
        return area.map.contains(name);
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
                Area_& area = locals_area_.at(depth);
                if (auto it = area.map.find(name); it != area.map.end())
                    return {Value::VLC_LOCAL, uplevel, it->second};
                if (depth == 0)
                    break;
                depth--;
                uplevel++;
            }
        }
        if (auto it = global_area_.map.find(name); it != global_area_.map.end())
            return {Value::VLC_GLOBAL, 0, it->second};
        if (auto it = host_area_.map.find(name); it != host_area_.map.end())
            return {Value::VLC_HOST, 0, it->second};
        return {};
    }
}
