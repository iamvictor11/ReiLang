#pragma once
#include <string>
#include <vector>
#include "core/base/common.hpp"

namespace vic::Error
{
    struct Msg final
    {
    public:
        std::string data;
        Position pos;
    public:
        std::string toString();
        void print();
        void println();
    };
    using Stack = std::vector<Msg>;
    class Reporter final
    {
    private:
        Stack _stack = {};
    public:
        bool empty();
        void report(const std::string& data, Position pos);
        Msg pop();
        void clear();
    };
}
