#pragma once
#include <string>
#include <vector>
#include "core/base/common.hpp"

namespace luna::Error
{
    struct Msg final
    {
        std::string data;
        Position pos;
    };
    using Stack = std::vector<Msg>;
    class Reporter final
    {
    private:
        Stack _stack = {};
    public:
        bool empty();
        void report(const std::string& data, Position pos);
        [[nodiscard]] Msg pop();
        void clear();
    };
}
