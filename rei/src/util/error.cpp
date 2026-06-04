#include "error.hpp"
#include <iostream>
#include "debug/log.hpp"

namespace rei::Error
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
        return stack_.empty();
    }
    void Reporter::report(const std::string& data, Position pos)
    {
        REI_DEBUG_LOG_ERROR("{}{}", data, pos.toString());
        stack_.emplace_back(data, pos);
    }
    Msg Reporter::pop()
    {
        if (stack_.empty())
            return {"", 0, 0};
        Msg top = stack_.back();
        stack_.pop_back();
        return top;
    }
    void Reporter::clear()
    {
        stack_.clear();
    }
}
