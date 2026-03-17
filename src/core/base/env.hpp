#pragma once
#include "common.hpp"

namespace rei
{
    namespace Var
    {
        enum Nest : Bytecode
        {
            VN_OUTER,
            VN_INNER
        };
    }
    class Env final
    {
    private:
        struct _Scope final
        {
            std::unordered_map<std::string, Bytecode> map {};
            std::vector<Value::Data> stack {};
        };
    public:
        struct Coord final
        {
            Bytecode depth = REI_BYTECODE_MAX;
            Bytecode slot = REI_BYTECODE_MAX;
        };
    private:
        std::vector<_Scope> _staticBlock {};
    public:
        Env();
        ~Env();
    public:
        void enter();
        void exit();
        Bytecode curr();
    public:
        void clear();
    public:
        Coord def(const std::string& name);
        Coord def(const std::string& name, Value::Data value);
        Coord def(const std::string& name, Value::Data value, Bytecode depth);
    public:
        Value::Data get(const std::string& name);
        Value::Data get(const std::string& name, Bytecode depth);
        Value::Data get(Coord c);
    public:
        void set(const std::string& name, Value::Data value);
        void set(const std::string& name, Value::Data value, Bytecode depth);
        void set(Coord c, Value::Data value);
    public:
        Coord toCoord(const std::string& name);
        Coord toCoord(const std::string& name, Bytecode depth);
    };
}