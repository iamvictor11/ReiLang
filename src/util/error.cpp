#include "error.hpp"

namespace luna::Error
{
    bool Reporter::empty()
    {
        return _stack.empty();
    }
    void Reporter::report(const std::string& data, Position pos)
    {
        _stack.emplace_back(data, pos);
    }
    Msg Reporter::pop()
    {
        if (_stack.empty())
            return {"", 0, 0};
        Msg top = _stack.back();
        _stack.pop_back();
        return top;
    }
    void Reporter::clear()
    {
        _stack.clear();
    }
}
