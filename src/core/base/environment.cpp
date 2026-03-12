#include "environment.hpp"

namespace luna
{
    void Environment::decl(const std::string& name, bool is_const)
    {
        if (_inner)
            _inner->decl(name, is_const);
        else
            _vars[name] = Variable{is_const};
    }
    void Environment::def(const std::string& name, const Variable& var)
    {
        if (_inner)
            _inner->def(name, var);
        else
            _vars[name] = var;
    }
    bool Environment::get(const std::string& name, Variable** outPtr)
    {
        if (_inner && _inner->get(name, outPtr))
            return true;
        if (auto it = _vars.find(name); it != _vars.end())
        {
            *outPtr = &(it->second);
            return true;
        }
        return false;
    }
    bool Environment::assign(const std::string& name, const Value::Data& value)
    {
        if (_inner && _inner->assign(name, value))
            return true;
        Variable* var = nullptr;
        if (!get(name, &var))
            return false;
        if (var->is_const)
            return false;
        var->value = value;
        return true;
    }
    void Environment::nest()
    {
        if (_inner)
            _inner->nest();
        else
            _inner = std::make_unique<Environment>();
    }
    void Environment::uest()
    {
        if (_inner->_inner)
            _inner->uest();
        else
            _inner.reset();
    }
}
