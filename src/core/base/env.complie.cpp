#include "env.complie.hpp"
#include <iostream>
#include "debug/log.hpp"

namespace rei
{
    void Env<PS_COMPILE>::enter()
    {
        Area_ area;
        area.stack_start = local_stack_size_;
        locals_area_.push_back(std::move(area));
    }
    void Env<PS_COMPILE>::exit()
    {
        local_stack_size_ = locals_area_.back().stack_start;
        locals_area_.pop_back();
    }
    Bytecode Env<PS_COMPILE>::currLocalDepth()
    {
        return locals_area_.size();
    }
    void Env<PS_COMPILE>::clearHost()
    {
        host_area_.map.clear();
        host_area_.size = 0;
    }
    void Env<PS_COMPILE>::clearCache()
    {
        global_area_.map.clear();
        global_area_.size = 0;
        locals_area_.clear();
        local_stack_size_ = 0;
    }
#pragma region Def
    Bytecode Env<PS_COMPILE>::defHost(const std::string& name)
    {
        Area_& area = host_area_;
        if (auto it = area.map.find(name); it != area.map.end())
        {
            Bytecode slot = it->second;
            REI_DEBUG_LOG_WARNING("原 host 变量 {} 将被覆盖", name);
            return slot;
        }
        Bytecode slot = static_cast<Bytecode>(area.size);
        area.map[name] = slot;
        area.size++;
        return slot;
    }
    Value::Coord Env<PS_COMPILE>::def(const std::string& name)
    {
        if (locals_area_.empty())
            return defGlobal_(name);
        return defLocal_(name);
    }
    Value::Coord Env<PS_COMPILE>::defGlobal_(const std::string& name)
    {
        Area_& area = global_area_;
        if (auto it = area.map.find(name); it != area.map.end())
            return {Value::VLT_GLOBAL, 0, REI_BYTECODE_MAX};
        Bytecode slot = static_cast<Bytecode>(area.size);
        area.map[name] = slot;
        area.size++;
        return {Value::VLT_GLOBAL, 0, slot};
    }
    Value::Coord Env<PS_COMPILE>::defLocal_(const std::string& name)
    {
        Area_& area = locals_area_.back();
        if (auto it = area.map.find(name); it != area.map.end())
            return {Value::VLT_LOCAL, 0, REI_BYTECODE_MAX};
        Bytecode slot = static_cast<Bytecode>(area.size);
        area.map[name] = slot;
        area.size++;
        local_stack_size_++;
        return {Value::VLT_LOCAL, 0, slot};
    }
#pragma endregion
#pragma region Get/Set/Overlap
    bool Env<PS_COMPILE>::overlap(const std::string& name)
    {
        if (locals_area_.empty())
            return global_area_.map.contains(name);
        Area_& area = locals_area_.at(currLocalDepth() - 1);
        return area.map.contains(name);
    }
#pragma endregion
    Value::Coord Env<PS_COMPILE>::toCoord(const std::string& name)
    {
        if (!locals_area_.empty())
        {
            Bytecode depth = currLocalDepth() - 1;
            Bytecode uplevel = 0;
            for (;;)
            {
                Area_& area = locals_area_.at(depth);
                if (auto it = area.map.find(name); it != area.map.end())
                    return {Value::VLT_LOCAL, uplevel, it->second};
                if (depth == 0)
                    break;
                depth--;
                uplevel++;
            }
        }
        if (auto it = global_area_.map.find(name); it != global_area_.map.end())
            return {Value::VLT_GLOBAL, 0, it->second};
        if (auto it = host_area_.map.find(name); it != host_area_.map.end())
            return {Value::VLT_HOST, 0, it->second};
        return {};
    }
}
