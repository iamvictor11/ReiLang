#pragma once
#include "common.hpp"

namespace rei
{
    class Env final
    {
    private:
        struct _Scope final
        {
            std::unordered_map<std::string, Bytecode> map {};
            size_t stack_start = 0;
            size_t size = 0;
        };
    public:
        struct Coord final
        {
            bool is_global = false;
            Bytecode uplevel = 0;
            Bytecode slot = REI_BYTECODE_MAX;
            bool is_valid() const { return slot != REI_BYTECODE_MAX; }
        };
    private:
        std::vector<Value::Data> _global_stack {};
        _Scope _global {};
        std::vector<Value::Data> _local_stack {};
        std::vector<_Scope> _locals {};
    public:
        void enter();
        void exit();
        Bytecode currLocalDepth();
    public:
        void clear();
    public:
        Coord def(const std::string& name);
        void def(Coord c, Value::Data value);
    private:
        Coord _defGlobal(const std::string& name);
        Coord _defLocal(const std::string& name);
        void _defGlobal(Coord c, Value::Data value);
        void _defLocal(Coord c, Value::Data value);
    public:
        Value::Data get(Coord c);
        void set(Coord c, Value::Data value);
        bool overlap(const std::string& name);
    public:
        Coord toCoord(const std::string& name);
    };
}