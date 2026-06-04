#pragma once
#include "env.hpp"

namespace rei
{
    template<>
    class Env<PS_RUNTIME>
    {
    private:
        std::vector<Value::Data> host_stack_ {};
        std::vector<Value::Data> global_stack_ {};
        std::vector<Value::Data> local_stack_ {};
        std::vector<Bytecode> locals_area_sp_ {};
    public:
        void enter();
        void exit();
    public:
        Bytecode currLocalDepth();
    public:
        void clearHost();
        void clearCache();
    public:
        void defHost(const Value::Data& value);
        void defGlobal(const Value::Data& value);
        void defLocal(const Value::Data& value);
    public:
        const Value::Data& getHost(Bytecode slot);
        const Value::Data& getGlobal(Bytecode slot);
        const Value::Data& getLocal(Bytecode uplevel, Bytecode slot);
    public:
        void setHost(Bytecode slot, const Value::Data& value);
        void setGlobal(Bytecode slot, const Value::Data& value);
        void setLocal(Bytecode uplevel, Bytecode slot, const Value::Data& value);
    };
}
