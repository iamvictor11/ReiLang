#pragma once
#include "common.hpp"

namespace luna
{
    class Environment final
    {
    private:
        std::unordered_map<std::string, Variable> _vars;
    private:
        std::unique_ptr<Environment> _inner = nullptr;
    public:
        Environment() = default;
        ~Environment() = default;
    public:
        void decl(const std::string& name, bool is_const);
        void def(const std::string& name, const Variable& var);
        bool get(const std::string& name, Variable** outPtr);
        bool assign(const std::string& name, const Value::Data& value);
    public:
        void nest();
        void uest();
    };
}
