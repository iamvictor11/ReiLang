#pragma once
#include "common.hpp"

namespace rei
{
    namespace Value
    {
        enum Lifecycle : Bytecode
        {
            VLC_HOST,
            VLC_GLOBAL,
            VLC_LOCAL,
            VLC_UPVALUE
        };
        struct Coord final
        {
            Lifecycle lifecycle = VLC_GLOBAL;
            Bytecode uplevel = 0;
            Bytecode slot = REI_BYTECODE_MAX;
            bool isValid() const { return slot != REI_BYTECODE_MAX; }
        };
    }
    class Env final
    {
    private:
        struct Area_ final
        {
            std::unordered_map<std::string, Bytecode> map {};
            size_t stack_start = 0;
            size_t size = 0;
        };
    private:
        std::vector<Value::Data> host_stack_ {};
        Area_ host_area_ {};
        std::vector<Value::Data> global_stack_ {};
        Area_ global_area_ {};
        std::vector<Value::Data> local_stack_ {};
        std::vector<Area_> locals_area_ {};
    public:
        void enter();
        void exit();
        Bytecode currLocalDepth();
    public:
        void clearHost();
        void clearCache();
    public:
        Value::Coord bind(const std::string& name, const Value::Data& val);
    public:
        Value::Coord def(const std::string& name);
        void def(Value::Coord c, Value::Data value);
    private:
        Value::Coord defGlobal_(const std::string& name);
        Value::Coord defLocal_(const std::string& name);
        void defGlobal_(Value::Coord c, Value::Data value);
        void defLocal_(Value::Coord c, Value::Data value);
    public:
        Value::Data get(Value::Coord c);
        void set(Value::Coord c, Value::Data value);
        bool overlap(const std::string& name);
    public:
        Value::Coord toCoord(const std::string& name);
    };
}