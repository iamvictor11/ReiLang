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
            std::vector<Value::Data> stack {};
            bool is_free = false;
        };
    public:
        struct Coord final
        {
            Bytecode depth = REI_BYTECODE_MAX;
            Bytecode slot = REI_BYTECODE_MAX;
            Bytecode in_free = 0;
        };
    private:
        std::vector<_Scope> _nested {};
    public:
        Env();
        ~Env();
    public:
        void enter();
        void enter(bool is_free);
        void exit();
        Bytecode currDepth();
    public:
        void clear();
    public:
        Coord def(const std::string& name);
        Coord def(const std::string& name, Value::Data value);
        Coord def(const std::string& name, Value::Data value, Bytecode depth);
        void def(Coord c, Value::Data value);
    public:
        Value::Data get(Coord c);
        void set(Coord c, Value::Data value);
        bool overlap(const std::string& name);
    public:
        Coord toCoord(const std::string& name);
        Coord toCoord(const std::string& name, Bytecode depth);
    };
}