#include "env.hpp"

namespace luna
{
    void Env::defGlobal(const std::string& name, Value::Data value)
    {
        _globals[name] = value;
    }
    Value::Data Env::getGlobal(const std::string& name)
    {
        auto it = _globals.find(name);
        if (it != _globals.end())
            return it->second;
        return Nil{};
    }
    void Env::setGlobal(const std::string& name, Value::Data value)
    {
        auto it = _globals.find(name);
        if (it != _globals.end())
            it->second = value;
    }
    void Env::pushFrame()
    {
        _frames.emplace_back();
    }
    void Env::popFrame()
    {
        if (!_frames.empty())
            _frames.pop_back();
    }
    size_t Env::addLocal(Value::Data value)
    {
        if (_frames.empty()) pushFrame();
        _frames.back().locals.push_back(value);
        return _frames.back().locals.size() - 1;
    }
    Value::Data Env::getLocal(size_t index)
    {
        if (_frames.empty()) return Nil{};
        if (index >= _frames.back().locals.size()) return Nil{};
        return _frames.back().locals[index];
    }
    void Env::setLocal(size_t index, Value::Data value)
    {
        if (_frames.empty()) return;
        if (index < _frames.back().locals.size())
            _frames.back().locals[index] = value;
    }
    Value::Data Env::getLocalInEnclosing(size_t depth, size_t index)
    {
        Env* env = this;
        for (size_t i = 0; i < depth && env; i++)
            env = env->_frames.empty() ? nullptr : env->_frames.back().enclosing;
        if (env && !env->_frames.empty() && index < env->_frames.back().locals.size())
            return env->_frames.back().locals[index];
        return Nil{};
    }
}
