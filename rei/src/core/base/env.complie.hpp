#pragma once
#include "env.hpp"

namespace rei
{
    template<>
    class Env<PS_COMPILE>
    {
    private:
        struct Area_ final
        {
            std::unordered_map<std::string, Bytecode> map {};
            size_t stack_start = 0;
            size_t size = 0;
        };
    private:
        Area_ host_area_ {};
        Area_ global_area_ {};
        std::vector<Area_> locals_area_ {};
        size_t local_stack_size_ = 0;
    public:
        void enter();
        void exit();
    public:
        Bytecode currLocalDepth();
    public:
        void clearHost();
        void clearCache();
    public:
        Bytecode defHost(const std::string& name);
        Value::Coord def(const std::string& name);
    private:
        Value::Coord defGlobal_(const std::string& name);
        Value::Coord defLocal_(const std::string& name);
    public:
        bool overlap(const std::string& name);
    public:
        Value::Coord toCoord(const std::string& name);
    };
}