#pragma once
#include "common.hpp"

namespace luna
{
    class Environment final
    {
    private:
        std::unordered_map<std::string, Variable> _vars;
        Variable _failVar = Variable{Nil{}, true};
    public:
        Environment() = default;
        ~Environment() = default;
    public:
        void decl(const std::string& name, bool is_const);
        void def(const std::string& name, Variable var);
        Variable& get(const std::string& name);
    };
}
