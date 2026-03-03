#pragma once
#include <string>
#include <memory>
#include "util/error.hpp"

namespace luna
{
    class Lexer;
    class VM final
    {
    private:
        Error::Reporter _error_reporter;
    public:
        VM() = default;
        ~VM() = default;
    public:
        void runSimple(const std::string& source);
        void runFile(const std::string& path);
    };
}
