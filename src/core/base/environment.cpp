#include "environment.hpp"

namespace luna
{
    void Environment::decl(const std::string& name, bool is_const)
    {
        _vars[name] = Variable{is_const};
    }
    void Environment::def(const std::string& name, Variable var)
    {
        _vars[name] = var;
    }
    Variable& Environment::get(const std::string& name)
    {
        if (auto it = _vars.find(name); it != _vars.end())
            return it->second;
        return _failVar;
    }
    void Environment::assign(const std::string& name, const Value::Data& value)
    {
        Variable& var = get(name);
        if (var.is_const)
            return;
        var.value = value;
    }
}
