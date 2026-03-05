#include "error.hpp"
#include <iostream>
#include "debug/log.hpp"

namespace luna::Error
{
    std::string Msg::toString()
    {
        return data + pos.toString();
    }
    void Msg::print()
    {
        std::cout << data << pos.toString();
    }
    void Msg::println()
    {
        std::cout << data << pos.toString() << std::endl;
    }

    bool Reporter::empty()
    {
        return _stack.empty();
    }
    void Reporter::report(const std::string& data, Position pos)
    {
        LUNA_DEBUG_LOG_ERROR("{}", data);
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
