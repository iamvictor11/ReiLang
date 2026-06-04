#include "env.runtime.hpp"

namespace rei
{
    void Env<PS_RUNTIME>::enter()
    {
        locals_area_sp_.push_back(local_stack_.size());
    }
    void Env<PS_RUNTIME>::exit()
    {
        local_stack_.resize(locals_area_sp_.back());
        locals_area_sp_.pop_back();
    }
    Bytecode Env<PS_RUNTIME>::currLocalDepth()
    {
        return locals_area_sp_.size();
    }
    void Env<PS_RUNTIME>::clearHost()
    {
        host_stack_.clear();
    }
    void Env<PS_RUNTIME>::clearCache()
    {
        global_stack_.clear();
        local_stack_.clear();
        locals_area_sp_.clear();
    }
#pragma region Def
    void Env<PS_RUNTIME>::defHost(const Value::Data& value)
    {
        host_stack_.push_back(value);
    }
    void Env<PS_RUNTIME>::defGlobal(const Value::Data& value)
    {
        global_stack_.push_back(value);
    }
    void Env<PS_RUNTIME>::defLocal(const Value::Data& value)
    {
        local_stack_.push_back(value);
    }
#pragma endregion
#pragma region Get
    const Value::Data& Env<PS_RUNTIME>::getHost(Bytecode slot)
    {
        return host_stack_.at(slot);
    }
    const Value::Data& Env<PS_RUNTIME>::getGlobal(Bytecode slot)
    {
        return global_stack_.at(slot);
    }
    const Value::Data& Env<PS_RUNTIME>::getLocal(Bytecode uplevel, Bytecode slot)
    {
        return local_stack_.at(
            locals_area_sp_.at(
                locals_area_sp_.size() - 1 - uplevel
            ) + slot
        );
    }
#pragma endregion
#pragma region Set
    void Env<PS_RUNTIME>::setHost(Bytecode slot, const Value::Data& value)
    {
        host_stack_.at(slot) = value;
    }
    void Env<PS_RUNTIME>::setGlobal(Bytecode slot, const Value::Data& value)
    {
        global_stack_.at(slot) = value;
    }
    void Env<PS_RUNTIME>::setLocal(Bytecode uplevel, Bytecode slot, const Value::Data& value)
    {
        local_stack_.at(
            locals_area_sp_.at(
                locals_area_sp_.size() - 1 - uplevel
            ) + slot
        ) = value;
    }
#pragma endregion
}
