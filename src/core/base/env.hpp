#pragma once
#include "common.hpp"

namespace luna
{
    class Env final
    {
    private:
        struct Frame final
        {
            std::vector<Value::Data> locals;
            Env* enclosing;
            size_t address;
        };
    private:
        std::unordered_map<std::string, Value::Data> _globals;
        std::vector<Frame> _frames;
    public:
        Env() = default;
        void defGlobal(const std::string& name, Value::Data value);
        Value::Data getGlobal(const std::string& name);
        void setGlobal(const std::string& name, Value::Data value);
        void pushFrame();
        void popFrame();
        size_t addLocal(Value::Data value);
        Value::Data getLocal(size_t index);
        void setLocal(size_t index, Value::Data value);
        Value::Data getLocalInEnclosing(size_t depth, size_t index);
    };
}